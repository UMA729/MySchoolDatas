#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

//位置
class Pos
{
public:
	float x, y, z;
};

//テクスチャ座標位置
class Uv
{
public:
	float u,v;
};

//法線（面の向き）
class Nol
{
public:
	float x, y, z;
};

//レイアウト
class Layout
{
public:
	int pos, uv, nol;
};

//面
class Face
{
public:
	vector<Layout> v;
};

//ファイルデータを入れるグローバル変数
vector<Pos>	 g_Pos;
vector<Uv>	 g_Uv;
vector<Nol>	 g_Nol;
vector<Face> g_Face;

int main()
{
	FILE* fp;
	fopen_s(&fp, "Grapefruit.obj", "r");

	if (fp == nullptr)
	{
		printf("しっぱい");
		return 1;
	}

	//ファイル解析
	int eof{ -1 };
	char str[256];
	do
	{
		eof = fscanf_s(fp, "%s", str, 256);
		printf("%s\n", str);
		
		if (strcmp(str, "v") == 0)
		{
			Pos pos;//頂点位置
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.x);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.y);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.z);
			g_Pos.push_back(pos);//グローバル変数にデータを入れる
			printf("x=%f,y=%f,z=%f \n", pos.x, pos.y, pos.z);

		}
		if (strcmp(str, "vt") == 0)
		{
			Uv uv;//頂点位置
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &uv.u);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &uv.v);
			g_Uv.push_back(uv);
			printf("u=%f,v=%f \n", uv.u, uv.v);

		}
		if (strcmp(str, "vn") == 0)
		{
			Nol nol;//頂点位置
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &nol.x);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &nol.y);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &nol.z);
			g_Nol.push_back(nol);
			printf("x=%f,y=%f,z=%f \n", nol.x,nol.y, nol.z);

		}

		if (strcmp(str, "f") == 0)
		{
			Face f;
			for (int i = 0;i < 3;i++)
			{
				Layout L;
				eof = fscanf_s(fp, "%s", str, 256);
				sscanf_s(str, "%d/%d/%d", &L.pos, &L.uv, &L.nol);
				f.v.push_back(L);
			}
			//三角なのか？四角なのか？
			eof = fscanf_s(fp, "%s", str, 256);
			if (strcmp(str, "f") != 0 && eof != EOF)
			{
				//四角の場合
				Layout L;
				sscanf_s(str, "%d/%d/%d", &L.pos, &L.uv, &L.nol);
				f.v.push_back(L);
			}
			else
			{
				//三角の場合
				fseek(fp, -1, SEEK_CUR);//fを読み込んだ分fpを戻す
			}

			g_Face.push_back(f);

		}
	} while (eof != EOF);

	return 0;
}