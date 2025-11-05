#include "map.h"
#include "block.h"
#include "player.h"
#include "function.h" 

//文字分割関数
vector<string> split(string& input, char delimiter) {
	istringstream stream(input);
	string field;
	vector<string>result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

CMap::CMap() {
	img = LoadGraph("image\\maptip.png");
}

void CMap::LoadMap() {
	ifstream fp("image\\map.txt");
	string str;

	if (!fp.fail()) {
		for (int y = 0; y < 11; y++) {
				getline(fp, str);
				vector<string>strv = split(str, ',');//読み込んだ行を','区切りで分割
			for (int x = 0; x < 160; x++){
				map[y][x] = stoi(strv.at(x));
			}
		}
		fp.close();
	}
}

void CMap::Map_ObjCreation(vector<unique_ptr<BaseVector>>& base) {
	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 160; x++) {
			Point p{ x * 16,y * 16 };
			base.emplace_back((unique_ptr<BaseVector>)new CBlock(p, map[y][x], img));
		}
	}
}

void CMap::Action(vector<unique_ptr<BaseVector>>& base)
{
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	//カメラ座標をプレイヤー座標へ
	camera = p->m_pos;

	//カメラの領域判定
	if (camera.x < WINDOW_WIDTH / 2)camera.x = WINDOW_WIDTH / 2;//左
	if (camera.x > 16 * 160 - WINDOW_WIDTH / 2)camera.x = 16 * 160 - WINDOW_WIDTH / 2;
	//32．．．マップチップの横サイズ 30．．．マップの横ブロック数

	//現在のマップチップオブジェクトを削除
	Del_ListObj(base, BLOCK);

	//マップ上のカメラ位置
	Point m_camera{ WINDOW_WIDTH / 2 - camera.x,WINDOW_HEIGHT / 2 - camera.y };
	//描画するマップ位置（配列）
	int ChipX = -(m_camera.x / 16);//32マップチップ横サイズ
	int ChipY = -(m_camera.y / 16);//32マップチップ縦サイズ
	//描画開始位置
	Point DrawPos{ m_camera.x + ChipX * 16,m_camera.y + ChipY * 16 };//32マップチップサイズ

	//マップチップオブジェクト生成
	Point chipPos{ DrawPos.x,DrawPos.y };
	for (int y = 0; y < 10+1; y++) {
		chipPos.x = DrawPos.x;
		for (int x = 0; x < 10+1; x++) {
			base.emplace_back((unique_ptr<BaseVector>)new CBlock(chipPos, map[y + ChipY][x + ChipX], img));
			chipPos.x += 16;
		}
		chipPos.y += 16;
	}

	//プレイヤーの描画位置更新
	p->pos.y = p->m_pos.y - camera.y + WINDOW_WIDTH	 / 2;
	p->pos.x = p->m_pos.x - camera.x + WINDOW_HEIGHT / 2;
}