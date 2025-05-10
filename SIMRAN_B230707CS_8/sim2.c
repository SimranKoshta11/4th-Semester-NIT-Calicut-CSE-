#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>

#define align4(x) (((((x)-1)>>2)<<2)+4)
#define bsize 20

struct mblock{
	int free;
	size_t size;
	char data[1];
	void* ptr;
	struct mblock* next;
	struct mblock* prev;
};
typedef struct mblock* block;
block base=NULL;
void split(block bl,size_t size){
	if(bl->size<size+bsize)
		return ;
	block new;
	new=(block)((char*)bl->data+size);
	new->size=bl->size-size-bsize;
	new->free=1;
	new->ptr=new->data;
	new->next=bl->next;
	bl->next=new;
	bl->size=size;
	if(new->next)
		new->next->prev=new;
}
block extend(block last,size_t size){
	block new;
	new=(block)sbrk(size+bsize);
	if(sbrk(size+bsize)==(void*)-1)
		return NULL;
	new->size=size;
	new->free=0;
	new->next=NULL;
	if(last)
		last->next=new;
	new->prev=last;
	new->ptr=new->data;
	return new;
}
block find(block *last,size_t size){
	if(!base)
		return NULL;
	block bl=base;
	while(bl && !(bl->free && bl->size>=size)){
		*last=bl;
		bl=bl->next;
	}
	return bl;
}
void* my_malloc(size_t size){
	block last,fit;
	size_t a;
	a=align4(size);
	if(base){
		last=base;
		fit=find(&last,a);
		if(fit){
			if(fit->size-a>=bsize){
				split(fit,a);
			}
			fit->free=0;
		}
		else{
			fit=extend(last,a);
			if(!fit)
				return NULL;
			base=fit;
		}
	}
	else{
		fit=extend(NULL,a);
		if(!fit)
			return NULL;
		base=fit;
	}
	return fit->data;
}
block merge(block bl){
	if(bl->next && bl->next->free){
		bl->size+=bsize+bl->next->size;
		bl->next=bl->next->next;
		if(bl->next)
			bl->next->prev=bl;
	}
	return bl;
}
block addr(void *p){
	return (block)((char*)p-bsize);
}
int valid(void* p){
	if(base){
		if(p>(void*)base && p<sbrk(0))
			return (p == ((block)((char*)p - offsetof(struct mblock, data)))->ptr);
	}
	return 0;
}
void my_free(void *ptr){
	if(valid(ptr)){
		block bl=addr(ptr);
		bl->free=1;
		if(bl->prev && bl->prev->free)
			bl=merge(bl->prev);
		if(bl->next && bl->next->free)
			bl=merge(bl);
		if(bl->next==NULL){
			if(bl->prev)
				bl->prev->next=NULL;
			else
				base=NULL;
			sbrk(-(bl->size+bsize));
		}
	}
}
void fill_matrix(int** mat,int row,int col){
	int val=1;
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			mat[i][j]=val;
			val++;
		}
	}
}
void print_matrix(int** mat,int row,int col){
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			printf("%d ",mat[i][j]);
		}
		printf("\n");
	}
}
void free_matrix(int** mat,int row){
	if (!mat) return;
	for(int i=0; i<row; i++){
		if(mat[i])
			my_free(mat[i]);
	}
	my_free(mat);
}			
void *my_calloc(size_t num,size_t size){
	void* new;
	new=my_malloc(num*size);
	if(new){
		memset(new,0,num*size);
	}
	return new;
}
int** create_matrix(int row,int col){
	int** mat=(int**)my_calloc(row,sizeof(int*));
	if(mat==NULL){
		printf("Failed\n");
		return NULL;
	}
	for(int i=0; i<row; i++){
		mat[i]=(int*)my_calloc(col,sizeof(int));
		if(mat[i]==NULL){
			printf("Failed\n");
			 for (int j = 0; j < i; j++) {
                		my_free(mat[j]);
            		  }
           		  my_free(mat);
            		  return NULL;
		}
	}
	return mat;
}
int main(){
	int row=3,col=3;
	int** matrix=create_matrix(row,col);
	printf("Matrix Initialised with Zero Values:\n");
	print_matrix(matrix,row,col);
	fill_matrix(matrix,row,col);
	printf("Matrix After Filling with Values:\n");
	print_matrix(matrix,row,col);
	free_matrix(matrix,row);
	printf("Memory Freed Successfully.\n");
	return 0;
}	
