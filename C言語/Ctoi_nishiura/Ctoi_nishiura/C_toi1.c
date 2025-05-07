#include <stdio.h>

int main()
{
	int set = 0;
	int sam = 0;
	int num1 = 0;
	int num2 = 0;
	int i;

	printf("整数の組の数を入力してください。\n");
	scanf_s("%d", &set);
	if (set > 5)
	{
		return 0;
	}

	printf("整数の組を入力してください。\n");
	for (i = 0;i < set;i++)
	{
		scanf_s("%d", &num1);
		scanf_s("%d", &num2);

		if (num1 == num2)
			sam += num1 * num2;
		else
			sam += num1 + num2;
	}

	printf("合計は%dです。\n", sam);

	return 0;
}
