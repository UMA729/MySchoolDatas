#include "wall.h"
#include "function.h"


CWall::CWall(int randnum, int count)
{
	wall_max_y = randnum;		//•Ç‚ª‹ó‚¢‚Ä‚éˆÊ’u
	wall_spa = 100;			//ŒŠ‚Ì‚¨‚¨‚«‚³
	wall_wid = 80;				//•Ç“¯m‚ÌŠÔŠu
	center_x = WINDOW_WIDTH / 2;//‰æ–Ê^‚ñ’†
}
int x[5];

int CWall::Action(list<unique_ptr<Base>>& base)
{
	total_width = (CCount + 1) * 30 + CCount * wall_wid;

	return 0;
}

void CWall::Draw()
{
	DrawBox(center_x - 10, 0, center_x + 10, wall_max_y, GetColor(255, 255, 255), true);
	if (wall_max_y + wall_spa < WINDOW_HEIGHT)
	{
		DrawBox(center_x - 10, wall_max_y+wall_spa, center_x + 10, WINDOW_HEIGHT, GetColor(255, 255, 255), true);
	}
}