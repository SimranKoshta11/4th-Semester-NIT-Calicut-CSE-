#!/bin/bash

if [ $# -ne 1 ]; then
	echo "The file name was not provided as an argument"
	exit 1
fi
filename="$1"
basename=$(basename "$filename" .c)

if [[ "$filename" != *.c ]]; then
	echo "Error: The file must have a .c extension."
	exit 1
elif [ ! -f "$filename" ]; then
	echo "Error: File $filename does not exist in the current directory"
	exit 1
fi
# compile to object file
gcc -c "$filename" -o "${basename}.o"
if [ $? -ne 0 ]; then
	echo "Failed to create object file (${basename}.o)."
	exit 1
else 
	echo "object file (${basename}.o) created successfully."
fi
# generate preprocessed file
gcc -E "$filename" -o "${basename}.i"
if [ $? -ne 0 ]; then
	echo "Failed to create preprocessed file (${basename}.i)."
	exit 1
else 
	echo "preprocessed file (${basename}.i) created successfully."
fi
# generate assembly file
gcc -S "$filename" -o "${basename}.s"
if [ $? -ne 0 ]; then
	echo "Failed to create assembly file (${basename}.s)."
	exit 1
else
	echo "Assembly file (${basename}.s) created successfully."
fi
#create executable file
gcc "$filename" -o final_exec
if [ $? -ne 0 ]; then
	echo "Failed to create executable (final_exec)."
	exit 1
else
	echo "Executable file created successfully."
fi

