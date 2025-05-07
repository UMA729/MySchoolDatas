#include <stdio.h>
#include <string.h>

int main()
{
	char str[5][20] = { "" };
	int set_num = 5;
	int i;
	int j;

	for (i = 0; i < set_num; i++)
	{
		scanf_s("%s", &str[i], 20);
	}

	for (i = 0; i < set_num ; i++)
	{
		for (j = 1; j < set_num; j++)
		{
			int result = strcmp(str[i], str[j],20);
			if (result > 0)
			{
				char tmp = {""};
				strcpy_s(tmp,   20,str[i]);
				strcpy_s(str[i],20, str[j]);
				strcpy_s(str[j],20, tmp);
			}
		}
	}
	for (i = 0; i < set_num; i++)
	{
		printf("%s\n", str[i]);
	}
	
	return 0;
}