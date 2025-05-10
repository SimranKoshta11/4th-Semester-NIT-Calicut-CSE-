#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
void f1(){
	int* arr=(int*)malloc(5*sizeof(int));   // Memory allocation issue
	int i;
	if(!arr){
		printf("Memory allocation failed\n");
		return ;
	}
	
	int un_var; // Uninitialized variable issue
	printf("Unitialized Variable: %d\n",un_var);   // Garbage value
	
	for(i=0; i<=5; i++){ // Array out-of-bounds issue
		arr[i]=i*10;     // Writing beyond allocated memory
	}
	
	int *null_ptr=NULL; // Null pointer issue
	*null_ptr=42;    // Dereferencing NULL pointer (crash)
	
	int x=INT_MAX;  // Integer overflow
	x=x+1;         // Overflow, undefined behavior
	free(arr);
}
	
int main(){
	f1();
	return 0;
}
