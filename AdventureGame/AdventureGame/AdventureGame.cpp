#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <string.h>

#include "Setting.h"
#include "MakeArea.h"
#include "MakeCharacter.h";

#define maxSize 64//エリア全体のサイズ
#define F_width 32//エリア全体の幅
#define F_height 32//エリア全体の高さ
#define Min_Landsize 16//空間のサイズ
#define Look_Radius 8//主人公が見える範囲　半径8
#define CHARBUFF 124

Land lands[maxSize];
Character player;
Character enemy;

int count;//生成済みのエリアを数える
int space[F_height][F_width];//空間
int field_num = 1;//現在の階数
int turn;//主人公のターンを数える変数
bool game_Flag = true;//ゲームを動かすフラグ

char currentDirectory[CHARBUFF];
char keyword_energy[CHARBUFF];
char keyword_attack[CHARBUFF];
char settingFile[CHARBUFF];
char w_filename[CHARBUFF];
errno_t error;
FILE* fp;

//メイン関数--------------------------------------------------------------------------------------------------------------------------
//====================================================================================================================================
int main()
{
    //主人公の初期値設定
    player_init_parameter();

    


    sprintf_s(w_filename, "result_file.txt");
    error = fopen_s(&fp, w_filename, "w");

    if (error != 0) { fprintf_s(stderr, "failed to open"); }
    else {
        fprintf(fp, "ターン数  体力  行動 \n");

        srand((unsigned int)time(NULL));//乱数の生成





        MakeField();//エリアの生成

        while (game_Flag)
        {
            display(); //画面描画
             //-----------------------------------------------------------------------------------------------------
            Playermove();//プレイヤーの座標を更新
             //-------------------------------------------------------------------------------------------------------
            Enemymove();//敵の座標を更新

            fprintf(fp, "\n");
        }
    }
}
//===================================================================================================================================
//-----------------------------------------------------------------------------------------------------------------------------------



//プレイヤーの情報を初期化する------------------------------------------------------------------------------------------------------
void player_init_parameter() {

    
    getGurrentDirectory(currentDirectory);

    char hero[CHARBUFF];
    

    sprintf_s(hero, "player");
    

    char info_hero[CHARBUFF];
   

    

    sprintf_s(keyword_energy, "maxenergy");
    sprintf_s(keyword_attack, "attack");

    
    sprintf_s(settingFile, "%s\\parameter.ini", currentDirectory);

    readChar(hero, keyword_energy, "none", info_hero, settingFile);
    player.maxenergy = atoi(info_hero);
    readChar(hero, keyword_attack, "none", info_hero, settingFile);
    player.attack = atoi(info_hero);

   

    player.energy = player.maxenergy;

    
    
    
}
//-----------------------------------------------------------------------------------------------------------------------------------


//敵の情報を初期化する---------------------------------------------------------------------------------------------------------------
void enemy_init_parameter() {

    getGurrentDirectory(currentDirectory);
    char Enemy[CHARBUFF];


    sprintf_s(Enemy, "enemy");


    char info_enemy[CHARBUFF];

    sprintf_s(keyword_energy, "maxenergy");
    sprintf_s(keyword_attack, "attack");

    sprintf_s(settingFile, "%s\\parameter.ini", currentDirectory);

    readChar(Enemy, keyword_energy, "none", info_enemy, settingFile);
    enemy.maxenergy = atoi(info_enemy);
    readChar(Enemy, keyword_attack, "none", info_enemy, settingFile);
    enemy.attack = atoi(info_enemy);

    enemy.attack += field_num;
    enemy.maxenergy += field_num;
    enemy.energy = enemy.maxenergy;


}
//----------------------------------------------------------------------------------------------------------------------------------



