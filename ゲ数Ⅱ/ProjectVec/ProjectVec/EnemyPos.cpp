#include <stdio.h>
#include <math.h>

#define M_PI 180

// 2点間の距離を計算する関数
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// 主人公の姿勢方向に基づいてレーザーの進行方向を計算する関数
void laser_direction(double player_x, double player_y, double angle, double laser_length, double* laser_x, double* laser_y) {
    *laser_x = player_x + laser_length * cos(angle);
    *laser_y = player_y + laser_length * sin(angle);
}

// レーザーと敵の進行方向との角度差を計算する関数
double angle_between(double player_x, double player_y, double enemy_x, double enemy_y, double laser_angle) {
    // 主人公と敵の位置から角度を計算
    double dx = enemy_x - player_x;
    double dy = enemy_y - player_y;
    double enemy_angle = atan2(dy, dx); // 敵の方向の角度

    // 角度差を計算
    double angle_diff = fabs(enemy_angle - laser_angle);

    // 角度が180度を超える場合は、補正を行う
    if (angle_diff > M_PI) {
        angle_diff = 2 * M_PI - angle_diff;
    }

    return angle_diff;
}

// 敵1と敵2の位置に対してどちらがレーザーに近いかを判定する関数
void classify_enemy(double player_x, double player_y, double angle, double laser_length,
    double enemy1_x, double enemy1_y, double enemy2_x, double enemy2_y) {
    // 主人公の姿勢方向に基づいてレーザーの進行方向を計算
    double laser_x, laser_y;
    laser_direction(player_x, player_y, angle, laser_length, &laser_x, &laser_y);

    // 敵1と敵2の進行方向との角度差を計算
    double angle_diff1 = angle_between(player_x, player_y, enemy1_x, enemy1_y, angle);
    double angle_diff2 = angle_between(player_x, player_y, enemy2_x, enemy2_y, angle);

    // 角度差が小さい方がレーザーに近い
    if (angle_diff1 < angle_diff2) {
        printf("敵1がレーザーに近い、敵2が遠い\n");
    }
    else if (angle_diff1 == angle_diff2)
    {
        printf("敵2と敵1は同じ位置\n");
    }
    else {
        printf("敵2がレーザーに近い、敵1が遠い\n");
    }
}

int main() {
    // 主人公の位置、角度、レーザーの長さ
    double player_x = 0.0, player_y = 0.0;
    double angle = M_PI / 2.0;  // 45度をラジアンに変換
    double laser_length = 100.0;

    // 敵1と敵2の位置
    double enemy1_x = 20.0, enemy1_y = 40.0;
    double enemy2_x = 40.0, enemy2_y = 10.0;

    // 敵の位置に基づいて、どちらがレーザーに近いかを判定
    classify_enemy(player_x, player_y, angle, laser_length, enemy1_x, enemy1_y, enemy2_x, enemy2_y);

    return 0;
}
