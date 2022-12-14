#pragma once
#include<iostream>
#include <random>
#include <string>
#include <conio.h>
#include<iomanip>
#include<Windows.h>
#define MaxLen 10

using namespace std;

class Road {
public:
	Road();
	~Road();

public:
	void Init();
	void PrintBoard();
	void SetRoad();
	void MoveBoard(bool dir);

public:
	int board[MaxLen][MaxLen];
	int numberSize;
};

struct Position {
	int x;
	int y;
};