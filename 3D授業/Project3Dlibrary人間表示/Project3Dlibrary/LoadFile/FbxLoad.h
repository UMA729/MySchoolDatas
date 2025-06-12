#pragma once

#define NOMINMAX
#include<windows.h>
#include<gdiplus.h>		//GDI+���g�p
#include<shlwapi.h>

#include <stdio.h>
#include <vector>
#include <array>
#include <map>
#include<codecvt>
#include<cassert>
#include<string.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#pragma comment (lib,"gdiplus.lib")//GDI+��lib�ǂݍ���
#pragma comment (lib,"shlwapi.lib")//SHCreateMemStream�g�p

using namespace std;


namespace FBX_FILE
{
	//�m�[�h
	class Node
	{
	public:
		int32_t index;			//���̃m�[�h�̃C���f�b�N�X
		int32_t parent_index;	//���̃m�[�h�̐e�̃C���f�b�N�X
		string parent_name;		//���̃m�[�h�̐e�m�[�h�̖��O
		string access_name;		//���̃m�[�h�̐ڑ���m�[�h�̖��O
		float mat[16];			//���̃m�[�h�������[�J���s��i�ʒu�E�g��E��]�j
		vector<int32_t>mesh_id;	//���̃m�[�h�������b�V��ID
	};

	//�o�[�e�b�N�X
	class MeshVertex
	{
	public:
		float pos[3];//���_
		float nol[3];//�@��
		float uv[3]; //UV
		float tan[3];//�ڑ�
	};

	//�}�e���A��
	class Material
	{
	public:
		float diffuse[3];	   //�f�t�B�[�Y(�g�U����)
		float speclar[3];	   //�X�y�L�����i���ʔ��ˁj
		float ambient;		   //�A���r�G���g�i�����i�^���j�j
		float shininess;	   //�V���C��
		string diffuseMap;	   //�f�t�B�[�Y�}�b�v���@����͂���
		string specularMap;	   //�X�y�L�����}�b�v��
		string shininessMap;   //�V���C���}�b�v��
		string normalMap;      //�m�[�}���}�b�v��
	};

	//�e�N�X�`��
	class TexImage
	{
	public:
		string name;//�e�N�X�`���l�[��
		int32_t w, h;//�e�N�X�`���̖{���̑傫��
		float const_img[512][512][4];//�k������512�~512�e�N�X�`�����
	};
	//���b�V��
	class Mesh
	{
	public:
		string					name;		//���b�V����
		vector<MeshVertex>		vertexs;	//�o�[�e�b�N�X
		vector<uint32_t>		index; 		//�C���f�b�N�X
		int32_t					materialId;	//���̃��b�V���̃}�e���A��ID
	};

