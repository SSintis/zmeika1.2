#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

VOID ErrorExit(LPCSTR);

/*game polygon congig
	x_max - weight settings
	y_max - height settings
*/
const int x_max = 100;
const int y_max = 25;
const int max_len = y_max * x_max;

//cursor
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}
//Class game pokygon
class Field {
public:
	int PositionAppleX;
	int PositionAppleY;
	char CharApple = '+';
	bool AppleOnField;

	char field[y_max][x_max+1];
	void CreateBorder(char ch);
	void Show();
	void GenerateApple();
};

class Snake {
private:
	int PositionSnakeX[max_len];
	int PositionSnakeY[max_len];

	int Rotation;// 1 - UP; 2 - DOWN; 3 - LEFT; 4 - RIGHT
	char sChar = -2;

	int Snake_Length;
public:
	Snake(int rotation, int lenght) {
		this->Rotation = rotation;
		this->Snake_Length = lenght;

		this->PositionSnakeX[0] = x_max / 2;
		this->PositionSnakeY[0] = y_max / 2;
	}

	void SetRotation(int rot) {
		this->Rotation = rot;
	}

	void ToStarGame(Field& field);
	void Move(Field& field);
	void AddTail();
	bool CheckGameOver();

	int Get_rotation() {
		return Rotation;
	}
};

void Field::CreateBorder(char ch) {
	for (int i = 0; i < y_max; ++i) {
		for (int j = 0; j < x_max; ++j) {
			if (i == 0 || i == y_max - 1 ||
				j == 0 || j == x_max-1) {
				field[i][j] = ch;
			}
			else {
				field[i][j] = ' ';
			}
		}
		field[i][y_max] = '\0';
	}
}

void Field::Show() {
	gotoxy(0, 0);
	for (int i = 0; i < y_max; ++i) {
		for (int j = 0; j < x_max; ++j) {
			cout << field[i][j];
		}
		cout << endl;
	}
}

void Field::GenerateApple() {
	if (!AppleOnField) {
		AppleOnField = true;
		PositionAppleX = rand() % (x_max - 2);
		PositionAppleY = 1 + rand() % (y_max - 2);
		field[PositionAppleY][PositionAppleX] = CharApple;
	}
}

void Snake::ToStarGame(Field& field) {
	field.field[PositionSnakeY[0]][PositionSnakeX[0]] = sChar;
}

void Snake::Move(Field& field) {
	for (int i = 0; i < Snake_Length; ++i) {
		field.field[PositionSnakeY[i]][PositionSnakeX[i]] = ' ';
	}

	for (int i = Snake_Length; i>0; --i) {
		PositionSnakeX[i] = PositionSnakeX[i - 1];
		PositionSnakeY[i] = PositionSnakeY[i - 1];
	}

	if (this->Rotation == 1) {
		this->PositionSnakeY[0]--;
	}
	else if (this->Rotation == 2) {
		this->PositionSnakeY[0]++;
	}
	else if (this->Rotation == 3) {
		this->PositionSnakeX[0]--;
	}
	else if (this->Rotation == 4) {
		this->PositionSnakeX[0]++;
	}

	if (PositionSnakeX[0] == field.PositionAppleX && PositionSnakeY[0] == field.PositionAppleY) {
		field.AppleOnField = false;
		this->AddTail();
		field.GenerateApple();
	}

	for (int i = 0; i < Snake_Length; ++i) {
		field.field[PositionSnakeY[i]][PositionSnakeX[i]] = sChar;
	}
}

void Snake::AddTail() {
	this->PositionSnakeX[Snake_Length] = this->PositionSnakeX[Snake_Length - 1];
	this->PositionSnakeY[Snake_Length] = this->PositionSnakeY[Snake_Length - 1];

	this->Snake_Length++;
}

bool Snake::CheckGameOver() {
	if (PositionSnakeX[0] == x_max - 1 || PositionSnakeX[0] == 0 ||
		PositionSnakeY[0] == y_max - 1 || PositionSnakeY[0] == 0) return true;
	for (int i = 1; i <= Snake_Length; ++i) {
		if (PositionSnakeX[0] == PositionSnakeX[i + 1] &&
			PositionSnakeY[0] == PositionSnakeY[i + 1]) return true;
	}
	return false;
}

int main(VOID) {
	Field fi;
	Snake snake(1, 1); //create cnake class

	fi.CreateBorder(219);
	snake.ToStarGame(fi);
	fi.AppleOnField = false;
	fi.GenerateApple();

	HANDLE hStdin;
	const short bufferSize = 128;
	INPUT_RECORD irInBuf[bufferSize];

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE) {
		ErrorExit("GetStdHandle");
	}

	int index = 0;
	int eventCount = 0;

	while (index < 1000000) {
		DWORD cNumRead = 0;
		BOOL peekSuccesful = PeekConsoleInput(
			hStdin,
			irInBuf,
			bufferSize,
			&cNumRead
		);

		if (!peekSuccesful) {
			ErrorExit("PeekConsoleInput");
		}
		if (!FlushConsoleInputBuffer(hStdin)) {
			ErrorExit("FlushConsoleInputBuffer");
		}

		eventCount += cNumRead;

		for (DWORD i = 0; i < cNumRead; ++i) {
			if (irInBuf[i].EventType == KEY_EVENT) {
				KEY_EVENT_RECORD ker = irInBuf[i].Event.KeyEvent;
				if (ker.bKeyDown) {
					if (ker.wVirtualKeyCode == 87) {
						if (snake.Get_rotation() != 2) {
							snake.SetRotation(1);
						}
					}
					else if (ker.wVirtualKeyCode == 83) {
						if (snake.Get_rotation() != 1) {
							snake.SetRotation(2);
						}
					}
					else if (ker.wVirtualKeyCode == 65) {
						if (snake.Get_rotation() != 4) {
							snake.SetRotation(3);
						}
					}
					else if (ker.wVirtualKeyCode == 68) {
						if (snake.Get_rotation() != 3) {
							snake.SetRotation(4);
						}
					}
				}
				else {
					//printf("Key Realesed\n");
				}
			}
		}
		snake.Move(fi);
		fi.Show();
		Sleep(85);

		if (snake.CheckGameOver()) break;

		index++;
	}
	return 0;
}

VOID ErrorExit(LPCSTR lpszMessage) {
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(0);
}