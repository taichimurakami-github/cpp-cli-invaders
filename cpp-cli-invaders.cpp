// cpp-cli-invaders.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//



#include "Game.h"


int main() {

	Game* game = new Game();

	game->SetGState(Game::GState::PLAYING);
	game->PlayGame();

	//Init();
	//int a = _getch(screen_canvas);

	return 0;
}