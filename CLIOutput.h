#pragma once
#include <iostream>
#include <string>
#include <time.h>

class CLIOutput
{
public:
	CLIOutput() {}

	void DrawCenter(std::string s)
	{
		std::string margin_left = "";
		int left_space_length = (_line_width - (int)s.length()) / 2;

		for (int i = 0; i < left_space_length; i++) {
			margin_left += " ";
		}

		std::cout << margin_left << s << "\n";
	}

	void DrawCRLF(int num = 1) {

		for (int i = 0; i < num; i++) {
			std::cout << "\n";
		}
	}

	void DrawLine(std::string s) {
		for (int i = 0; i < _line_width; i++) {
			std::cout << s;
		}

		std::cout << "\n";
	}

	void DrawEndl() {
		std::cout << std::endl;
	}

	void DrawString(std::string s) {
		std::cout << s;
	}

	void DrawTitle() {
		DrawLine("=");
		DrawCRLF(3);
		DrawCenter("INVADER ++");
		DrawCRLF(2);
		DrawCenter("(c)2022 Taichi Murakami all rights reserved.");
		DrawCRLF(3);
		DrawLine("=");
		DrawCRLF(3);
		DrawCenter("PRESS 'SPACE' TO START");
		DrawCRLF();
		DrawEndl();
	}

	void DrawReady() {

		clock_t now_clock = clock();
		clock_t last_clock = now_clock;

		while (now_clock - last_clock < 1500) {
			system("cls");
			now_clock = clock();

			DrawCRLF(3);

			//ローディング風簡易アニメーション
			if (now_clock - last_clock < 300) {
				DrawCenter("READY");
			}
			else if (now_clock - last_clock < 600) {
				DrawCenter("READY.");
			}
			else if (now_clock - last_clock < 900) {
				DrawCenter("READY..");
			}
			else if (now_clock - last_clock < 1200) {
				DrawCenter("READY...");
			}
			else {
				DrawCenter("READY....");
			}

			DrawCRLF(3);
		}
	}

	void DrawGameOver() {
		DrawLine("!");
		DrawCRLF(2);
		DrawCenter("GAME OVER");
		DrawCRLF(2);
		DrawLine("!");
		DrawCRLF(4);
		DrawCenter("PRESS 'Q' TO TITLE");
		DrawCRLF(2);
		DrawCenter("PRESS 'SPACE' TO PLAY AGAIN");
		DrawEndl();
	}

	void DrawGameClear() {
		DrawCRLF(2);
		DrawCenter("★GAME CLEAR★");
		DrawCRLF(2);
		DrawCenter("Thank you for playing.");
		DrawCRLF(4);
		DrawCenter("PRESS 'Q' TO TITLE");
		DrawCRLF(2);
		DrawCenter("PRESS 'SPACE' TO PLAY AGAIN");
		DrawEndl();
	}

	static const std::string colorset_white;
	static const std::string colorset_green;
	static const std::string colorset_yellow;
	static const std::string colorset_red;
	static const std::string colorset_cian;


private:
	int _line_width = 32 * 2;
	int _line_height = 30;
};
