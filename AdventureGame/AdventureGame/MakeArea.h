#pragma once


//���ꂼ��̋�Ԃ̍\����
struct Space {
	int x;
	int y;
	int width;
	int height;
};

struct Land
{
	int x_location;//���݂̃G���A��x���W
	int y_location;//���݂̃G���A��y���W
	int width;//�G���A�̕�
	int height;//�G���A�̍���
	Space space;//���ꂼ��̃G���A�ɂ�����
};



enum 
{
	CELL_TYPE_NONE,
	CELL_TYPE_WALL,
	CELL_TYPE_PLAYER,
	CELL_TYPE_TREASURE,
	CELL_TYPE_STAIR,
	CELL_TYPE_ENEMY
};

char sellArt[][3] = {//�S�p�͔z��̃T�C�Y�R��
	"�E",//CELL_TYPE_NONE�̂��Ƃœ��ʂȂ��̂ł͂Ȃ�
	"��",//CELL_TYPE_WALL�̂��Ƃŕǂ��Ӗ�����
	"��",//CELL_TYPE_PLAYER�̂��ƂŃv���C���[���Ӗ�����
	"��",//CELL_TYPE_TREASURE�̂��Ƃŕ���Ӗ�����
	"��",//CELL_TYPE_STAIR�̂��ƂŊK�i���Ӗ�����
    "�Z"//CELL_TYPE_ENEMY�̂��ƂœG���Ӗ�����

};


void MakeField();//�G���A�̐��� ��Ԃ̐���
void SpritLand(int landNum);//�G���A���m�̐؂藣��
void display();
