// cpp-cli-invaders.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

constexpr int SCREEN_WIDTH = 32;
constexpr int SCREEN_HEIGHT = 20;
constexpr int INVADER_COLUMN = 11;
constexpr int INVADER_ROW = 5;
int screen_canvas[SCREEN_HEIGHT][SCREEN_WIDTH];

constexpr int FPS = 4;
constexpr int REFRESH_INTERVAL = 1000 / FPS;

enum
{
	TILE_NONE,
	TILE_INVADER,
	TILE_PLAYER,
	TILE_MAX
};

enum {
	DIR_RIGHT,//行動１：右に行く
	DIR_DOWN,//行動２：下に下がる
	DIR_LEFT,//行動３：左に行く
	DIR_MAX, //底辺に着地したら止まる？
};

typedef struct {
	int x, y;
}VEC2;

const char* tileAA[TILE_MAX] = {
	"　", //TILE_NONE
	"☆", //TILE_INVADER,
	"▲" //TILE_PLAYER
};

typedef struct {
	int x, y;
} INVADER;

VEC2 directions[] = {//インベーダーの動くベクトル
	{1,0},//DIR_RIGHT
	{0,1},//DIR_DOWN
	{-1,0},//DIR_LEFT
};
int invaderDirection = 0;
INVADER invaders[INVADER_ROW][INVADER_COLUMN];


typedef struct {
	int x, y;
} PLAYER;
PLAYER player;

void DrawScreen() {
	//コンソールのバッファ削除
	memset(screen_canvas, 0, sizeof screen_canvas);

	//インベーダー描画
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++) {
			INVADER _temp = invaders[y][x];
			screen_canvas[_temp.y][_temp.x] = TILE_INVADER;
		}

	//プレーヤー描画
	screen_canvas[player.y][player.x] = TILE_PLAYER;

	//コンソールのリフレッシュ
	system("cls");

	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			std::cout << tileAA[screen_canvas[y][x]];
			//std::cout << screen_canvas[y][x];
		}
		std::cout << std::endl;
	}

}

void Init() {
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++) {
			invaders[y][x].x = x * 2;
			invaders[y][x].y = y * 2;
		}

	DrawScreen();
	player.x = SCREEN_WIDTH / 2;
	player.y = SCREEN_HEIGHT - 2;
}


void MoveInvaders() {

	int nextDirection = invaderDirection;

	//インベーダーの位置更新
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++) {
			invaders[y][x].x += directions[invaderDirection].x;
			invaders[y][x].y += directions[invaderDirection].y;

			//次の作戦決定
			switch (invaderDirection) {
			case DIR_RIGHT:
				if (invaders[y][x].x >= SCREEN_WIDTH - 1) {
					//右に動いた後右端にいる場合は下に移動させる
					nextDirection = DIR_DOWN;
				}
				break;
			case DIR_DOWN:
				if (invaders[y][x].x >= SCREEN_WIDTH - 1) {
					//下に動いた後右端にいる場合は左に移動させる
					nextDirection = DIR_LEFT;
				}
				else if (invaders[y][x].x <= 0) {
					//左に動いた後左端にいる場合は右に移動させる
					nextDirection = DIR_RIGHT;
				}
				break;
			case DIR_LEFT:
				if (invaders[y][x].x <= 0) {
					//左に動いた後左端にいる場合は右に移動させる
					nextDirection = DIR_DOWN;
				}
				break;
			}

		}

	invaderDirection = nextDirection;
}


int main()
{
	//std::cout << "Hello World!\n";

	bool isContinue = true;

	Init();
	//int a = _getch(screen_canvas);
	clock_t lastClock = clock();
	while (isContinue) {
		clock_t nowClock = clock();

		if (nowClock - lastClock >= REFRESH_INTERVAL) {
			lastClock = nowClock;
			MoveInvaders();
			DrawScreen();
		}
	}
	return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
