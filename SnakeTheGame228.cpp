#include <iostream>
#include <time.h>
#include <conio.h>
#include "windows.h"
#include <string>
#include <fstream>
#include <vector>

using namespace std;

bool gameOver, restart, gameOver228;

bool nameHave = false;

bool isShowMenu = true;

const int width = 118;
const int height = 26;
int snakeX, snakeY, fructX, fructY, score;
string user_name = "";
string out_s;
int saveScore = 0;

struct snakeTail { int x, y; } tail[100];

int lengthTeil;

enum direction { start = 0, levo = 1, pravo = 2, verx = 3, niz = 4 };
direction dir;

enum toStartGame { loh = 0, toStart = 1, toLider = 2, toName = 3 };
toStartGame keyStart;

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void setProperes() {
	restart = false;
	gameOver = false;
	gameOver228 = false;
	lengthTeil = 0;
	dir = start;
	snakeX = width / 2 - 1;
	snakeY = height / 2 - 1;
	fructX = rand() % (width - 2);
	fructY = rand() % (height - 1);
	score = 0;
}

void DrawRestart() {

	for (int i = 0; i < height + 3; i++) {
		for (int j = 0; j < width + 1; j++) {
			if (i == snakeY + 2 && j == snakeX - 12) {
				cout << "GAME HAS BEEN RESTARTED";
			}
			if (i == snakeY + 3 && j == snakeX - 7) {
				cout << "Wait 5 seconds";
			}
			else cout << " ";
		}
		cout << endl;
	}
}

void DrawMenu() {
	system("cls");
	for (int i = 0; i < height + 3; i++) {
		for (int j = 0; j < width + 1; j++) {
			if (i == snakeY - 1 && j == snakeX - 13) {
				cout << "User Name: " << user_name;
			}
			if (i == snakeY && j == snakeX - 12) {
				cout << "Select Option";
			}
			if (i == snakeY + 2 && j == snakeX - 16) {
				cout << "A - Start Game";
			}
			if (i == snakeY + 4 && j == snakeX - 16) {
				cout << "S - Show Leaderboard";
			}
			if (i == snakeY + 6 && j == snakeX - 16) {
				cout << "W - Change Name";
			}
			if (i == snakeY + 8 && j == snakeX - 16) {
				cout << "D - Exit";
			}
			else cout << " ";
		}
		cout << endl;
	}
}

void DrawGameOver() {
	for (int i = 0; i < height + 3; i++) {
		for (int j = 0; j < width + 1; j++) {
			if (i == snakeY && j == snakeX - 9) {
				cout << "GAME OVER";
			}
			if (i == snakeY + 1 && j == snakeX - 10) {
				cout << "RESTART ?";
			}
			if (i == snakeY + 2 && j == snakeX - 9) {
				cout << "Press R";
			}
			if (i == snakeY + 3 && j == snakeX - 9) {
				cout << "Exit Q";
			}
			else cout << " ";
		}
		cout << endl;
	}
}

void Draw() {
	setcur(0, 0);
	for (int i = 0; i < width + 1; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1)
				cout << "#";
			if (i == snakeY && j == snakeX)
				cout << "S";
			else if (i == fructY && j == fructX)
				cout << "F";
			else {
				bool print = false;
				for (int k = 0; k < lengthTeil; k++) {
					if (tail[k].x == j && tail[k].y == i) {
						print = true;
						cout << "s";
					}
				}
				if (!print) cout << " ";
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width + 1; i++)
		cout << "#";
	cout << endl;
	cout << "Score: " << score << ";" << "\tMove: W,A,S,D;" << "\tRestart Game: R;" << "\tExit to menu : Q";
}

void KeyRestart() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'r':
			restart = true;
			break;
		case 'q':
			isShowMenu = true;
			break;
			
		}
	}
}

void KeyExitMenu() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'q':
			isShowMenu = true;
			break;
		}
	}
}

void KeyMenu() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			keyStart = toStart;
			break;
		case 's':
			keyStart = toLider;
			break;
		case 'w':
			keyStart = toName;
			break;
		case 'd':
			FatalExit(0);
			break;
		}
	}
}

