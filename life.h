#pragma once

#include "SDL.h"
#include <cstdlib>
#include <cstring>

const int LIFE_W = 100;
const int LIFE_H = 100;
const int FRAME_W = LIFE_W;
const int FRAME_H = LIFE_H;
const int ZOOM = 8;

using byte = unsigned char;

extern byte F1[LIFE_H][LIFE_W];
extern byte F2[LIFE_H][LIFE_W];
extern byte Frame[FRAME_H][FRAME_W][3];

int GetCell(byte F[LIFE_H][LIFE_W], int x, int y);
void FieldInit(byte F[LIFE_H][LIFE_W]);
void PutPixel(int x, int y, int r, int g, int b);
void FieldDraw(byte F[LIFE_H][LIFE_W]);
void SetCell(byte F[LIFE_H][LIFE_W], int x, int y, int value);
int GetNeighbours(byte field[LIFE_H][LIFE_W], int x, int y);
void NewGeneration(byte f1[LIFE_H][LIFE_W], byte f2[LIFE_H][LIFE_W]);

void PlaceSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY);
void PlaceBigSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY);
void ShootFromSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY);
