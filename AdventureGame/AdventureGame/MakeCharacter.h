#pragma once

struct Character
{
	int x;//x���W
	int y;//y���W
	int energy;//�̗�
	int maxenergy;//�ő�̗�
	int attack;//�U����
	
};

void SetRandomLocation(int* _px, int* _py);//�����_���ȍ��W���擾
int GetSpace(int x, int y);//���݂����Ԃ̃C���f�b�N�X���擾
void Playermove();//��l���̓������Ǘ�
void Enemymove();//�G�̓������Ǘ�
void player_init_parameter();
void enemy_init_parameter();