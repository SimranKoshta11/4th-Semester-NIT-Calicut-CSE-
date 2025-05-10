#!/bin/bash
if [ $# -ne 2 ]; then
	echo "Please provide the file name"
	exit 1
fi
file=$1
flag=$2
if [[ "$file" != *.c ]]; then
	echo "Error: The file must have a .c exetension."
	exit 1
elif [ ! -f "$file" ]; then
	echo "Error : File $file does not exist."
	exit 1
fi
case "$flag" in
	-E) echo "Performing pre-processing (-E)..."
 	    basename=$(basename "$file" .c)
	    gcc -E "$file" -o "${basename}.i"
	    if [ $? -ne 0 ]; then
	    	echo "Error pre processing was not created."
	    	exit 1
	    else
	    	echo "pre processing completed : file ${basename}.i created."
	    	exit 1
	    fi
	    ;;
	-S) echo "Generating assembly code (-S)..."
	    basename=$(basename "$file" .c)
	    gcc -S "$file" -o "${basename}.s"
	    if [ $? -ne 0 ]; then
	    	echo "Error assembly code was not created."
	    	exit 1
	    else
	    	echo "Assembly code created : file ${basename}.s created."
	    	exit 1
	    fi
	    ;;
	-c) echo "Compiling to object file (-c)..."
	    basename=$(basename "$file" .c)
	    gcc -c "$file" -o "${basename}.o"
	    if [ $? -ne 0 ]; then
	    	echo "Error object file was not created."
	    	exit 1
	    else
	    	echo "Object file created : file ${basename}.o created."
	    	exit 1
	    fi
	    ;;
	 *) echo "Error: Invalid Flag"
	    exit 1
	    ;;
esac
