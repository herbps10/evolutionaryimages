#!/bin/bash

PIDFILE=/tmp/mpirun_3q7BY0.pid
USAGE="USAGE: $0 [ -i | history | early | recent | current | kill ]"
NODES="node132 node133 node134 node135 node136 node137 node138 node139 node140 node141 node142 node143 node144 node145 node146 node147"
NODE_COUNT=`wc -w <<<"$NODES"`
PROGNAME=distributed # for pgrep to find the right program
COMBINED_DIR="/home/srk3/evolving_images/output" # / rank / file
WORKING_DIR="/home/srk3/evolving_images"


history_view() {
	kill -2 $$
	# get all pictures sorted by time
	pushd "$COMBINED_DIR" >/dev/null
		A=(`ls -tr *`) # assumes no spaces in file names and no directories within
	popd >/dev/null
	LEN=${#A[*]}
	k=0
	for i in $NODES; do
		let INDEX=LEN/NODE_COUNT*k
		ssh -f $i "export DISPLAY=:0 && \"$WORKING_DIR\"/local_disp_image \"$COMBINED_DIR/${A[$INDEX]}\""
		let k=k+1
	done
}

#shows more early pics
weighted_history_view() {
	kill -2 $$
	# get all pictures sorted by time
	pushd "$COMBINED_DIR" >/dev/null
		A=(`ls -tr *`) # assumes no spaces in file names and no directories within
	popd >/dev/null
	LEN=${#A[*]}
	k=0
	for i in $NODES; do
		let INDEX=`expr $LEN / \( $NODE_COUNT - $k \) / $NODE_COUNT \* $k`
	echo index is $INDEX
		ssh -f $i "export DISPLAY=:0 && \"$WORKING_DIR\"/local_disp_image \"$COMBINED_DIR/${A[$INDEX]}\""
		let k=k+1
	done
}

local_current_view() {
	kill -2 $$
	MPI_PROCESS=0
	for i in $NODES; do
		pushd "$COMBINED_DIR" >/dev/null
			# get name of most recent picture
			MOST_RECENT="`ls -t $MPI_PROCESS-* | head -n 1`"
		popd >/dev/null
		# display it
		ssh -f $i "export DISPLAY=:0 && \"$WORKING_DIR\"//local_disp_image \"$COMBINED_DIR/$MOST_RECENT\""
		let MPI_PROCESS=MPI_PROCESS+1
	done
}

stretched_current_view() {
	kill -2 $$
	mpi_display_image "$COMBINED_DIR/`ls -t $COMBINED_DIR | head -n 1`"
}

kill_individual_image_displays() {
	echo -n killing\ 
	for i in $NODES; do
		echo -n "$i "
		ssh -f $i "pkill -2 local_disp_im" 1>/dev/null 2>/dev/null
	done
	echo
}

# args: image in shared folder
mpi_display_image() {
	if (cd $COMBINED_DIR && test -f "$1"); then
		mpirun -mca btl tcp,self -np 16 --hostfile default_hosts -x DISPLAY mpi_disp_image "$1" 1>/dev/null 2>/dev/null &
		echo $! > $PIDFILE
	else
		echo "$FUNCNAME: file \"$1\" does not exist or is a relative path that is not valid from $WORKING_DIR" 1>&2
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

trap interrupt INT
trap interrupt TERM

interactive_loop() {
	echo "interactive mode not implemented"
	exit 0
}

########### BEGIN ############
# get_cwd_of_program
cd "$WORKING_DIR" || exit 1
if test -z "`ls "$WORKING_DIR"`"; then
	echo "no pictures to display" 1>&2
	exit 1
fi

while test "$1"; do
	echo "$i"
	
	case "$1" in
		-h|--help)
			echo "$USAGE"
		;;
		-i|--interactive)
			interactive_loop
		;;
		[hH]|hist|history)
			echo history_view
			history_view
		;;
		[eE]|early)
			echo weighted_history_view
			weighted_history_view
		;;
		recent)
			 # intended to be like history but with more recent pics
			 echo "recent not implemented"  
		;;
		-c|current)
			echo local_current_view
			local_current_view
		;;
		-C|CURRENT)
			echo stretched_current_view
			stretched_current_view
		;;
		-k|--kill|kill)
			interrupt 
		;;
		-s|--sleep|sleep)
			 if [ "`<<<"$2" tr -d 0-9`" ]; then
		 echo sleeping for 1 day
				 sleep 1d
			 else
				 sleep $2
				 shift
			 fi
		;;
		*)
			echo "invalid option: " "$1"
			echo "$USAGE"
		;;
	esac

	shift
	
done


exit 0