	//FBX�t�@�C�����[�h
	class FbxFile
	{
	public:
		FbxFile()
		{
			//GDI�{�̏����ݒ�
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
				return;


			Assimp::Importer importer;	//Assimp���f�[�^
			int flag{ 0 };				//Assimp��̓t���O


			flag |= aiProcess_Triangulate;				//���ׂĎO�p�`k
			flag |= aiProcess_CalcTangentSpace;			//�ڐG�x�N�g���Ə]�ڐ�x�N�g�����v�Z
			flag |= aiProcess_GenSmoothNormals;			//�@�����Ȃ��ꍇ�A�@�����쐬
			flag |= aiProcess_GenUVCoords;				//UV�L��
			flag |= aiProcess_RemoveRedundantMaterials;	//�g�p���Ă��Ȃ��ގ��͍폜
			flag |= aiProcess_OptimizeMeshes;			//�V�[���K�w�̍œK��
			flag |= aiProcess_FlipUVs;					//Y�����]
			flag |= aiProcess_JoinIdenticalVertices;	//���f���̒��_�͓���Ƃ��Ĉ���

			//Assimp�ł̓ǂݍ���
			string str{ "3DModelResource\\Jump.fbx" };
			const aiScene* pScene = importer.ReadFile(str.c_str(), flag);

			//�m�[�h���
			ParseNode(pScene->mRootNode, node);

			//���b�V�����
			meshs.resize(pScene->mNumMeshes);
			for (size_t i = 0; i < meshs.size(); i++)
			{
				ParseMesh(pScene->mMeshes[i], meshs[i]);


			}

			//�}�e���A���f�[�^
			material.resize(pScene->mNumMaterials);
			for (size_t i = 0; i < pScene->mNumMaterials; i++)
				ParseMaterial(material[i], pScene->mMaterials[i]);

			//�e�N�X�`���f�[�^
			tex.resize(pScene->mNumTextures);
			for (size_t i = 0; i < pScene->mNumTextures; i++)
				ParseTextures(pScene->mTextures[i], tex[i]);

			Gdiplus::GdiplusShutdown(gdiplusToken);

			return ;
		}
		//�m�[�h�f�[�^�l���֐�
		void ParseNode(aiNode* ai_node, map<string, Node>& node)
		{
			if (ai_node == nullptr)return;

			//MAP�Ƀm�[�h�ǉ�
			node.insert(make_pair(ai_node->mName.C_Str(), Node()));

			//�ڑ����m�[�h���O�̊l���i�e�m�[�h�j
			if (ai_node->mParent != nullptr)
			{
				node[ai_node->mName.C_Str()].parent_name = ai_node->mParent->mName.C_Str();
				node[ai_node->mName.C_Str()].parent_index = node[ai_node->mParent->mName.C_Str()].index;
			}
			else
			{
				//�m�[�h�̑�{�Ȃ̂Őڑ���͂Ȃ��Bparent_index��-1�ɂ���
				node[ai_node->mName.C_Str()].parent_index = -1;
			}

			//�C���f�b�N�X
			node[ai_node->mName.C_Str()].index = node.size() - 1;
			//�A�N�Z�X��(���ۂ͐e��������΂����̂ł��܂�g��Ȃ��̂Ŏ��M�̖��O�����Ă���)
			node[ai_node->mName.C_Str()].access_name = ai_node->mName.C_Str();
			//�m�[�h�̃��[�J���s��i�ʒu�E�g��E��]�j
			auto m = &ai_node->mTransformation;
			float* mat = node[ai_node->mName.C_Str()].mat;
			mat[0] = m->a1; mat[1] = m->b1; mat[2] = m->c1; mat[3] = m->d1;
			mat[4] = m->a2; mat[5] = m->b2; mat[6] = m->c2; mat[7] = m->d2;
			mat[8] = m->a3; mat[9] = m->b3; mat[10] = m->c3; mat[11] = m->d3;
			mat[12] = m->a4; mat[13] = m->b4; mat[14] = m->c4; mat[15] = m->d4;

			//���̃m�[�h�����b�V���i���f���j����ێ����Ă���ꍇ�́A�f�[�^���擾
			if (ai_node->mNumMeshes != 0)
			{
				//mNumMeshes����memory�m��
				node[ai_node->mName.C_Str()].mesh_id.resize(ai_node->mNumMeshes);
				//���b�V���i���f���j�ԍ���o�^
				for (int i = 0; i < ai_node->mNumMeshes; i++)
					node[ai_node->mName.C_Str()].mesh_id[i] = ai_node->mMeshes[i];
			}

			//�q�̃m�[�h�ֈړ�
			int num_child = ai_node->mNumChildren;
			for (int i = 0; i < num_child; i++)
			{
				aiNode* n = ai_node->mChildren[i];
				ParseNode(n, node);
			}


			return;
		}

