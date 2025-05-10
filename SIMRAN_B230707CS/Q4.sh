#!/bin/bash
is_prime() {
	num=$1
	if [ $num -le 1 ] ; then
		return 1
	fi
	for ((i=2; i*i<=num; i++)); do
		if [ $((num % i)) -eq 0 ]; then
			return 1
		fi
	done
	return 0
}
echo "Enter the numbers: "
read -a arr
p_count=0
for num in "${arr[@]}"; do
	is_prime $num
	if [ $? -eq 0 ]; then
		p_count=$(($p_count + 1));
	fi
done
echo "Number of prime numbers in the array is : $p_count."
