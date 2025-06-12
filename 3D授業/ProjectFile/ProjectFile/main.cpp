#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

//�ʒu
class Pos
{
public:
	float x, y, z;
};

//�e�N�X�`�����W�ʒu
class Uv
{
public:
	float u,v;
};

//�@���i�ʂ̌����j
class Nol
{
public:
	float x, y, z;
};

//���C�A�E�g
class Layout
{
public:
	int pos, uv, nol;
};

//��
class Face
{
public:
	vector<Layout> v;
};

//�t�@�C���f�[�^������O���[�o���ϐ�
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
		printf("�����ς�");
		return 1;
	}

	//�t�@�C�����
	int eof{ -1 };
	char str[256];
	do
	{
		eof = fscanf_s(fp, "%s", str, 256);
		printf("%s\n", str);
		
		if (strcmp(str, "v") == 0)
		{
			Pos pos;//���_�ʒu
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.x);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.y);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &pos.z);
			g_Pos.push_back(pos);//�O���[�o���ϐ��Ƀf�[�^������
			printf("x=%f,y=%f,z=%f \n", pos.x, pos.y, pos.z);

		}
		if (strcmp(str, "vt") == 0)
		{
			Uv uv;//���_�ʒu
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &uv.u);
			eof = fscanf_s(fp, "%s", str, 256);
			sscanf_s(str, "%f", &uv.v);
			g_Uv.push_back(uv);
			printf("u=%f,v=%f \n", uv.u, uv.v);

		}
		if (strcmp(str, "vn") == 0)
		{
			Nol nol;//���_�ʒu
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
			//�O�p�Ȃ̂��H�l�p�Ȃ̂��H
			eof = fscanf_s(fp, "%s", str, 256);
			if (strcmp(str, "f") != 0 && eof != EOF)
			{
				//�l�p�̏ꍇ
				Layout L;
				sscanf_s(str, "%d/%d/%d", &L.pos, &L.uv, &L.nol);
				f.v.push_back(L);
			}
			else
			{
				//�O�p�̏ꍇ
				fseek(fp, -1, SEEK_CUR);//f��ǂݍ��񂾕�fp��߂�
			}

			g_Face.push_back(f);

		}
	} while (eof != EOF);

	return 0;
}