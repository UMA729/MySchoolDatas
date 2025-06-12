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

//ノード
class Node
{
	public:
	int32_t index;			//このノードのインデックス
	int32_t parent_index;	//このノードの親のインデックス
	string parent_name;		//このノードの親ノードの名前
	string access_name;		//このノードの接続先ノードの名前
	float mat[16];			//このノードがもつローカル行列（位置・拡大・回転）
	vector<int32_t>mesh_id;	//このノードが持つメッシュID
};

//バーテックス
class MeshVertex
{
public:
	float pos[3];//頂点
	float nol[3];//法線
	float uv[3]; //UV
	float tan[3];//接続
};

class Mesh
{
public:
	string			      name;	//メッシュ名
	vector<MeshVertex> vertexs;	//バーテックス
	vector<uint32_t> index; 	//インデックス
};

//ノードデータ
map<string, Node>node;//ノード
//メッシュデータ群
vector<Mesh>meshs;//メッシュ

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

//ノードデータ獲得関数
void ParseNode(aiNode* ai_node, map<string, Node>& node)
{
	if (ai_node == nullptr)return;

	//MAPにノード追加
	node.insert(make_pair(ai_node->mName.C_Str(), Node()));

	//接続元ノード名前の獲得（親ノード）
	if (ai_node->mParent != nullptr)
	{
		node[ai_node->mName.C_Str()].parent_name = ai_node->mParent->mName.C_Str();
		node[ai_node->mName.C_Str()].parent_index = node[ai_node->mParent->mName.C_Str()].index;
	}
	else
	{
		//ノードの大本なので接続先はない。parent_indexも-1にする
		node[ai_node->mName.C_Str()].parent_index = -1;
	}

	//インデックス
	node[ai_node->mName.C_Str()].index = node.size() - 1;
	//アクセス名(実際は親が分かればいいのであまり使わないので自信の名前を入れておく)
	node[ai_node->mName.C_Str()].access_name = ai_node->mName.C_Str();
	//ノードのローカル行列（位置・拡大・回転）
	auto m = &ai_node->mTransformation;
	float* mat = node[ai_node->mName.C_Str()].mat;
	mat[0]  = m->a1; mat[1]  = m->b1; mat[2]  = m->c1; mat[3]  = m->d1;
	mat[4]  = m->a2; mat[5]  = m->b2; mat[6]  = m->c2; mat[7]  = m->d2;
	mat[8]  = m->a3; mat[9]  = m->b3; mat[10] = m->c3; mat[11] = m->d3;
	mat[12] = m->a4; mat[13] = m->b4; mat[14] = m->c4; mat[15] = m->d4;

	//このノードがメッシュ（モデル）情報を保持している場合は、データを取得
	if (ai_node->mNumMeshes != 0)
	{
		//mNumMeshes分のmemory確保
		node[ai_node->mName.C_Str()].mesh_id.resize(ai_node->mNumMeshes);
		//メッシュ（モデル）番号を登録
		for (int i = 0; i < ai_node->mNumMeshes; i++)
			node[ai_node->mName.C_Str()].mesh_id[i] = ai_node->mMeshes[i];
	}
	
	//子のノードへ移動
	int num_child = ai_node->mNumChildren;
	for (int i = 0; i < num_child; i++)
	{
		aiNode* n = ai_node->mChildren[i];
		ParseNode(n, node);
	}

	//node[ai_node->mName.C_Str()]のデータをすべて出力
	//先生のやつ
	Node* w = &node[ai_node->mName.C_Str()];

	/*printf("\n---ノードデータ---\n");
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
		

	printf("[ローカル行列情報]\n");
	for (int i = 0; i < 16; i++)
	{
		printf("[%2d]=%.3f", i, w->mat[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}*/
	

	//自分の考えたやつ
	//printf("%d", node[ai_node->mName.C_Str()].index);
	//printf("%s", node[ai_node->mName.C_Str()].parent_name);


	return;
}

