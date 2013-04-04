#!/bin/bash

if [ "`whoami`" == srk3 ]; then
    mpirun -np 16 --hostfile /home/srk3/evolving_images/default_hosts /home/srk3/evolving_images/bin/distributed "$1"
else
    mpirun -np 16 /home/hps1/evolutionaryimages/bin/distributed
fi
