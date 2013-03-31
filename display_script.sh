#!/bin/sh
set -n
PIDFILE=/tmp/mpirun_3q7BY0.pid
USAGE="USAGE: $0 [ -i | history | recent | current ]"
NODES="node132 node133 node134 node135 node136 node137 node138 node139 node140 node141 node142 node143 node144 node145 node146 node147"
PROGNAME=distributed # for pgrep to find the right program
PROG_CWD=""
COMBINED_DIR="/home/hps1/evolutionaryimages/output" # / rank / file

# finds the working directory of our program so we can find the files
get_cwd_of_program() {
    case `uname` in

    Darwin)
        TMP=`pgrep $PROGNAME`
        if test "$TMP"; then
            # assumes the path does not have a "=" in it, works for processes spawned by bash, but not the initial bash itself
            TMP="`ps -A -E | grep "^$TMP " | sed -e 's/^.*PWD=//' -e 's/=.*$//' -e 's/ [A-Z]*$//'`"
            if test -d $TMP; then
                PROG_CWD="$TMP"
            else
                echo "$FUNCNAME: $TMP is not a valid directory" 1>&2
            fi
        fi
        ;;

    OpenBSD|NetBSD)
        TMP=`pgrep $PROGNAME`
        if test "$TMP"; then
            # assumes the path does not have a "=" in it, works for processes spawned by bash, but not the initial bash itself
            TMP="`ps aux -e | grep "^[a-z_]* *$TMP " | sed -e 's/^.*PWD=//' -e 's/=.*$//' -e 's/ [A-Z]*$//'`"
            if test -d $TMP; then
                PROG_CWD="$TMP"
            else
                echo "$FUNCNAME: $TMP is not a valid directory" 1>&2
            fi
        fi
        ;;

    FreeBSD)
        if [ "`ls /proc`" ]; then
            TMP=`pgrep $PROGNAME`                                                                                       
            if test "$TMP"; then
                # assumes the path does not have a "=" in it, works for processes spawned by bash, but not the initial bash itself
                TMP="`export COLUMNS=1000 && ps aux -e | grep "^[a-z_]* *$TMP " | sed -e 's/^.*PWD=//' -e 's/=.*$//' -e 's/ [A-Z]*$//'`"
                if test -d $TMP; then
                    PROG_CWD="$TMP"
                else
                    echo "$FUNCNAME: $TMP is not a valid directory" 1>&2
                fi
            fi
        else
            echo "Could not get environment. Rty mouning /proc filesystem and try again" 1>&2
        fi
        ;;

    Linux)
        TMP=`pgrep $PROGNAME`
        if test "$TMP"; then
            # assumes the path does not have a "=" in it, does not reflect changes since program start
            TMP="`ps -p $TMP e | sed -e 's/^.*PWD=//' -e 's/=.*$//' -e 's/ [A-Z]*$//'`"
            if test -d $TMP; then
                PROG_CWD="$TMP"
            else
                echo "$FUNCNAME: $TMP is not a valid directory" 1>&2
            fi
        else
            echo "$FUNCNAME: $TMP is not a valid directory" 1>&2
        fi
        ;;

      *)
        echo "$FUNCNAME: operating system `uname` unknown" 1>&2
        ;;

    esac
}

# need to make change to distributed.cpp
history_view() {
    pkill -2 $$
    # get all pictures sorted by time
    pushd "$COMBINED_DIR"
        A=(`ls -td *`) # assumes no spaces in file names
    popd
    LEN=${#A[*]}
    k=0
    for i in $NODES; do
        let INDEX=LEN/${#NODES}*k
        ssh -f $i "/root/displayImage \"$COMBINED_DIR/${A[$INDEX]}\""
        let k=k+1
    done
}

current_view() {
    pkill -2 $$
    MPI_PROCESS=0
    for i in $NODES; do
        pushd "$PROG_CWD/output/$MPI_PROCESS"
            # get name of most recent picture
            MOST_RECENT="`ls -t | head -n 1`"
        popd
        # display it
        ssh -f $i "/root/displayImage \"$PROG_CWD/output/$MPI_PROCESS/$MOST_RECENT/$MOST_RECENT\""
        let MPI_PROCESS=MPI_PROCESS+1
    done
}

# displays an image on a single node
# currently not used
# args: computer image
display_image() {
    if test -f "$2"; then
        ssh -f "$1" "/script/path/x \"$2\""
    else
        echo "$FUNCNAME: file \"$2\" does not exist or is a relative path that is not valid from /root/stephen" 1>&2
        exit 1
    fi
}

kill_individual_image_displays() {
    for i in $NODES; do
        ssh -f $i "pkill displayImage"
    done
}

# args: image in shared folder
mpi_display_image() {
    if (cd /root/stephen/ && test -f "$1"); then
        mpirun -mca btl tcp,self -np 16 -x DISPLAY=0:0 /root/stephen/mpiDispImg2 "$1" 1>/dev/null 2>/dev/null &
        echo $! > $PIDFILE
    else
        echo "$FUNCNAME: file \"$1\" does not exist or is a relative path that is not valid from /root/stephen" 1>&2
        exit 1
    fi
}

kill_mpirun() {
    if test -f $PIDFILE; then
        kill `cat $PIDFILE` 2>/dev/null 1>/dev/null
        rm $PIDFILE
    fi
}

interrupt() {
    kill_mpirun
    kill_individual_image_displays
}

trap interrupt 2,15

interactive_loop() {
    echo "interactive mode not implemented"
    exit 0
}

########### BEGIN ############
get_cwd_of_program

case "$1" in
    -h|--help)
        echo "$USAGE"
    ;;
    -i|--interactive)
        interactive_loop
    ;;
    [hH]|hist|history)
          history_view
    ;;
    recent)
          # intended to be like history but with more recent pics
          echo "recent not implemented"  
    ;;
    current)
          current_view
    ;;
    *)
        echo "invalid option: " "$1"
        echo "$USAGE"
    ;;
esac



exit 0
