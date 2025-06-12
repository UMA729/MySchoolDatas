#pragma once
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

namespace OBJ_FILE
{

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
		float u, v;
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

	//オブジェクトファイル読み込みクラス
	class ObjFile
	{
	public:
		ObjFile()
		{
			FILE* fp;
			fopen_s(&fp, "3DModelResource\\Grapefruit.obj", "r");

			if (fp == nullptr)
			{
				printf("しっぱい");
				return;
			}

			//ファイル解析
			int eof{ -1 };
			char str[256];
			do
			{
				eof = fscanf_s(fp, "%s", str, 256);

				if (strcmp(str, "v") == 0)
				{
					Pos pos;//頂点位置
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.x);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.y);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.z);
					m_Pos.push_back(pos);//グローバル変数にデータを入れる

				}
				if (strcmp(str, "vt") == 0)
				{
					Uv uv;//頂点位置
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &uv.u);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &uv.v);
					m_Uv.push_back(uv);

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
					m_Nol.push_back(nol);

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

					m_Face.push_back(f);

				}
			} while (eof != EOF);

			return;
		}
		//ファイルデータを入れるメンバ変数
		vector<Pos>	 m_Pos;
		vector<Uv>	 m_Uv;
		vector<Nol>	 m_Nol;
		vector<Face> m_Face;
	};
}