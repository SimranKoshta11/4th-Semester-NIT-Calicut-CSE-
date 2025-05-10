#!/bin/bash
is_palindrome(){
	local str="$1"
	local n=${#str}
	for (( i=0; i<n/2; i++ )); do
		if [ "${str:i:1}" != "${str:n-i-1:1}" ]; then
			return 1
		fi
	done
	return 0
}
echo -n "Enter the String array: "
read -r input
string=($input)
for str in "${string[@]}"; do
	if is_palindrome "$str"; then
		echo "$str "
	fi
done
