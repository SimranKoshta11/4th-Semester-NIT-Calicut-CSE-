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
	void* brk = sbrk(size + bsize);
	if(brk == (void*)-1)
	        return NULL;
	new = (block)brk;
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
void* my_realloc(void* ptr, size_t size) {
    if(size==0){
    	my_free(ptr);
    	return NULL;
    }
    if(!ptr)
    	return my_malloc(size);
    block bl=addr(ptr);
    size_t old_size=bl->size;
    void* new_ptr=my_malloc(size);
    if(new_ptr){
    	memcpy(new_ptr,ptr,old_size<size ? old_size: size);
    	my_free(ptr);
    }
    return new_ptr;
}

struct st {
    int *data;
    int size;
    int capacity;
};
typedef struct st* darr;

void init_array(darr* arr, int is) {
    *arr = (darr)my_malloc(sizeof(struct st));
    (*arr)->data = (int*)my_malloc(is * sizeof(int));
    (*arr)->size = 0;
    (*arr)->capacity = is;
    printf("Initial array created with capacity: %d\n", (*arr)->capacity);
}

void resize_array(darr* arr, int ns) {
    if (ns <= 0) {
        printf("Invalid new size!\n");
        return;
    }

    int* new_data = (int*)my_realloc((*arr)->data, ns * sizeof(int));
    
    if (!new_data) {
        printf("Memory reallocation failed for size %d!\n", ns);
        return;
    }

    (*arr)->data = new_data;
    (*arr)->capacity = ns;
    if ((*arr)->size > ns)
        (*arr)->size = ns;
    printf("Resized array to new capacity: %d\n", (*arr)->capacity);
}


void insert_element(darr* arr, int value) {
    if ((*arr)->size == (*arr)->capacity) {
        resize_array(arr, (*arr)->capacity * 2);
    }
    (*arr)->data[(*arr)->size++] = value;
    printf("Inserted %d\n", value);
}

void remove_element(darr* arr, int index) {
    if (index < 0 || index >= (*arr)->size) {
        printf("Invalid index\n");
        return;
    }
    for (int i = index; i < (*arr)->size - 1; i++) {
        (*arr)->data[i] = (*arr)->data[i + 1];
    }
    (*arr)->size--;

    if ((*arr)->size < (*arr)->capacity / 4 && (*arr)->capacity > 1) {
        resize_array(arr, (*arr)->capacity / 2);
    }
    printf("Removing element at index %d \n", index);
}

void free_array(darr* arr) {
    my_free((*arr)->data);
    my_free(*arr);
    printf("Memory Freed Successfully\n");
}

void print_array(darr* arr) {
    if (!arr || !(*arr) || !(*arr)->data) {
        printf("Array is empty or memory allocation failed!\n");
        return;
    }
    printf("Array contents: [");
    for (int i = 0; i < (*arr)->size; i++) {
        printf("%d", (*arr)->data[i]);
        if (i < (*arr)->size - 1)
            printf(" , ");
    }
    printf("]\n");
}

int main() {
    darr arr;
    init_array(&arr, 5);
    insert_element(&arr, 10);
    insert_element(&arr, 20);
    insert_element(&arr, 30);
    print_array(&arr);
    remove_element(&arr, 1);
    print_array(&arr);
    resize_array(&arr,10);
    print_array(&arr);
    free_array(&arr);
    return 0;
}

