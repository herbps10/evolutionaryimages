#!/bin/sh
while true; do for i in history current early CURRENT; do ./display_script.sh $i; sleep 30; done; done
