#include <stdio.h>
int main(){
	int x1,x2,x3,y1,y2,y3;
	printf("Enter the first coordinate: ");
	scanf("%d %d",&x1,&y1); 
	printf("Enter the second coordinate: ");
	scanf("%d %d",&x2,&y2); 
	printf("Enter the third coordinate: ");
	scanf("%d %d",&x3,&y3); 
	double area= (x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/2.0;
	printf("The area is: %.2f\n",area);
	return 0;
}
