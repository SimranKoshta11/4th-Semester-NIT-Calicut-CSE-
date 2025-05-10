#!/bin/bash
if [ $# -ne 1 ]; then
	echo "Please provide the file name."
	exit 1
fi
file=$1
basename=$(basename "$file" .c)
if [[ "$file" != *.c ]]; then
	echo "Error: the file must have a .c extention."
	exit 1
fi
if [ ! -f "$file" ]; then
	echo "file was not found."
	exit 1
fi
logfile="compile.log"
>"$logfile"
echo "Preprocessing ($file)..."
gcc -E "$file" -o "${basename}.i" 2>> "$logfile"
if [ $? -ne 0 ]; then
	echo "Preprocessing is not done."
	exit 1
else
	echo "Preprocessing completed."
fi
echo "Generating assembly"
gcc -S "$file" -o "${basename}.s" 2>> "$logfile"
if [ $? -ne 0 ]; then
	echo "Assembly is not generated."
	exit 1
else
	echo "Assembly completed."
fi
echo "Compiling to object file"
gcc -c "$file" -o "${basename}.o" 2>> "$logfile"
if [ $? -ne 0 ]; then
	echo "Object file is not created."
	exit 1
else
	echo "Object file created."
fi
echo "Create executable file"
gcc "${basename}.o" -o final 2>> "$logfile"
if [ $? -ne 0 ]; then
	echo "Executable file is not created."
	exit 1
else
	echo "Executable file created."
fi
warnings=$(grep -c "warning:" "$logfile")
errors=$(grep -c "error:" "$logfile")
echo "Warnings: $warnings"
echo "Errors: $errors"
