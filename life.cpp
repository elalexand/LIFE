#include "life.h"
#include <iostream>

byte F1[LIFE_H][LIFE_W] = {0};
byte F2[LIFE_H][LIFE_W] = {0};
byte Frame[FRAME_H][FRAME_W][3] = {0};

int GetCell(byte F[LIFE_H][LIFE_W], int x, int y)
{
    x = (x + LIFE_W) % LIFE_W;
    y = (y + LIFE_H) % LIFE_H;
    return F[y][x];
}

void FieldInit(byte F[LIFE_H][LIFE_W])
{
    std::cout << "Initializing field with random cells..." << std::endl;
    int liveCells = 0;
    for (int y = 0; y < LIFE_H; y++)
    {
        for (int x = 0; x < LIFE_W; x++)
        {
            F[y][x] = rand() % 2;
            if (F[y][x]) liveCells++;
        }
    }
    std::cout << "Live cells: " << liveCells << "/" << (LIFE_W * LIFE_H) << std::endl;
}

void PutPixel(int x, int y, int r, int g, int b)
{
    if (x >= 0 && x < FRAME_W && y >= 0 && y < FRAME_H)
    {
        Frame[y][x][0] = b;
        Frame[y][x][1] = g;
        Frame[y][x][2] = r;
    }
}

void FieldDraw(byte F[LIFE_H][LIFE_W])
{
    // Сначала очистим кадр
    for (int y = 0; y < FRAME_H; y++)
    {
        for (int x = 0; x < FRAME_W; x++)
        {
            Frame[y][x][0] = 255; // B
            Frame[y][x][1] = 255; // G
            Frame[y][x][2] = 255; // R - белый фон
        }
    }

    // Затем рисуем клетки
    for (int y = 0; y < LIFE_H; y++)
    {
        for (int x = 0; x < LIFE_W; x++)
        {
            if (GetCell(F, x, y))
                PutPixel(x, y, 255, 20, 255);  // Фиолетовые живые клетки
            else
                PutPixel(x, y, 30, 10, 40);   // Серые мертвые клетки
        }
    }
}

void SetCell(byte F[LIFE_H][LIFE_W], int x, int y, int value)
{
    if (x >= 0 && x < LIFE_W && y >= 0 && y < LIFE_H)
        F[y][x] = value;
}

int GetNeighbours(byte field[LIFE_H][LIFE_W], int x, int y)
{
    return GetCell(field, x - 1, y - 1) +
           GetCell(field, x, y - 1) +
           GetCell(field, x + 1, y - 1) +
           GetCell(field, x - 1, y) +
           GetCell(field, x + 1, y) +
           GetCell(field, x - 1, y + 1) +
           GetCell(field, x, y + 1) +
           GetCell(field, x + 1, y + 1);
}

void NewGeneration(byte f1[LIFE_H][LIFE_W], byte f2[LIFE_H][LIFE_W])
{
    for (int y = 0; y < LIFE_H; y++)
    {
        for (int x = 0; x < LIFE_W; x++)  // Исправлено: LIFE_W вместо LIFE_H
        {
            int n = GetNeighbours(f1, x, y);
            if (GetCell(f1, x, y))
            {
                if (n < 2 || n > 3)
                    SetCell(f2, x, y, 0);
                else
                    SetCell(f2, x, y, 1);
            }
            else
            {
                if (n == 3)
                    SetCell(f2, x, y, 1);
                else
                    SetCell(f2, x, y, 0);
            }
        }
    }
}
void PlaceSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY)
{
    // Фигура "Glider" - легкий космический корабль
    int glider[5][2] = {
        {0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}
    };
    
    // Размещаем фигуру относительно центра клика
    for (int i = 0; i < 5; i++)
    {
        int x = centerX + glider[i][0];
        int y = centerY + glider[i][1];
        SetCell(F, x, y, 1);
    }
}
// Добавьте в конец life.cpp

// Большой космический корабль (паттерн "Heavyweight spaceship")
void PlaceBigSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY)
{
    int hwss[13][2] = {
        {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 0},
        {2, 2}, {3, 0}, {3, 1}, {3, 2}, {3, 3},
        {4, 0}, {4, 2}, {5, 1}
    };
    
    for (int i = 0; i < 13; i++)
    {
        int x = centerX + hwss[i][0];
        int y = centerY + hwss[i][1];
        SetCell(F, x, y, 1);
    }
}

// "Стрельба" - создаем паттерн, который выглядит как выстрел
void ShootFromSpaceship(byte F[LIFE_H][LIFE_W], int centerX, int centerY)
{
    // Паттерн "выстрела" - небольшая фигура, которая движется
    int shot[4][2] = {
        {0, 0}, {1, 0}, {2, 0}, {1, 1}
    };
    
    // Размещаем выстрел перед кораблем
    for (int i = 0; i < 4; i++)
    {
        int x = centerX + shot[i][0];
        int y = centerY + shot[i][1] - 4; // Выше корабля
        SetCell(F, x, y, 1);
    }
    
    std::cout << "Pew! Pew! Shot fired!" << std::endl;
}
