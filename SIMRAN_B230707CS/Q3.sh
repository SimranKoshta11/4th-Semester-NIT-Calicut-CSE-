#!/bin/bash
file_count(){
	count=$(ls -p | grep -v / | wc -l)
	echo "Number of files in the present working directory: $count"
}
file_count
