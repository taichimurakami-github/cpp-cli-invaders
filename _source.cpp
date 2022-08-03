// cpp-cli-invaders.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>

namespace _source {

	constexpr int SCREEN_WIDTH = 32;
	constexpr int SCREEN_HEIGHT = 20;
	constexpr int INVADER_COLUMN = 11;
	constexpr int INVADER_ROW = 5;
	int screen_canvas[SCREEN_HEIGHT][SCREEN_WIDTH];

	//set fps
	constexpr int FPS = 60;
	constexpr int INVADER_REFRESH_FPS = 2;
	constexpr int INVADER_BULLET_REFRESH_FPS = 2;


	enum
	{
		TILE_NONE,
		TILE_INVADER,
		TILE_PLAYER,
		TILE_PLAYER_BULLET,
		TILE_INVADER_BULLET,
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
		"　",//TILE_NONE
		"☆",//TILE_INVADER,
		"▲",//TILE_PLAYER
		"↑",//TILE_PLAYYER_BULLET
		"○", //TILE_INVADER_BULLET
	};

	typedef struct {
		int x, y;
		bool isDead;
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
		bool isFired;
	}INVADERBULLET;
	INVADERBULLET invaderBullet[INVADER_COLUMN];//各列に一つとする


	typedef struct {
		int x, y;
	} PLAYER;

	PLAYER player;

	typedef struct {
		int x, y;
		bool isFired;
	}PLAYERBULLET;

	PLAYERBULLET playerBullet;

	void DrawScreen() {
		//コンソールのバッファ削除
		memset(screen_canvas, 0, sizeof screen_canvas);

		//インベーダー描画
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				//死んでいたら描画しない
				if (invaders[y][x].isDead) continue;

				INVADER _temp = invaders[y][x];
				screen_canvas[_temp.y][_temp.x] = TILE_INVADER;
			}

		//インベーダーの弾描画
		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (invaderBullet[x].isFired) {
				INVADERBULLET _temp = invaderBullet[x];
				screen_canvas[_temp.y][_temp.x] = TILE_INVADER_BULLET;
			}
		}

		//プレーヤー描画
		screen_canvas[player.y][player.x] = TILE_PLAYER;

		//プレーヤーの弾発射
		if (playerBullet.isFired) {
			screen_canvas[playerBullet.y][playerBullet.x] = TILE_PLAYER_BULLET;
		}

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
		//インベーダー初期化
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				invaders[y][x].x = x * 2;
				invaders[y][x].y = y * 2;
				invaders[y][x].isDead = false;
			}

		//インベーダーの弾初期化
		for (int x = 0; x < INVADER_COLUMN; x++) {
			invaderBullet[x].isFired = false;
		}

		//プレーヤーの位置初期化
		player.x = SCREEN_WIDTH / 2;
		player.y = SCREEN_HEIGHT - 2;

		//プレーヤーの弾初期化
		playerBullet.isFired = false;

		DrawScreen();
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
	//インベーダーの弾発射処理
	void ShootInvaderBullets() {

		for (int x = 0; x < INVADER_COLUMN; x++) {
			//弾の発射確率を操作
			if ((rand() % 100)) {
				continue;
			}

			if (!invaderBullet[x].isFired) {
				//invaderBullet[x].isFired = true;

				//その列の生きているinvaderの中で一番下の行にいるInvaderを求める
				int invaderRow = -1;
				for (int y = 0; y < INVADER_ROW; y++) {
					if (!invaders[y][x].isDead) {
						invaderRow = y;
					}
				}

				//生きているインベーダーがその列にいた場合
				if (invaderRow >= 0) {
					invaderBullet[x].isFired = true;
					invaderBullet[x].x = invaders[invaderRow][x].x;
					invaderBullet[x].y = invaders[invaderRow][x].y + 1;
				}
			}
		}

		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (invaderBullet[x].isFired) {
				invaderBullet[x].y++;
				if (invaderBullet[x].y >= SCREEN_HEIGHT) {
					invaderBullet[x].isFired = false;
				}
			}
		}
	}

	//playerBulletとInvaderの当たり判定
	bool playerBulletIntersectInvaders() {
		for (int y = 0; y < INVADER_ROW; y++)
			for (int x = 0; x < INVADER_COLUMN; x++) {
				//インベーダーの亡霊に弾が当たってもすり抜けるようにする
				if (invaders[y][x].isDead) continue;

				bool isXaxisEqual = (invaders[y][x].x == playerBullet.x);
				bool isYaxisEqual = (invaders[y][x].y == playerBullet.y);
				if (isXaxisEqual && isYaxisEqual) {
					playerBullet.isFired = false;
					invaders[y][x].isDead = true;
					return true;
				}
			}
		return false;
	}

	bool invaderBulletIntersectPlayer() {
		for (int x = 0; x < INVADER_COLUMN; x++) {
			if (!invaderBullet[x].isFired) continue;

			bool isXaxisEqual = (invaderBullet[x].x == player.x);
			bool isYaxisEqual = (invaderBullet[x].y == player.y);

			//プレーヤーと衝突しました
			if (isXaxisEqual && isYaxisEqual) {
				return true;//ゲームオーバー！！！
			}
		}

		return false;
	}

	int main()
	{
		//std::cout << "Hello World!\n";

		bool isContinue = true;

		std::cout << "this is _source.cpp" << std::endl;

		//calc interval from fps
		constexpr int refresh_interval = 1000 / FPS;
		constexpr int invader_refresh_interval = 1000 / INVADER_REFRESH_FPS;
		constexpr int invader_bullet_refresh_interval = 1000 / INVADER_BULLET_REFRESH_FPS;

		Init();
		//int a = _getch(screen_canvas);
		clock_t lastClock = clock();
		clock_t lastClockForInvaderInterval = lastClock;
		clock_t lastClockForInvaderBulletInterval = lastClock;
		while (isContinue) {
			clock_t nowClock = clock();

			clock_t clockDiff = nowClock - lastClock;
			clock_t clockDiffForInvaderInterval = nowClock - lastClockForInvaderInterval;
			clock_t clockDiffForInvaderBulletinterval = nowClock - lastClockForInvaderBulletInterval;
			if (clockDiff >= refresh_interval) {
				lastClock = nowClock;

				if (_kbhit()) {
					switch (_getch()) {
					case 'a': player.x--; break;
					case 'd': player.x++; break;
					default:
						if (!playerBullet.isFired) {
							playerBullet.isFired = true;
							playerBullet.x = player.x;
							playerBullet.y = player.y - 1;
						}
						break;
					}

					if (player.x < 0) player.x = 0;
					if (player.x >= SCREEN_WIDTH) player.x = SCREEN_WIDTH - 1;
					if (invaderBulletIntersectPlayer()) {
						Init();
						continue;
					}
				}

				//プレーヤーの弾を移動させる
				if (playerBullet.isFired) {
					playerBullet.y--;
					if (playerBullet.y < 0) {
						playerBullet.isFired = false;
					}

					playerBulletIntersectInvaders();
				}

				//インベーダー移動処理
				if (clockDiffForInvaderInterval >= invader_refresh_interval) {
					lastClockForInvaderInterval = nowClock;
					MoveInvaders();
				}


				//インベーダーの弾の発射処理
				if (clockDiffForInvaderBulletinterval >= invader_bullet_refresh_interval) {
					lastClockForInvaderBulletInterval = nowClock;
					ShootInvaderBullets();
				}

				//canvas更新
				DrawScreen();

				//インベーダーの弾が当たったらゲームオーバー！！
				if (invaderBulletIntersectPlayer()) {
					Init();
					lastClock = clock();
				}

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
}
