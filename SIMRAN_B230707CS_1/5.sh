#!/bin/bash
if [ $# -ne 1 ]; then
	echo "Please provide the file name."
	exit 1
fi
file=$1
basename=$(basename "$file" .c)
if [[ "$file" != *.c ]]; then
	echo "File must have .c exetention."
	exit 1
elif [ ! -f "$file" ]; then
	echo "File was not founded."
	exit 1
fi
if [ ! -f "libarith.so" ]; then
	echo "Creating the shared library libarith.so"
	gcc -fPIC -shared -o libarith.so arith.c
	if [ $? -ne 0 ]; then
		echo "Failed to create shared library libarith.so."
		exit 1
	else
		echo "Shared librar libarith.so created successfully."
	fi
fi
echo "Compiling $file and linking with libarith.so"
gcc "$file" -o "$basename" -L. -larith -Wl,-rpath,. 2> compile.log
if [ $? -ne 0 ]; then
	echo "Compilation failed."
	exit 1
else
	echo "Compilation Successful."
fi
