#include <stdio.h>
#include <stdbool.h>

int main()
{
	int num[5] = { 0 };
	bool flag = false;
	int i;

	for (i = 0; i < 5; i++)
	{
		scanf_s("%d", &num[i]);
	}

	for (i = 0; i < 5; i++)
	{
		printf("%d", num[i]);
	}

	for (i = 1; i <= 5; i++)
	{
		if (num[i] == num[i - 1])
		{
			if (flag == false)
			{
				printf("\n�A�����ē��͂��ꂽ����������܂����B");
				flag = true;
			}
			printf("\n%d", num[i]);
		}
	}
	if (flag == false)
	{
		printf("�A�����ē��͂��ꂽ����������܂���ł����B");
	}
	
	
	return 0;
}