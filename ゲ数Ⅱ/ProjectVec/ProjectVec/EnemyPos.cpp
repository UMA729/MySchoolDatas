#include <stdio.h>
#include <math.h>

#define M_PI 180

// 2�_�Ԃ̋������v�Z����֐�
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// ��l���̎p�������Ɋ�Â��ă��[�U�[�̐i�s�������v�Z����֐�
void laser_direction(double player_x, double player_y, double angle, double laser_length, double* laser_x, double* laser_y) {
    *laser_x = player_x + laser_length * cos(angle);
    *laser_y = player_y + laser_length * sin(angle);
}

// ���[�U�[�ƓG�̐i�s�����Ƃ̊p�x�����v�Z����֐�
double angle_between(double player_x, double player_y, double enemy_x, double enemy_y, double laser_angle) {
    // ��l���ƓG�̈ʒu����p�x���v�Z
    double dx = enemy_x - player_x;
    double dy = enemy_y - player_y;
    double enemy_angle = atan2(dy, dx); // �G�̕����̊p�x

    // �p�x�����v�Z
    double angle_diff = fabs(enemy_angle - laser_angle);

    // �p�x��180�x�𒴂���ꍇ�́A�␳���s��
    if (angle_diff > M_PI) {
        angle_diff = 2 * M_PI - angle_diff;
    }

    return angle_diff;
}

// �G1�ƓG2�̈ʒu�ɑ΂��Ăǂ��炪���[�U�[�ɋ߂����𔻒肷��֐�
void classify_enemy(double player_x, double player_y, double angle, double laser_length,
    double enemy1_x, double enemy1_y, double enemy2_x, double enemy2_y) {
    // ��l���̎p�������Ɋ�Â��ă��[�U�[�̐i�s�������v�Z
    double laser_x, laser_y;
    laser_direction(player_x, player_y, angle, laser_length, &laser_x, &laser_y);

    // �G1�ƓG2�̐i�s�����Ƃ̊p�x�����v�Z
    double angle_diff1 = angle_between(player_x, player_y, enemy1_x, enemy1_y, angle);
    double angle_diff2 = angle_between(player_x, player_y, enemy2_x, enemy2_y, angle);

    // �p�x�����������������[�U�[�ɋ߂�
    if (angle_diff1 < angle_diff2) {
        printf("�G1�����[�U�[�ɋ߂��A�G2������\n");
    }
    else if (angle_diff1 == angle_diff2)
    {
        printf("�G2�ƓG1�͓����ʒu\n");
    }
    else {
        printf("�G2�����[�U�[�ɋ߂��A�G1������\n");
    }
}

int main() {
    // ��l���̈ʒu�A�p�x�A���[�U�[�̒���
    double player_x = 0.0, player_y = 0.0;
    double angle = M_PI / 2.0;  // 45�x�����W�A���ɕϊ�
    double laser_length = 100.0;

    // �G1�ƓG2�̈ʒu
    double enemy1_x = 20.0, enemy1_y = 40.0;
    double enemy2_x = 40.0, enemy2_y = 10.0;

    // �G�̈ʒu�Ɋ�Â��āA�ǂ��炪���[�U�[�ɋ߂����𔻒�
    classify_enemy(player_x, player_y, angle, laser_length, enemy1_x, enemy1_y, enemy2_x, enemy2_y);

    return 0;
}
