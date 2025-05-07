#include <stdio.h>

int main()
{
	int num1 = 1;
	int num2 = 2;
	int tmp = 0;

	num1 = num1^num2;
	num2 = !num1 ^ num2;

	printf("%d/%d", num1,num2);
}