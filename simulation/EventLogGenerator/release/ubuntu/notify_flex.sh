#!/bin/bash

process=Ratchet
if [ $# -eq 1 ]; then
	process=$1
fi

pid=$(ps -ef | grep ${process} | grep -v grep | grep -v notify_flex | awk '{print $2}')
#echo "pid = ${pid}"
if [ "$pid" ]; then
	kill -SIGUSR1 $pid
#	kill -SIGALRM $pid
else
	echo "can't get process ${process}"
fi