void Keys() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			dir = levo;
			break;
		case 'w':
			dir = verx;
			break;
		case 'd':
			dir = pravo;
			break;
		case 's':
			dir = niz;
			break;
		case 'q':
			isShowMenu = true;
			break;
		case 'r':
			restart = true;
			break;
		}
	}
}
void Logic() {
	int predX = tail[0].x;
	int predY = tail[0].y;
	int pred2X, pred2Y;
	tail[0].x = snakeX;
	tail[0].y = snakeY;

	for (int i = 1; i < lengthTeil; i++)
	{
		pred2X = tail[i].x;
		pred2Y = tail[i].y;
		tail[i].x = predX;
		tail[i].y = predY;
		predX = pred2X;
		predY = pred2Y;
	}

	switch (dir)
	{
	case levo:
		snakeX--;
		break;
	case pravo:
		snakeX++;
		break;
	case verx:
		snakeY--;
		break;
	case niz:
		snakeY++;
		break;
	}

	/*if (snakeX > width || snakeX < 0 || snakeY > height || snakeY < 0) */
	if (snakeX >= width - 1) snakeX = 0;
	else if (snakeX < 0) snakeX = width - 2;

	if (snakeY >= height) snakeY = 0;
	else if (snakeY < 0) snakeY = height - 1;

	//при ударе в себя конец
	for (int i = 0; i < lengthTeil; i++) {
		if (tail[i].x == snakeX && tail[i].y == snakeY) {
			gameOver = true;
			gameOver228 = true;
		}
	}

	//срубает свой хвост при ударе себя
	/*for (int i = 1; i < lengthTeil; i++)
		if (tail[i].x == snakeX && tail[i].y == snakeY) lengthTeil = i;*/


	if (snakeX == fructX && snakeY == fructY) {
		score += 10;
		lengthTeil++;
		fructX = rand() % (width - 2);
		fructY = rand() % (height - 1);
	}
}

void HideCursor(bool Visible) {
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = !Visible;
	Cursor.dwSize = sizeof(Cursor);
	HANDLE Hand = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(Hand, &Cursor);
}

void recordInFile() {
	ofstream fout("liderbord.txt", ios_base::out | ios_base::app);
	fout << "\n" << "Nickname: " << user_name << " | " << "Score: " << score;
	fout.close();
}

void outputFile() {
	system("cls");
	vector<string> arr;

	int loh = 0;

	ifstream i("liderbord.txt");
	if (i.is_open())
	{

		while (getline(i, out_s))
		{
			arr.push_back(out_s);
		}

		for (int i = 0; i < height + 3; i++) {
			for (int j = 0; j < width + 1; j++) {
				if (i == (height / 2 - 1) - 6 && j == snakeX - 6) {
					cout << "Leaderboard";
				}
				if (i == ((height / 2 - 1) - 6) + (1 + loh) && j == snakeX - 14) {
					cout << arr[loh];
					if (loh < arr.size() - 1) {
						loh++;
					}
				}
				if (i == (height / 2 - 1) + (1 + loh) && j == snakeX - 8) {
					cout << "Exit to menu : Q";
				}
				else cout << " ";
			}
			cout << endl;
		}
	}
	i.close();

	while (true) {
		KeyExitMenu();

		if (isShowMenu) {
			snakeX = width / 2 - 1;
			snakeY = height / 2 - 1;
			DrawMenu();
			break;
		}
	}
}

void gameStart() {
	srand(time(0));
	setProperes();
	while (true) {

		if (isShowMenu) {
			snakeX = width / 2 - 1;
			snakeY = height / 2 - 1;
			DrawMenu();
			recordInFile();
			break;
		}

		if (!isShowMenu && !gameOver) {
			if (restart) {
				setProperes();
				DrawRestart();
				Sleep(5000);
			}
			/*Sleep(30);*/
			HideCursor(true);
			Draw();
			Keys();
			Logic();
		}

		else if (gameOver) {
			HideCursor(true);
			while (true) {
				if (gameOver228) {
					system("cls");
					gameOver228 = false;
					snakeX = width / 2 - 1;
					snakeY = height / 2 - 1;
					DrawGameOver();
				}
				if (saveScore < score) {
					saveScore = score;
				}
				snakeX = width / 2 - 1;
				snakeY = height / 2 - 1;
				KeyRestart();
				if (restart) {
					setProperes();
					DrawRestart();
					Sleep(5000);
					break;
				}
				if (isShowMenu) {
					gameOver = false;
					break;
				}
			}
		}
	}
}


int main()
{
	while (true)
	{
		cout << "Enter the name: ";
		cin >> user_name;
		if (user_name.length() > 0) {
			nameHave = true;
			isShowMenu = true;
		}
		if (nameHave) {
			setProperes();
			DrawMenu();
			while (isShowMenu) {

				HideCursor(true);

				KeyMenu();
				switch (keyStart) {
				case toStart:
					isShowMenu = false;
					keyStart = loh;
					gameStart();
					break;
				case toLider:
					isShowMenu = false;
					keyStart = loh;
					outputFile();
					break;
				case toName:
					user_name = "";
					nameHave = false;
					isShowMenu = false;
					keyStart = loh;
					system("cls");
					break;
				}
			}
		}
	}
	
}

