//�Q�[���V�[��
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include "goal.h"
#include "carry_thing.h"
#include "spring_circle.h"
#include "spring_circle2.h"
#include "carry_spring.h"

int GSCORE{ 0 };

//�R���X�g���N�^
CGame::CGame(CManager* p) :CScene(p){
	//Point pos{ Range_Random_NumberF(32, WINDOW_WIDTH - 32),400};
	//base.emplace_back((unique_ptr<Base>)new CEnemy(pos.x, pos.y));

	//base.emplace_back((unique_ptr<Base>)new CSpring_Circle());

	base.emplace_back((unique_ptr<Base>)new CCarry_Spring());
	base.emplace_back((unique_ptr<Base>)new CCarry_thing());
}

//�X�V����
int CGame::Update(){



	//list�I�u�W�F�N�g�̍X�V����
	for (auto i = base.begin(); i != base.end(); i++)
		(*i)->Action(base);

	//���X�g�̃T�C�Y�ۑ�
	temp_list_Length = (int)base.size();

	//���X�g����̃I�u�W�F�N�g�폜����
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	return 0;
}

//�`�揈��
void CGame::Draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "List_Size = %d", base.size());
	DrawFormatString(0, 16, GetColor(255, 255, 255), "Score = %d", GSCORE);

	//list�I�u�W�F�N�g�̕`��
	for (auto i = base.begin(); i != base.end(); i++)
		if ((*i)->FLAG) (*i)->Draw();

	//3D���̕`��
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 0, 100),
	//	0x0000ff
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(100, 0, 0),
	//	0xff0000
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 100, 0),
	//	0x00ff00
	//);
}

CGame::~CGame()
{

}
