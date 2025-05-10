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
struct Node{
	int data;
	struct Node* next;
};
typedef struct Node* node;
void insert_at_end(node *head,int val){
	node x=(node)my_malloc(sizeof(struct Node));
	if(x==NULL)
		return ;
	x->data=val;
	x->next=NULL;
	if(*head==NULL)
		*head=x;
	else{
		node y=*head;
		while(y->next!=NULL){
			y=y->next;
		}
		y->next=x;
	}
}
void delete_at_position(node *head,int pos){
	if(*head==NULL)
		return ;
	if(pos==0){
		node x=*head;
		*head=(*head)->next;
		my_free(x);
	}
	else{
		int count=0;
		node x=*head;
		node prev=NULL;
		while(x!=NULL && count!=pos){
			prev=x;
			x=x->next;
			count++;
		}
		if(x==NULL){
			printf("Invalid\n");
			return ;
		}
		prev->next=x->next;
		my_free(x);
	}
}
void print_list(node head){
	node x=head;
	while(x!=NULL){
		printf("%d",x->data);
		if(x->next)
			printf(" -> ");
		x=x->next;
	}
	printf("\n");
}
void free_list(node *head){
	node x=	*head;
	while(x!=NULL){
		node tmp=x;
		x=x->next;
		my_free(tmp);
	}
	*head=NULL;
}
int main(){
	node head=NULL;
	for(int i=0; i<10; i++){
		insert_at_end(&head,i+1);
	}
	printf("Initial List: ");
	print_list(head);
	delete_at_position(&head,2);
	printf("After Deleting at Position 2: ");
	print_list(head);
	free_list(&head);
	return 0;
}
