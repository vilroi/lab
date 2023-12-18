#!/bin/sh

if [ $# -ne 1 ];then
	echo "usage: $0 pid"
	exit 1
fi

pid=$1
watch -n 0.1 "sudo ./ptdump/ptdump -g $pid | grep rwx"
