#!/bin/bash
if [ $# -lt 1 ]; then
	echo "Please provide the file names with .c extention"
	exit 1
fi

for file in "$@" ; do
	if [[ "$file" != *.c ]]; then
		echo "The given file is not extention .c"
		exit 1
	fi
done
for file in "$@" ; do
	if [ ! -f "$file" ]; then
		echo "The given file is not exist in the current directory"
		exit 1
	fi
done
object_files=()
for file in "$@" ; do
	basename=$(basename "$file" .c)
	#gcc -c "$file" -o "${basename}.o"
	obj_file="${basename}.o"
	object_files+=("$obj_file")
	if [ ! -f "$obj_file" ] || [ "$file" -nt "$obj_file" ]; then
		 echo "Compiling $file into $obj_file..."
        	 gcc -c "$file" -o "$obj_file"
		 if [ $? -ne 0 ]; then
			echo "Failed to compile the $file."
			exit 1
		 else
			echo "$file compiled successfully."
		 fi
	fi
done

gcc "${object_files[@]}" -o project_exec
if [ $? -ne 0 ]; then
	echo "Error :the executable file was not created."
	exit 1
else 
	echo "Executable project_exec created successfully."
fi
