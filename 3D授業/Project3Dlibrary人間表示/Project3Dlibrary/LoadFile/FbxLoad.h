#pragma once

#define NOMINMAX
#include<windows.h>
#include<gdiplus.h>		//GDI+を使用
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

#pragma comment (lib,"gdiplus.lib")//GDI+のlib読み込み
#pragma comment (lib,"shlwapi.lib")//SHCreateMemStream使用

using namespace std;


namespace FBX_FILE
{
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

	//マテリアル
	class Material
	{
	public:
		float diffuse[3];	   //デフィーズ(拡散反射)
		float speclar[3];	   //スペキュラ（鏡面反射）
		float ambient;		   //アンビエント（環境光（疑似））
		float shininess;	   //シャイン
		string diffuseMap;	   //デフィーズマップ名　今回はこれ
		string specularMap;	   //スペキュラマップ名
		string shininessMap;   //シャインマップ名
		string normalMap;      //ノーマルマップ名
	};

	//テクスチャ
	class TexImage
	{
	public:
		string name;//テクスチャネーム
		int32_t w, h;//テクスチャの本来の大きさ
		float const_img[512][512][4];//縮小した512×512テクスチャ情報
	};
	//メッシュ
	class Mesh
	{
	public:
		string					name;		//メッシュ名
		vector<MeshVertex>		vertexs;	//バーテックス
		vector<uint32_t>		index; 		//インデックス
		int32_t					materialId;	//このメッシュのマテリアルID
	};

	//FBXファイルロード
	class FbxFile
	{
	public:
		FbxFile()
		{
			//GDI＋の初期設定
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
				return;


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
			string str{ "3DModelResource\\Jump.fbx" };
			const aiScene* pScene = importer.ReadFile(str.c_str(), flag);

			//ノード解析
			ParseNode(pScene->mRootNode, node);

			//メッシュ解析
			meshs.resize(pScene->mNumMeshes);
			for (size_t i = 0; i < meshs.size(); i++)
			{
				ParseMesh(pScene->mMeshes[i], meshs[i]);


			}

			//マテリアルデータ
			material.resize(pScene->mNumMaterials);
			for (size_t i = 0; i < pScene->mNumMaterials; i++)
				ParseMaterial(material[i], pScene->mMaterials[i]);

			//テクスチャデータ
			tex.resize(pScene->mNumTextures);
			for (size_t i = 0; i < pScene->mNumTextures; i++)
				ParseTextures(pScene->mTextures[i], tex[i]);

			Gdiplus::GdiplusShutdown(gdiplusToken);

			return ;
		}
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
			mat[0] = m->a1; mat[1] = m->b1; mat[2] = m->c1; mat[3] = m->d1;
			mat[4] = m->a2; mat[5] = m->b2; mat[6] = m->c2; mat[7] = m->d2;
			mat[8] = m->a3; mat[9] = m->b3; mat[10] = m->c3; mat[11] = m->d3;
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


			return;
		}

		//メッシュデータ
		void ParseMesh(const aiMesh* pSrcMesh, Mesh& mesh)
		{
			//名前
			mesh.name = pSrcMesh->mName.C_Str();

			//このメッシュが使用するマテリアル番号
			mesh.materialId = pSrcMesh->mMaterialIndex;

			//頂点データのmemory確保
			mesh.vertexs.resize(pSrcMesh->mNumVertices);

			aiVector3D zero3D(0.0f, 0.0f, 0.0f);
			for (size_t i = 0; i < pSrcMesh->mNumVertices; i++)
			{
				//各データを参照
				auto p = &(pSrcMesh->mVertices[i]);	//頂点
				auto n = &(pSrcMesh->mNormals[i]);		//法線
				auto uv = (pSrcMesh->HasTextureCoords(0)) ? &(pSrcMesh->mTextureCoords[0][i]) : &zero3D;//UV
				auto t = (pSrcMesh->HasTangentsAndBitangents()) ? &(pSrcMesh->mTangents[i]) : &zero3D;//接戦

				//頂点データ確保
				memcpy(&mesh.vertexs[i].pos, p, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].nol, n, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].uv, uv, sizeof(float) * 3);
				memcpy(&mesh.vertexs[i].tan, t, sizeof(float) * 3);
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

			return;
		}

		//マテリアルデータ解析
		void ParseMaterial(Material& material, const aiMaterial* pSrcMaterial)
		{
			aiColor3D c(0.0f, 0.0f, 0.0f);
			//拡散反射
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
			//鏡面反射
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

			//鏡面反射強度（シャイン）
			float shininess = 0.0f;
			if (pSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
				material.shininess = shininess;
			else
				material.shininess = 0.0f;

			//アンビエント
			aiColor3D amb;
			if (pSrcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, amb) == AI_SUCCESS)
				material.ambient = (amb.r + amb.g + amb.b) / 3.0f;
			else
				material.ambient = 0.0f;

			//テクスチャファイル名
			aiString path;
			//ディフューズマップ
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
				material.diffuseMap = string(path.C_Str());
			else
				material.diffuseMap.clear();
			//スペキュラマップ
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS)
				material.specularMap = string(path.C_Str());
			else
				material.specularMap.clear();
			//ディフューズマップ
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
				material.shininessMap = string(path.C_Str());
			else		
				material.shininessMap.clear();
			//ディフューズマップ
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0), path) == AI_SUCCESS)
				material.normalMap = string(path.C_Str());
			else
				material.normalMap.clear();

			return;
		}

		//テクスチャデータ
		void ParseTextures(aiTexture* ai_tex, TexImage& tex)
		{
			//テクスチャ名獲得
			tex.name = ai_tex->mFilename.C_Str();

			//獲得テクスチャ
			vector<vector<array<float, 4>>> img;

			//メモリからFromStreamを使ってGDI＋でイメージデータをロード
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
					//グラフィックをデータから1ピクセルずつ描画していく
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
						//拡大前画像における座標計算
						float original_x = (double)x / (512.0f / tex.w);
						float original_y = (double)y / (512.0f / tex.h);
						//周囲4点の座標を計算
						int x0 = (int)original_x;	int x1 = x0 + 1;
						int y0 = (int)original_y;	int y1 = y0 + 1;
						//m1やn1が画像をはみ出た場合の処理
						if (x1 > 512)x1 = 512 - 1;
						if (y1 > 512)y1 = 512 - 1;
						//もともとの座標とm0,n0との距離を計算
						int dx = original_x - x0;
						int dy = original_y - y0;
						//周囲４点の画素の色cの輝度値より線形保管
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

		//ノードデータ
		map<string, Node>	node;//ノード
		//メッシュデータ群
		vector<Mesh>		meshs;//メッシュ

		//マテリアルデータ群
		vector<Material>	material;//マテリアル
		//テクスチャデータ群
		vector<TexImage>	tex;//テクスチャ（内部）
	};

}