void ParseMesh(const aiMesh* pSrcMesh, Mesh& mesh)
{
	//名前
	mesh.name = pSrcMesh->mName.C_Str();

	//頂点データのmemory確保
	mesh.vertexs.resize(pSrcMesh->mNumVertices);
	
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < pSrcMesh->mNumVertices; i++)
	{
		//各データを参照
		auto p  = &(pSrcMesh->mVertices[i]);	//頂点
		auto n  = &(pSrcMesh->mNormals[i]);		//法線
		auto uv = (pSrcMesh->HasTextureCoords(0))?&(pSrcMesh->mTextureCoords[0][i])	:	&zero3D;//UV
		auto t  = (pSrcMesh->HasTangentsAndBitangents())?&(pSrcMesh->mTangents[i])	:	&zero3D;//接戦

		//頂点データ確保
		memcpy(&mesh.vertexs[i].pos, p  , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].nol, n  , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].uv , uv , sizeof(float) * 3);
		memcpy(&mesh.vertexs[i].tan, t  , sizeof(float) * 3);
	}

	//インデックス
	mesh.index.resize(pSrcMesh->mNumFaces * 3);
	for (size_t i = 0; i < pSrcMesh->mNumFaces; i++)
	{
		const auto& face = pSrcMesh->mFaces[i];
		assert(face.mNumIndices == 3);//mNumIndivesを3であること断言（それ以外異常を知らせる）
		mesh.index[i * 3 + 0] = face.mIndices[0];
		mesh.index[i * 3 + 1] = face.mIndices[1];
		mesh.index[i * 3 + 2] = face.mIndices[2];

	}
	
	//インデックス・バーテックスのデータ　前10個・後10個のデータを出力
	//自分のやつ
	printf("vertex\n_____________________\n");
	for (int i = 0; i < pSrcMesh->mNumVertices; i++)
	{
		if (i < 10)
		{
			if (i == 0)
				printf("\n1～10\n\n");
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

	//先生のやつ
	/*int size{ 0 };
	printf("---インデックス前");
	size = 10;
	for (int i = 0; i < size; i++)
		printf("[%d]", mesh.index[i]);

	printf("\n-- - インデックス後\n");
	size = mesh.index.size();
	for (int i = size - 10; i < size; i++)
		printf("[%d]", mesh.index[i]);

	printf("\n\n---バーテックス前\n");
	size = 10;
	VerticesView(0, size, mesh);
	printf("\n\n---バーテックス後\n");
	size = mesh.vertexs.size();
	VerticesView(size - 10, size, mesh);*/

	return;
}

int main()
{

	Assimp::Importer importer;	//Assimp元データ
	int flag{ 0 };				//Assimp解析フラグ


	flag |= aiProcess_Triangulate;				//すべて三角形k
	flag |= aiProcess_CalcTangentSpace;			//接触ベクトルと従接戦ベクトルを計算
	flag |= aiProcess_GenSmoothNormals;			//法線がない場合、法線を作成
	flag |= aiProcess_GenUVCoords;				//UV有り
	flag |= aiProcess_RemoveRedundantMaterials;	//使用していない材質は削除
	flag |= aiProcess_OptimizeMeshes;			//シーン階層の最適化
	flag |= aiProcess_FlipUVs;					//Y軸反転
	flag |= aiProcess_JoinIdenticalVertices;	//モデルの頂点は同一として扱う

	//Assimpでの読み込み
	string str{ "Jump.fbx" };
	const aiScene* pScene = importer.ReadFile(str.c_str(), flag);

	//ノード解析
	ParseNode(pScene->mRootNode, node);

	//メッシュ解析
	meshs.resize(pScene->mNumMeshes);
	for (size_t i = 0; i < meshs.size(); i++)
	{
		ParseMesh(pScene->mMeshes[i],meshs[i]);


	}

	return 0;
}