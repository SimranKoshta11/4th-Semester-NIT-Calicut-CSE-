#!/bin/bash
if [ -z "$1" ]; then
	echo "Please provide a file_name."
	exit 1
fi
if [ ! -f "$1" ]; then 
	echo "file '$1' not found!"
	exit 1
fi

nl -s ': ' "$1"
