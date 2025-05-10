#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
void f1(){
	int* arr=(int*)malloc(5*sizeof(int));   
	if(!arr){
		printf("Memory allocation failed\n");
		return ;
	}
	
	int un_var=0; 
	printf("Unitialized Variable: %d\n",un_var);   
	
	for(int i=0; i<5; i++){ 
		arr[i]=i*10;     
	}
	
	int *null_ptr=NULL; 
	if(null_ptr)
		*null_ptr=42;    
	
	int x=INT_MAX;  
	if(x+1<x)
		printf("Overflow\n");        
	free(arr);
}
	
int main(){
	f1();
	return 0;
}
