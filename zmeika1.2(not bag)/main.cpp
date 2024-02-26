﻿#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

VOID ErrorExit(LPCSTR);

const int x_max = 100;								//Длина игрового поля
const int y_max = 25;								//Высота игрового поля
const int max_len = y_max * x_max;					//Количество "клеточек" в игровом поле

//cursor
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}
//Class game pokygon
class Field {
public:
	int PositionAppleX;								//Позиция яблока по оси ОХ
	int PositionAppleY;								//Позиция яблока по оси OY
	char CharApple = '+';							//Иконка яблока
	bool AppleOnField;								//Булевая перременная в которая показывает, имеется ли яблоко на поле

	char field[y_max][x_max+1];						//Двумерный массив и по совместительству игровое поле с размерами y_max и x_max

	//Все функции будут описаны ниже по коду
	void CreateBorder(char ch);						//Функция для создание границ игрового поля. 
													//char ch - Это определеный символ из которого будут состоять границы
	void Show();									//Функция для отрисовки всего поля
	void GenerateApple();							//Функция для генерации яблока
};
//Class game person. Snake
class Snake {
private:
	int PositionSnakeX[max_len];					//Массив позиций каждого элемента змейки по оси ОХ
	int PositionSnakeY[max_len];					//Массив позиций каждого элемента змейки по оси О

	int Rotation;									//Rotation - переменная показывающая вектор перемещания змейки
													//
													//      1 - UP; 2 - DOWN; 3 - LEFT; 4 - RIGHT
													//

	char sChar = -2;								//sChar - переменная которая хранит символ из которого состоит змейка
	int Snake_Length;								//Размер змейки
public:
	Snake(int rotation, int lenght) {				//Конструктор класса snake
		this->Rotation = rotation;					//Выставляем изначальный вектор перемещения
		this->Snake_Length = lenght;				//Выставляем изначальную длину змейки

		this->PositionSnakeX[0] = x_max / 2;		//Ставим изначальные координаты змейки относительно оси ОХ
		this->PositionSnakeY[0] = y_max / 2;		//Ставим изначальные координаты змейки относительно оси ОY
	}

	void SetRotation(int rot) { this->Rotation = rot; }				//Функция для изменения вектора перемещения змейки

	void ToStarGame(Field& field);					//Функция для начала отрисовки змейки
	void Move(Field& field);						//Логика перемещения змейки по игровому полю
	void AddTail();									//Функция для увеличения размера змейки
	bool CheckGameOver();							//Проверка столкновения змейки с препядствиями

	int Get_rotation() { return Rotation; }			//Гетер для возвращения приватной переменной Rotation
};
//Функция для создания границ игрового поля
void Field::CreateBorder(char ch) {					//ch - может принимать любой знак, но по стандарту моей змейки это - 219
	for (int i = 0; i < y_max; ++i) {				//Создаем массив для оси OY
		for (int j = 0; j < x_max; ++j) {			//Создаем массив для оси OX
			//Проверка координат
			if (i == 0 || i == y_max - 1 ||			//Если координата совпадает с верхней границей или нижней, то отрисовываем знак границы	
				j == 0 || j == x_max-1) {			//Если координата совпадает с правой или левой границей, то отрисовываем знак границы
				field[i][j] = ch;					//Отрисовка знака
			}
			else {
				field[i][j] = ' ';					//Если координата не совпадает с концами границ, то отрисовываем пустое значение. Игровое поле
			}
		}
		field[i][y_max] = '\0';						//Добавляем в конец каждой строки знак конца строки
	}
}
//Функция отрисовки игрового поля
void Field::Show() {
	gotoxy(0, 0);									//Курсор который ставит все поле в начале консоли

	//Обычный цикл по всм элементам поля и последующий вывод их на экран
	for (int i = 0; i < y_max; ++i) {
		for (int j = 0; j < x_max; ++j) {
			cout << field[i][j];
		}
		cout << endl;
	}
}
//Функция для создания яблок
void Field::GenerateApple() {
	if (!AppleOnField) {							//Проверка. Имеется ли яблоко?
													//Если имеется, то выполняется кусок кода
		AppleOnField = true;						//Ставим значение AppleOnField в true, так как мы сейчас создадим яблоко
		int PosOX = rand() % (x_max - 2);			//Псевдо-рандомная позиция яблока по оси OX
		int PosOY = 1 + rand() % (x_max - 2);		//Псевдо - рандомная позиция яблока по оси OY

		if (field[PosOY][PosOX] == ' ') {			//Проврка, если координаты являются пусятыми, то создаем яблоко
			PositionAppleX = PosOX;		
			PositionAppleY = PosOY;	
			field[PositionAppleY][PositionAppleX] = CharApple;				//Отрисовываем значок яблока
		}
	}
}
//Отрисовка начального положения змейки
void Snake::ToStarGame(Field& field) {
	field.field[PositionSnakeY[0]][PositionSnakeX[0]] = sChar;
}
//Логика перемещения змейки по игровому полю
void Snake::Move(Field& field) {
	//Цикл для очистки змейки с игрового поля
	for (int i = 0; i < Snake_Length; ++i) {
		field.field[PositionSnakeY[i]][PositionSnakeX[i]] = ' ';
	}

	for (int i = Snake_Length; i>0; --i) {
		PositionSnakeX[i] = PositionSnakeX[i - 1];	//Присваиваем координаты оси OX предыдущей змеки, нынешней
		PositionSnakeY[i] = PositionSnakeY[i - 1];	//Присваиваем координаты оси OY предыдущей змеки, нынешней
	}
	//Блок изменения координат в зависимости от вектора перемешения змейки
	if (this->Rotation == 1) {
		this->PositionSnakeY[0]--;					//Если вектор = 1, то мы передвигаем значение головы змейки на одну единицу вверх
	}
	else if (this->Rotation == 2) {
		this->PositionSnakeY[0]++;					//Если вектор = 2, то мы передвигае значение головы змейки на одну единицу вниз
	}
	else if (this->Rotation == 3) {
		this->PositionSnakeX[0]--;					//Если вектор = 3, то мы передвигаем голову змейки на одну еденицу влево
	}
	else if (this->Rotation == 4) {
		this->PositionSnakeX[0]++;					//Если вектор = 4, то мы передвигаем голову змейки на одну единицу вправо
	}

	/*Следует отметить, что в ире система координат оси OY - перевернута
	  Поэтому когда мы перемещаем голову вверх, мы вычитаем координату, а не прибавляем
	  0---------------------------------------------->OX
	  |
	  |
	  |
	  |
	  |
	  |						  0
	  |
	  |
	  |
	  |
	  |
	  ↓
	  OY
	  */

	if (PositionSnakeX[0] == field.PositionAppleX	//Проверка взимодействия головы змеи с яблоком, если они одинаковы, то выполняется следующий код
		&& PositionSnakeY[0] == field.PositionAppleY) {
		field.AppleOnField = false;					//Ставим значение AppleOnField на false, так как мы его считай съели
		this->AddTail();							//Добавляем размер змейки + 1
		field.GenerateApple();						//Создаём еще одно яблоко
	}

	for (int i = 0; i < Snake_Length; ++i) {
		field.field[PositionSnakeY[i]][PositionSnakeX[i]] = sChar;		//Отрисовка всей змеи
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