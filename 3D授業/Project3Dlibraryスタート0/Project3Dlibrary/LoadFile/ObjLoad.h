#pragma once
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

namespace OBJ_FILE
{

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
		float u, v;
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

	//�I�u�W�F�N�g�t�@�C���ǂݍ��݃N���X
	class ObjFile
	{
	public:
		ObjFile()
		{
			FILE* fp;
			fopen_s(&fp, "3DModelResource\\Grapefruit.obj", "r");

			if (fp == nullptr)
			{
				printf("�����ς�");
				return;
			}

			//�t�@�C�����
			int eof{ -1 };
			char str[256];
			do
			{
				eof = fscanf_s(fp, "%s", str, 256);

				if (strcmp(str, "v") == 0)
				{
					Pos pos;//���_�ʒu
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.x);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.y);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &pos.z);
					m_Pos.push_back(pos);//�O���[�o���ϐ��Ƀf�[�^������

				}
				if (strcmp(str, "vt") == 0)
				{
					Uv uv;//���_�ʒu
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &uv.u);
					eof = fscanf_s(fp, "%s", str, 256);
					sscanf_s(str, "%f", &uv.v);
					m_Uv.push_back(uv);

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

					m_Face.push_back(f);

				}
			} while (eof != EOF);

			return;
		}
		//�t�@�C���f�[�^�����郁���o�ϐ�
		vector<Pos>	 m_Pos;
		vector<Uv>	 m_Uv;
		vector<Nol>	 m_Nol;
		vector<Face> m_Face;
	};
}