//敵の動きを管理-------------------------------------------------------------------------------------------------------------------
void Enemymove() {

        //体力が残っていれば、行動
        if (enemy.energy > 0)
        {
            int field_location = GetSpace(enemy.x, enemy.y);
            int distance = abs(player.x - enemy.x) + abs(player.y - enemy.y);
            //主人公との距離が1だったときは攻撃する
            if (distance == 1) {
                display();
                printf("敵の攻撃\n");
                _getch();
                int attack = enemy.attack / 2 + rand() % enemy.attack;
                player.energy -= attack;
                printf("主人公に%dのダメージ", attack);
                _getch();
                fprintf(fp, "敵の攻撃で主人公に%dのダメージ", attack);
                if (player.energy < 0) {
                    player.energy = 0;
                    display();
                    printf("ゲームオーバー\a");
                    fprintf(fp, "ゲームオーバー");
                    _getch();
                    game_Flag = false;
                }

            }
            else if (((field_location >= 0) && (field_location == GetSpace(player.x, player.y))) || (distance == 5))
                //敵がどこかの空間内に存在し、同じ空間内にいるとき、または、距離が5マスだったとき、主人公を追いかける
            {

                int e_x = enemy.x;
                int e_y = enemy.y;
                //斜め方向移動を禁止
                if (e_x < player.x) e_x++;
                else if (e_x > player.x) e_x--;
                else if (e_y < player.y) e_y++;
                else if (e_y > player.y) e_y--;

                //主人公と重なっているときは何もしない
                if ((e_x == player.x) && (e_y == player.y)) {

                }
                else if (space[e_y][e_x] != CELL_TYPE_WALL) {
                    //ブロックではないマスだったら座標を更新する
                    enemy.x = e_x;
                    enemy.y = e_y;
                }
            }
        }
        else {
            //体力が０になれば、ランダムでまた敵を生成
            SetRandomLocation(&enemy.x, &enemy.y);
            //敵の初期値設定
            enemy_init_parameter();
        }

}
//---------------------------------------------------------------------------------------------------------------------------