		//���b�V���f�[�^
		void ParseMesh(const aiMesh* pSrcMesh, Mesh& mesh)
		{
			//���O
			mesh.name = pSrcMesh->mName.C_Str();

			//���̃��b�V�����g�p����}�e���A���ԍ�
			mesh.materialId = pSrcMesh->mMaterialIndex;

			//���_�f�[�^��memory�m��
			mesh.vertexs.resize(pSrcMesh->mNumVertices);

			aiVector3D zero3D(0.0f, 0.0f, 0.0f);
			for (size_t i = 0; i < pSrcMesh->mNumVertices; i++)
			{
				//�e�f�[�^���Q��
				auto p = &(pSrcMesh->mVertices[i]);	//���_
				auto n = &(pSrcMesh->mNormals[i]);		//�@��
				auto uv = (pSrcMesh->HasTextureCoords(0)) ? &(pSrcMesh->mTextureCoords[0][i]) : &zero3D;//UV
				auto t = (pSrcMesh->HasTangentsAndBitangents()) ? &(pSrcMesh->mTangents[i]) : &zero3D;//�ڐ�

				//���_�f�[�^�m��
				memcpy(&mesh.vertexs[i].pos, p, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].nol, n, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].uv, uv, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].tan, t, sizeof(float) * 3);
			}

			//�C���f�b�N�X
			mesh.index.resize(pSrcMesh->mNumFaces * 3);
			for (size_t i = 0; i < pSrcMesh->mNumFaces; i++)
			{
				const auto& face = pSrcMesh->mFaces[i];
				assert(face.mNumIndices == 3);//mNumIndives��3�ł��邱�ƒf���i����ȊO�ُ��m�点��j
				mesh.index[i * 3 + 0] = face.mIndices[0];
				mesh.index[i * 3 + 1] = face.mIndices[1];
				mesh.index[i * 3 + 2] = face.mIndices[2];

			}

			return;
		}

		//�}�e���A���f�[�^���
		void ParseMaterial(Material& material, const aiMaterial* pSrcMaterial)
		{
			aiColor3D c(0.0f, 0.0f, 0.0f);
			//�g�U����
			if (pSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, c) == AI_SUCCESS)
			{
				material.diffuse[0] = c.r;
				material.diffuse[1] = c.g;
				material.diffuse[2] = c.b;
			}
			else
			{
				material.diffuse[0] = 0.8f;
				material.diffuse[1] = 0.8f;
				material.diffuse[2] = 0.8f;
			}
			//���ʔ���
			if (pSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, c) == AI_SUCCESS)
			{
				material.speclar[0] = c.r;
				material.speclar[1] = c.g;
				material.speclar[2] = c.b;
			}
			else
			{
				material.speclar[0] = 0.0f;
				material.speclar[1] = 0.0f;
				material.speclar[2] = 0.0f;
			}

			//���ʔ��ˋ��x�i�V���C���j
			float shininess = 0.0f;
			if (pSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
				material.shininess = shininess;
			else
				material.shininess = 0.0f;

			//�A���r�G���g
			aiColor3D amb;
			if (pSrcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, amb) == AI_SUCCESS)
				material.ambient = (amb.r + amb.g + amb.b) / 3.0f;
			else
				material.ambient = 0.0f;

			//�e�N�X�`���t�@�C����
			aiString path;
			//�f�B�t���[�Y�}�b�v
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
				material.diffuseMap = string(path.C_Str());
			else
				material.diffuseMap.clear();
			//�X�y�L�����}�b�v
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS)
				material.specularMap = string(path.C_Str());
			else
				material.specularMap.clear();
			//�f�B�t���[�Y�}�b�v
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
				material.shininessMap = string(path.C_Str());
			else		
				material.shininessMap.clear();
			//�f�B�t���[�Y�}�b�v
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0), path) == AI_SUCCESS)
				material.normalMap = string(path.C_Str());
			else
				material.normalMap.clear();

			return;
		}

		//�e�N�X�`���f�[�^
		void ParseTextures(aiTexture* ai_tex, TexImage& tex)
		{
			//�e�N�X�`�����l��
			tex.name = ai_tex->mFilename.C_Str();

			//�l���e�N�X�`��
			vector<vector<array<float, 4>>> img;

			//����������FromStream���g����GDI�{�ŃC���[�W�f�[�^�����[�h
			IStream* pStream = SHCreateMemStream((BYTE*)ai_tex->pcData, ai_tex->mWidth);
			Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromStream(pStream, false);

			if (pBitmap->GetLastStatus() == Gdiplus::Ok)
			{
				tex.w = pBitmap->GetWidth();
				tex.h = pBitmap->GetHeight();

			}
			else
			{
				tex.w = 0;
				tex.h = 0;
				return;
			}

			img.resize(tex.h);
			for (int i = 0; i < tex.h; i++)
			{
				img[i].resize(tex.w);
			}

			Gdiplus::Color srcColor;
			for (int i = 0; i < tex.h; i++)
			{
				for (int j = 0; j < tex.w; j++)
				{
					//�O���t�B�b�N���f�[�^����1�s�N�Z�����`�悵�Ă���
					pBitmap->GetPixel(j, i, &srcColor);
					img[i][j][0] = srcColor.GetR();
					img[i][j][1] = srcColor.GetG();
					img[i][j][2] = srcColor.GetB();
					img[i][j][3] = srcColor.GetA();
				}
			}

			for (int y = 0; y < 512; y++)
			{
				for (int x = 0; x < 512; x++)
				{
					for (int c = 0; c < 4; c++)
					{
						//�g��O�摜�ɂ�������W�v�Z
						float original_x = (double)x / (512.0f / tex.w);
						float original_y = (double)y / (512.0f / tex.h);
						//����4�_�̍��W���v�Z
						int x0 = (int)original_x;	int x1 = x0 + 1;
						int y0 = (int)original_y;	int y1 = y0 + 1;
						//m1��n1���摜���͂ݏo���ꍇ�̏���
						if (x1 > 512)x1 = 512 - 1;
						if (y1 > 512)y1 = 512 - 1;
						//���Ƃ��Ƃ̍��W��m0,n0�Ƃ̋������v�Z
						int dx = original_x - x0;
						int dy = original_y - y0;
						//���͂S�_�̉�f�̐Fc�̋P�x�l�����`�ۊ�
						tex.const_img[y][x][c] =
							  img[y1][x1][c] * dy          * dx
							+ img[y1][x0][c] * dy          * (1.0f - dx)
							+ img[y0][x1][c] * (1.0f - dy) * dx
							+ img[y0][x0][c] * (1.0f - dy) * (1.0f - dx);
					}
				}
			}

			pStream->Release();
			delete pBitmap;
			return;
		}

		//�m�[�h�f�[�^
		map<string, Node>	node;//�m�[�h
		//���b�V���f�[�^�Q
		vector<Mesh>		meshs;//���b�V��

		//�}�e���A���f�[�^�Q
		vector<Material>	material;//�}�e���A��
		//�e�N�X�`���f�[�^�Q
		vector<TexImage>	tex;//�e�N�X�`���i�����j
	};

}