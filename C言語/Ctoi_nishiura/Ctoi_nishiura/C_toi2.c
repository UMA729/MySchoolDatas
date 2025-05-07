#include <stdio.h>

int main()
{
	int res = 0;  //c‚è”
	int div1 = 0; //‘f”‚©’²‚×‚é‚½‚ß‚Ì–ñ”1
	int pow = 1;  //—İæ
	int div2 = 0; //‘f”‚©’²‚×‚é‚½‚ß‚Ì–ñ”2
	int i;
	int j;
	int k;
	int l;

	for (i = 1;res <= 10000;i++)
	{
		div1 = 0;
		div2 = 0;
		pow = 1;
		for (j = 1;j <= i;j++)
		{

			if (i % j == 0)
			{
				div1++;
			}

			if (div1 == 2)
			{
				for (k = 1;k <= i;k++)
				{
					pow *= 2;
				}
				for (l = 1;l <= pow;l++)
				{
					if ((pow - 1) % l == 0)
					{
						div2++;
					}
				}
			}

			if (div2 == 2)
			{
				res = (pow / 2) * (pow - 1);
				if(res <= 10000)
				printf("%d\n", res);
			}
		}
	}

	return 0;
}