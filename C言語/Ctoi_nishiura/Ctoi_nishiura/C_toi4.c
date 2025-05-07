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
				printf("\n連続して入力された数が見つかりました。");
				flag = true;
			}
			printf("\n%d", num[i]);
		}
	}
	if (flag == false)
	{
		printf("連続して入力された数が見つかりませんでした。");
	}
	
	
	return 0;
}