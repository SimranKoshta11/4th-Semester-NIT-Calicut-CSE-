#!/bin/bash
for (( i=1; i<=100; i++ )); do
	if [ $(( i % 11 )) -eq 0 ]; then
		echo -n "$i "
	fi
done
echo
