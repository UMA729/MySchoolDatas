#include <stdio.h>
#include <vector>
#include <array>
#include <map>
#include<codecvt>
#include<cassert>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

using namespace std;

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

class Mesh
{
public:
	string			      name;	//���b�V����
	vector<MeshVertex> vertexs;	//�o�[�e�b�N�X
	vector<uint32_t> index; 	//�C���f�b�N�X
};

//�m�[�h�f�[�^
map<string, Node>node;//�m�[�h
//���b�V���f�[�^�Q
vector<Mesh>meshs;//���b�V��

//void VerticesView(int s, int e, Mesh& mesh)
//{
//	for (int i = s; i < e; i++)
//	{
//		float* pos = mesh.vertexs[i].pos;
//		float* nol = mesh.vertexs[i].nol;
//		float* uv = mesh.vertexs[i].uv;
//		float* tan = mesh.vertexs[i].tan;
//		printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, pos[0],pos[1],pos[2]);
//		printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, nol[0],nol[1],nol[2]);
//		printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, uv[0],uv[1],uv[2]);
//		printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, tan[0],tan[1],tan[2]);
//		printf("\n");
//	}
//}

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
	mat[0]  = m->a1; mat[1]  = m->b1; mat[2]  = m->c1; mat[3]  = m->d1;
	mat[4]  = m->a2; mat[5]  = m->b2; mat[6]  = m->c2; mat[7]  = m->d2;
	mat[8]  = m->a3; mat[9]  = m->b3; mat[10] = m->c3; mat[11] = m->d3;
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

	//node[ai_node->mName.C_Str()]�̃f�[�^�����ׂďo��
	//�搶�̂��
	Node* w = &node[ai_node->mName.C_Str()];

	/*printf("\n---�m�[�h�f�[�^---\n");
	printf("index=%d\n", w->index);
	printf("name[%s]\n", w->access_name.c_str());
	printf("parent_index=%d\n", w->parent_index);
	printf("access_name[%s]\n", w->parent_name.c_str());
	
	printf("mesh_id=");
	for (int i = 0; i < w->mesh_id.size(); i++)
	{
		printf("[%d]", w->mesh_id[i]);
	}
	printf("\n");
		

	printf("[���[�J���s����]\n");
	for (int i = 0; i < 16; i++)
	{
		printf("[%2d]=%.3f", i, w->mat[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}*/
	

	//�����̍l�������
	//printf("%d", node[ai_node->mName.C_Str()].index);
	//printf("%s", node[ai_node->mName.C_Str()].parent_name);


	return;
}

void ParseMesh(const aiMesh* pSrcMesh, Mesh& mesh)
{
	//���O
	mesh.name = pSrcMesh->mName.C_Str();

	//���_�f�[�^��memory�m��
	mesh.vertexs.resize(pSrcMesh->mNumVertices);
	
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < pSrcMesh->mNumVertices; i++)
	{
		//�e�f�[�^���Q��
		auto p  = &(pSrcMesh->mVertices[i]);	//���_
		auto n  = &(pSrcMesh->mNormals[i]);		//�@��
		auto uv = (pSrcMesh->HasTextureCoords(0))?&(pSrcMesh->mTextureCoords[0][i])	:	&zero3D;//UV
		auto t  = (pSrcMesh->HasTangentsAndBitangents())?&(pSrcMesh->mTangents[i])	:	&zero3D;//�ڐ�

		//���_�f�[�^�m��
		memcpy(&mesh.vertexs[i].pos, p  , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].nol, n  , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].uv , uv , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].tan, t  , sizeof(float) * 3);
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
	
	//�C���f�b�N�X�E�o�[�e�b�N�X�̃f�[�^�@�O10�E��10�̃f�[�^���o��
	//�����̂��
	printf("vertex\n_____________________\n");
	for (int i = 0; i < pSrcMesh->mNumVertices; i++)
	{
		if (i < 10)
		{
			if (i == 0)
				printf("\n1�`10\n\n");
			for (int j = 0; j < 1; j++)
			{
				
				printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i+1,mesh.vertexs[i].pos[0],mesh.vertexs[i].pos[1],mesh.vertexs[i].pos[2]);
				printf("Nol[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i+1,mesh.vertexs[i].nol[0],mesh.vertexs[i].nol[1],mesh.vertexs[i].nol[2]);
				printf("UV[%d](x=[%.3f]y=[%.3f])\n", i+1,mesh.vertexs[i].uv[0],mesh.vertexs[i].uv[1]);
				printf("Tan[%d](x=[%.3f]y=[%.3f])\n\n", i+1,mesh.vertexs[i].tan[0],mesh.vertexs[i].tan[1]);
			}
		}
		if (i == pSrcMesh->mNumVertices - 10)
			printf("\nLAST10\n\n");
		if (i > pSrcMesh->mNumVertices - 10)
		{
			printf("Pos[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, mesh.vertexs[i].pos[0], mesh.vertexs[i].pos[1], mesh.vertexs[i].pos[2]);
			printf("Nol[%d](x=[%.3f]y=[%.3f]z=[%.3f])\n", i + 1, mesh.vertexs[i].nol[0], mesh.vertexs[i].nol[1], mesh.vertexs[i].nol[2]);
			printf("UV[%d](x=[%.3f]y=[%.3f])\n", i + 1, mesh.vertexs[i].uv[0], mesh.vertexs[i].uv[1]);
			printf("Tan[%d](x=[%.3f]y=[%.3f])\n\n", i + 1, mesh.vertexs[i].tan[0], mesh.vertexs[i].tan[1]);
		}
	}
	printf("index\n_________________________\n");
	for (size_t i = 0; i < pSrcMesh->mNumFaces * 3; i++)
	{
		if (i < 10)
		{
			printf("%d\n", mesh.index[i]);
		}
		if (i > (pSrcMesh->mNumFaces * 3) - 10)
		{
			printf("%d\n", mesh.index[i]);
		}
	}

	//�搶�̂��
	/*int size{ 0 };
	printf("---�C���f�b�N�X�O");
	size = 10;
	for (int i = 0; i < size; i++)
		printf("[%d]", mesh.index[i]);

	printf("\n-- - �C���f�b�N�X��\n");
	size = mesh.index.size();
	for (int i = size - 10; i < size; i++)
		printf("[%d]", mesh.index[i]);

	printf("\n\n---�o�[�e�b�N�X�O\n");
	size = 10;
	VerticesView(0, size, mesh);
	printf("\n\n---�o�[�e�b�N�X��\n");
	size = mesh.vertexs.size();
	VerticesView(size - 10, size, mesh);*/

	return;
}

int main()
{

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
	string str{ "Jump.fbx" };
	const aiScene* pScene = importer.ReadFile(str.c_str(), flag);

	//�m�[�h���
	ParseNode(pScene->mRootNode, node);

	//���b�V�����
	meshs.resize(pScene->mNumMeshes);
	for (size_t i = 0; i < meshs.size(); i++)
	{
		ParseMesh(pScene->mMeshes[i],meshs[i]);


	}

	return 0;
}