//主人公の動きを管理--------------------------------------------------------------------------------------------------------
void Playermove() {

    int x_coordinate = player.x;
    int y_coordinate = player.y;

    //主人公の移動-----------------------------------------------------------------------------------------
    switch (_getch()) {
    case 'w':y_coordinate--; break;
    case 'a':x_coordinate--; break;
    case 's':y_coordinate++; break;
    case 'd':x_coordinate++; break;
    case 'c': game_Flag = false; break;
    }
    //-----------------------------------------------------------------------------------------------------

    fprintf(fp, "   %d     %d    ", turn,player.energy);

    //移動するマスによって動作を変える---------------------------------------------------------------------
    if ((enemy.energy > 0) && (x_coordinate == enemy.x) && (y_coordinate == enemy.y)) {
        turn++;
        printf("主人公の攻撃\n");
        _getch();
        int attack = player.attack / 2 + rand() % player.attack;
        enemy.energy -= attack;
        printf("敵に%dのダメージ", attack);
        _getch();
        fprintf(fp, "主人公の攻撃で敵に%dのダメージ ", attack);
        if (enemy.energy <= 0) {
            enemy.energy = 0;
            display();
            printf("敵を倒した\n");
            fprintf(fp,"敵を倒した");
            _getch();
        }
    }
    else {
        switch (space[y_coordinate][x_coordinate]) {
        case CELL_TYPE_NONE://「・」のマスなら移動
            turn++;
            player.x = x_coordinate;
            player.y = y_coordinate;
            //20マス移動すれば自動で1回復
            if ((turn % 20 == 0) && (player.energy < player.maxenergy)) player.energy++;
            fprintf(fp, "1マス進んだ ");
            break;

        case CELL_TYPE_WALL://「■」のマスなら移動できない
            fprintf(fp, "壁で進めない ");
            break;

        case CELL_TYPE_STAIR://「％」のマスなら新たなエリアを生成
            MakeField();
            field_num++;
            fprintf(fp, "階段を上った ");
            break;

        case CELL_TYPE_TREASURE://宝物ならエンディング
            printf("ゲームクリア！！");
            _getch();
            fprintf(fp, "ゲームをクリアした ");
            game_Flag = false;//ゲームを終了
            break;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------------


//ゲームの核の描画---------------------------------------------------------------------------------------------------
void display() {

    int str[F_height][F_width];
    memcpy(str, space, sizeof space);

    //キャラクターの体力が０になれば、表示を消すようにする------------------
    if(player.energy != 0) str[player.y][player.x] = CELL_TYPE_PLAYER;
    if(enemy.energy != 0) str[enemy.y][enemy.x] = CELL_TYPE_ENEMY;
    //---------------------------------------------------------------------

    system("cls");
    //階数や体力の描画-------------------------------------------------------
    printf("%d階 体力：%d/%d\n", field_num, player.energy, player.maxenergy);
    //-----------------------------------------------------------------------

    //主人公の半径８マスのみ見えるように設定--------------------------------------------------------------

    for (int y = player.y - Look_Radius; y < player.y + Look_Radius; y++) {
        for (int x = player.x - Look_Radius; x < player.x + Look_Radius; x++) {
            if ((x < 0) || (x >= F_width) || (y < 0) || (y >= F_height)) {
                printf("■");
            }
            else {
                printf("%s", sellArt[str[y][x]]);//周辺にあるものを描画する
            }


        }
        printf("\n");
    }
}
//--------------------------------------------------------------------------------------------------------------------






void MakeField() {
    count = 0;//エリアはまだ作成されていないため
    //階段の座標の変数--------------
    int stair_x;
    int stair_y;
    //------------------------------

    //初期位置の設定 座標（0,0）に設定------------------------
    lands[0].x_location = 0;
    lands[0].y_location = 0;

    lands[0].width = F_width;
    lands[0].height = F_height;
    //--------------------------------------------------------

    count++;//エリアが作成されたため

    SpritLand(0);

    //エリア全体を壁で覆う------------------------------------
    for (int h = 0; h < F_height; h++)
    {
        for (int w = 0; w < F_width; w++)
        {
            space[h][w] = CELL_TYPE_WALL;
        }
        
    }
    //--------------------------------------------------------

    //各空間の定義を行い描画する------------------------------------------------------------------------------------------
    for (int i = 0; i < count; i++) {
        lands[i].space.x = lands[i].x_location + 2;
        lands[i].space.y = lands[i].y_location + 2;
        lands[i].space.width = lands[i].width - 4;
        lands[i].space.height = lands[i].height - 4;

        //空間を作る
        for (int y = lands[i].space.y; y < lands[i].space.y + lands[i].space.height; y++) {
            for (int x = lands[i].space.x; x < lands[i].space.x + lands[i].space.width; x++) {
                space[y][x] = CELL_TYPE_NONE;
            }
        }

        //各空間の下に通路を作成
        for (int x = lands[i].x_location; x < lands[i].x_location + lands[i].width; x++) {
            space[lands[i].y_location + lands[i].height - 1][x] = CELL_TYPE_NONE;
        }

        //各空間の右端に通路を作成
        for (int y = lands[i].y_location; y < lands[i].y_location + lands[i].height; y++) {
            space[y][lands[i].x_location + lands[i].width - 1] = CELL_TYPE_NONE;
        }

        //各空間の上に入口を作成
        for (int j = lands[i].y_location; j < lands[i].space.y; j++) {
            space[j][lands[i].x_location + lands[i].space.width / 2] = CELL_TYPE_NONE;
        }

        //各空間の下に入口を作成
        for (int j = lands[i].y_location + lands[i].space.height; j < lands[i].y_location + lands[i].height; j++) {
            space[j][lands[i].x_location + lands[i].space.width / 2] = CELL_TYPE_NONE;
        }

        //各空間の左入口を作成
        for (int k = lands[i].x_location; k < lands[i].space.x; k++) {
            space[lands[i].y_location + lands[i].space.height /2][k] = CELL_TYPE_NONE;
        }

        //各空間の右入口を作成
        for (int k = lands[i].x_location + lands[i].space.width; k < lands[i].x_location + lands[i].width; k++) {
            space[lands[i].y_location + lands[i].space.height / 2][k] = CELL_TYPE_NONE;
        }

    }
    //-------------------------------------------------------------------------------------------------------------------------

    //エリアの右端と下端をブロックで埋める-------------------------------------------------------------------------------------
    for (int h = 0; h < F_height; h++)
    {
        space[h][F_width - 1] = CELL_TYPE_WALL;
        for (int w = 0; w < F_width; w++)
        {
            space[F_height - 1][w] = CELL_TYPE_WALL;
        }
    }
    //-------------------------------------------------------------------------------------------------------------------------


    //エリア外とつながる穴を塞ぐ作業-------------------------------------------------------------------------------------------------------    
        for (int h = 0; h < F_height; h++)
        {
            for (int w = 0; w < F_width; w++)
            {
                if (space[h][w] == CELL_TYPE_WALL)
                    continue;

                //四方を見るための配列
                int vec[][2] = {
                    {1,0},
                    {0,1},
                    {-1,0},
                    {0,-1},
                };
                //四方にあるブロックの数を数える変数
                int block = 0;


                for (int n = 0; n < 4; n++) {
                    int x = w + vec[n][0];
                    int y = h + vec[n][1];
                    if ((x < 0) || (x >= F_width) || (y < 0) || (y > F_height))
                        block++;
                    else if (space[y][x] == CELL_TYPE_WALL)
                        block++;
                }

                //ブロックが３つ以上あるとき、行き止まりとわかるため、そこもブロックでふさいでおく
                if (block >= 3) {
                    space[h][w] = CELL_TYPE_WALL;
                    
                }

            }

            
        }
    //-------------------------------------------------------------------------------------------------------------------------
    
        SetRandomLocation(&player.x, &player.y);
        SetRandomLocation(&stair_x, &stair_y);

    //５階までいくとランダムな座標に宝が設置。４階までは階段を設置。-----------------------------------------------------------
        if (field_num < 4 ) {
            space[stair_y][stair_x] = CELL_TYPE_STAIR;
        }
        else {
            space[stair_y][stair_x] = CELL_TYPE_TREASURE;
        }
    //-------------------------------------------------------------------------------------------------------------------------
        SetRandomLocation(&enemy.x, &enemy.y);
        //敵の初期値設定
        enemy.energy = enemy.maxenergy = 2 + field_num;
        enemy.attack = 2 + field_num;
}



//エリアを半分のサイズに分割する-----------------------------------------------------------------------------------------------
void SpritLand(int landNum) {
    int newNum = count;
    //前のエリアの幅と高さを保存-----------
    int w = lands[landNum].width;
    int h = lands[landNum].height;
    //-------------------------------------

    //横への分割と縦への分割を乱数によって決める-----------------------------------------------------
    if (rand() % 2) {
        lands[landNum].width /= 2;//幅を半分のサイズにする

        lands[newNum].x_location = lands[landNum].x_location + lands[landNum].width;
        lands[newNum].y_location = lands[landNum].y_location;
        lands[newNum].width = w - lands[landNum].width;
        lands[newNum].height = lands[landNum].height;
    }
    else
    {
        lands[landNum].height /= 2;//高さを半分のサイズにする

        lands[newNum].x_location = lands[landNum].x_location;
        lands[newNum].y_location = lands[landNum].y_location + lands[landNum].height;
        lands[newNum].width = lands[landNum].width;
        lands[newNum].height = h - lands[landNum].height;
    }
    //--------------------------------------------------------------------------------------------------

    //エリアの幅や高さがMin_Landsizeを下回れば、これ以上の分割はしないようにする------------------------
    if ((lands[landNum].width < Min_Landsize) || (lands[landNum].height < Min_Landsize) || (lands[newNum].width < Min_Landsize) || (lands[newNum].height < Min_Landsize))
    {
        //初期値に戻す
        lands[landNum].width = w;
        lands[landNum].height = h;
        return;
    }
    //--------------------------------------------------------------------------------------------------

    count++;

    SpritLand(landNum);//再帰的な呼び出し
    SpritLand(newNum);//再帰的な呼び出し
}
//------------------------------------------------------------------------------------------------------------------------------


//ランダムな座標をセットする----------------------------------------------------------------------------------------------------
void SetRandomLocation(int* _px, int* _py) {
    
    int initial_num = rand() % count;
    *_px = lands[initial_num].space.x + rand() % lands[initial_num].space.width;
    *_py = lands[initial_num].space.y + rand() % lands[initial_num].space.height;
}
//------------------------------------------------------------------------------------------------------------------------------


//敵が空間内にいるのか、空間同士をつなぐ通路にいるのかを判定する-------------------------------------------------------------------------------------------------------------
int GetSpace(int x, int y) {
    //どこの空間にいるのか
    for (int i = 0; i < count; i++) {
        if ((x >= lands[i].space.x) && (x < lands[i].space.x + lands[i].space.width) && (y >= lands[i].space.y) && (y < lands[i].space.y + lands[i].space.height)) {
            //どこかの空間内にいるときにはその空間のインデックスを返す
            return i;
        }
        else {
            //通路などにいるときには-1を返す
            return -1;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

