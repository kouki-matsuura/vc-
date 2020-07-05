#pragma once

struct Character
{
	int x;//x座標
	int y;//y座標
	int energy;//体力
	int maxenergy;//最大体力
	int attack;//攻撃力
	
};

void SetRandomLocation(int* _px, int* _py);//ランダムな座標を取得
int GetSpace(int x, int y);//現在いる空間のインデックスを取得
void Playermove();//主人公の動きを管理
void Enemymove();//敵の動きを管理
void player_init_parameter();
void enemy_init_parameter();