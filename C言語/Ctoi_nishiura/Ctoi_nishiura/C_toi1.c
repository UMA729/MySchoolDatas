#include <stdio.h>

int main()
{
	int set = 0;
	int sam = 0;
	int num1 = 0;
	int num2 = 0;
	int i;

	printf("�����̑g�̐�����͂��Ă��������B\n");
	scanf_s("%d", &set);
	if (set > 5)
	{
		return 0;
	}

	printf("�����̑g����͂��Ă��������B\n");
	for (i = 0;i < set;i++)
	{
		scanf_s("%d", &num1);
		scanf_s("%d", &num2);

		if (num1 == num2)
			sam += num1 * num2;
		else
			sam += num1 + num2;
	}

	printf("���v��%d�ł��B\n", sam);

	return 0;
}
