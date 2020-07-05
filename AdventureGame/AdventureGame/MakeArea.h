#pragma once


//それぞれの空間の構造体
struct Space {
	int x;
	int y;
	int width;
	int height;
};

struct Land
{
	int x_location;//現在のエリアのx座標
	int y_location;//現在のエリアのy座標
	int width;//エリアの幅
	int height;//エリアの高さ
	Space space;//それぞれのエリアにある空間
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

char sellArt[][3] = {//全角は配列のサイズ３つ分
	"・",//CELL_TYPE_NONEのことで特別なものではない
	"■",//CELL_TYPE_WALLのことで壁を意味する
	"＠",//CELL_TYPE_PLAYERのことでプレイヤーを意味する
	"宝",//CELL_TYPE_TREASUREのことで宝を意味する
	"％",//CELL_TYPE_STAIRのことで階段を意味する
    "〇"//CELL_TYPE_ENEMYのことで敵を意味する

};


void MakeField();//エリアの生成 空間の生成
void SpritLand(int landNum);//エリア同士の切り離し
void display();
