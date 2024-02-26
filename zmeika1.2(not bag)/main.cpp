#include <iostream>
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
	bool AppleOnField;								//Булевое поле в которая показывает, имеется ли яблоко на поле

	char field[y_max][x_max+1];						//Двумерный массив и по совместительству игровое поле с размерами y_max и x_max

	//Все методы будут описаны ниже по коду
	void CreateBorder(char ch);						//Метод для создание границ игрового поля. 
													//char ch - Это определеный символ из которого будут состоять границы
	void Show();									//Метод для отрисовки всего поля
	void GenerateApple();							//Метод для генерации яблока
};
//Class game person. Snake
class Snake {
private:
	int PositionSnakeX[max_len];					//Массив позиций каждого элемента змейки по оси ОХ
	int PositionSnakeY[max_len];					//Массив позиций каждого элемента змейки по оси ОY

	int Rotation;									//Rotation - поле показывающая вектор перемещания змейки
													//
													//      1 - UP; 2 - DOWN; 3 - LEFT; 4 - RIGHT
													//

	char sChar = -2;								//sChar - поле которая хранит символ из которого состоит змейка
	int Snake_Length;								//Размер змейки
public:
	Snake(int rotation, int lenght) {				//Конструктор класса snake для случаев когда нужно не стандартное начало
		this->Rotation = rotation;					//Выставляем изначальный вектор перемещения
		this->Snake_Length = lenght;				//Выставляем изначальную длину змейки

		this->PositionSnakeX[0] = x_max / 2;		//Ставим изначальные координаты змейки относительно оси ОХ
		this->PositionSnakeY[0] = y_max / 2;		//Ставим изначальные координаты змейки относительно оси ОY
	}
	Snake() {										//Стандартный конструктор
		this->Rotation = 1;							//Выставляем изначальный вектор перемещения
		this->Snake_Length = 1;						//Выставляем изначальную длину змейки

		this->PositionSnakeX[0] = x_max / 2;		//Ставим изначальные координаты змейки относительно оси ОХ
		this->PositionSnakeY[0] = y_max / 2;		//Ставим изначальные координаты змейки относительно оси ОY
	}

	void SetRotation(int rot) { this->Rotation = rot; }				//Метод для изменения вектора перемещения змейки

	void ToStarGame(Field& field);					//Метод для начала отрисовки змейки
	void Move(Field& field);						//Логика перемещения змейки по игровому полю
	void AddTail();									//Метод для увеличения размера змейки
	bool CheckGameOver();							//Проверка столкновения змейки с препядствиями

	int Get_rotation() { return Rotation; }			//Гетер для возвращения приватной переменной Rotation
};
//Метод для создания границ игрового поля
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
//Метод отрисовки игрового поля
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
//Метод для создания яблок
void Field::GenerateApple() {
	if (!AppleOnField) {							//Проверка. Имеется ли яблоко?
													//Если имеется, то выполняется кусок кода
		AppleOnField = true;						//Ставим значение AppleOnField в true, так как мы сейчас создадим яблоко
		while (true){								//Цикл для проверки координат для яблока
			int PosOX = rand() % (x_max - 2);			//Псевдо-рандомная позиция яблока по оси OX
			int PosOY = 1 + rand() % (y_max - 2);		//Псевдо - рандомная позиция яблока по оси OY

			if (field[PosOY][PosOX] == ' ') {			//Проврка, если координаты являются пусятыми, то создаем яблоко
				PositionAppleX = PosOX;					//Записываем координаты яблока по оси ОХ в поле PositionAppleX
				PositionAppleY = PosOY;					//Записываем координаты яблока по оси ОY в поле PositionAppleY
				field[PositionAppleY][PositionAppleX] = CharApple;				//Отрисовываем значок яблока
				break;									//Заканчиваем цикл
			}
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
//Метод по увеличению размера змеи на одну единицу
void Snake::AddTail() {
	this->PositionSnakeX[Snake_Length] = this->PositionSnakeX[Snake_Length - 1];//В последний элемент змеи запиывается координаты предпоследнего элемента змеи по оси OX
	this->PositionSnakeY[Snake_Length] = this->PositionSnakeY[Snake_Length - 1];//В последний элемент змеи запиывается координаты предпоследнего элемента змеи по оси OY

	this->Snake_Length++;							//Увеличение на одну еденицу длину змеи
}
//Метод для проверки проигрыша
bool Snake::CheckGameOver() {
	if (PositionSnakeX[0] == x_max - 1 || PositionSnakeX[0] == 0 ||				//Если координата головы змеи равна координатам границы игрового поля, то возвращаем trut (игра окончена)
		PositionSnakeY[0] == y_max - 1 || PositionSnakeY[0] == 0) return true;
	for (int i = 1; i <= Snake_Length; ++i) {									//Цикл в котором Сравниваем координату головы змеи с координатами других частей змеи
		if (PositionSnakeX[0] == PositionSnakeX[i + 1] &&						//Если координаты совпадают то возвращаем true (игра окончена)
			PositionSnakeY[0] == PositionSnakeY[i + 1]) return true;
	}
	return false;									//Возвращаем false
}

int main(VOID) {
	Field fi;										//Объявлеям объект игрового поля
	Snake snake;									//Объявляем объект змеи со стандартными значениями
													//int rotation = 1;
													//int lenght = 1;
													
													//Так же можно вызвать через 
													//Snake snake(int rotation, int lenght);
	//Небольшая настройка игрового поля
	fi.CreateBorder(219);							//Создание границ игрового поля (вместо '219', можно вписать любой другой символ)
	snake.ToStarGame(fi);							//Начальная отрисовка змейки
	fi.AppleOnField = false;						//Яблок нету на игровом поле
	fi.GenerateApple();								//создание первого яблока

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