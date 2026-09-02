#include "life.h"
#include <iostream>
#include <cstring>
#include "SDL.h"


int main(int argc, char* argv[])
{
    std::cout << "Starting Game of Life..." << std::endl;

    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "SDL initialized successfully" << std::endl;
    // Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "Game of Life - SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        FRAME_W * ZOOM,
        FRAME_H * ZOOM,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    std::cout << "Window created successfully" << std::endl;

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);
    
    if (!renderer)
    {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer)
        {
            std::cout << "Failed to create software renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        std::cout << "Using software renderer" << std::endl;
    }
    else
    {
        std::cout << "Hardware renderer created successfully" << std::endl;
    }

    // Создание текстуры
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_BGR24,
        SDL_TEXTUREACCESS_STREAMING,
        FRAME_W,
        FRAME_H
    );

    if (!texture)
    {
        std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "Texture created successfully" << std::endl;

    // Инициализация игрового поля
    FieldInit(F1);
    std::cout << "Field initialized" << std::endl;

    // Основной цикл
    bool running = true;
    SDL_Event event;
    int frameCount = 0;
    bool placingSpaceship = false;
    bool placingBigSpaceship = false;
    bool shootingMode = false;

    std::cout << "Entering main loop..." << std::endl;
    std::cout << "Controls: SPACE=clear, A+click=small ship, B+click=big ship, S+click=shoot, ESC=quit" << std::endl;

    while (running)
    {
        // Обработка событий
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (event.key.keysym.sym == SDLK_SPACE)
                {
                    // Очистка поля
                    memset(F1, 0, sizeof(F1));
                    memset(F2, 0, sizeof(F2));
                    std::cout << "Field cleared!" << std::endl;
                }
                else if (event.key.keysym.sym == SDLK_a)
                {
                    placingSpaceship = true;
                    placingBigSpaceship = false;
                    shootingMode = false;
                    std::cout << "Click to place small spaceship!" << std::endl;
                }
                else if (event.key.keysym.sym == SDLK_b)
                {
                    placingBigSpaceship = true;
                    placingSpaceship = false;
                    shootingMode = false;
                    std::cout << "Click to place BIG spaceship!" << std::endl;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    shootingMode = true;
                    placingSpaceship = false;
                    placingBigSpaceship = false;
                    std::cout << "Click to shoot!" << std::endl;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    int fieldX = mouseX / ZOOM;
                    int fieldY = mouseY / ZOOM;
                    
                    std::cout << "Mouse clicked at: " << mouseX << "," << mouseY;
                    std::cout << " -> Field: " << fieldX << "," << fieldY << std::endl;
                    
                    if (placingSpaceship)
                    {
                        PlaceSpaceship(F1, fieldX, fieldY);
                        std::cout << "Small spaceship placed!" << std::endl;
                        placingSpaceship = false;
                    }
                    else if (placingBigSpaceship)
                    {
                        PlaceBigSpaceship(F1, fieldX, fieldY);
                        std::cout << "Big spaceship placed!" << std::endl;
                        placingBigSpaceship = false;
                    }
                    else if (shootingMode)
                    {
                        ShootFromSpaceship(F1, fieldX, fieldY);
                        std::cout << "Shot fired!" << std::endl;
                        shootingMode = false;
                    }
                }
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 77, 120, 204, 255);
        SDL_RenderClear(renderer);

        // Обновление и отрисовка игры
        FieldDraw(F1);
        NewGeneration(F1, F2);
        memcpy(F1, F2, sizeof(F1));

        // Обновление текстуры
        if (SDL_UpdateTexture(texture, NULL, Frame, FRAME_W * 3) != 0)
        {
            std::cout << "SDL_UpdateTexture Error: " << SDL_GetError() << std::endl;
        }

        // Отрисовка текстуры
        SDL_Rect destRect = {0, 0, FRAME_W * ZOOM, FRAME_H * ZOOM};
        if (SDL_RenderCopy(renderer, texture, NULL, &destRect) != 0)
        {
            std::cout << "SDL_RenderCopy Error: " << SDL_GetError() << std::endl;
        }

        // Обновление экрана
        SDL_RenderPresent(renderer);

        frameCount++;
        if (frameCount % 100 == 0)
        {
            std::cout << "Frames rendered: " << frameCount << std::endl;
        }

        // Небольшая задержка для контроля скорости
        SDL_Delay(5);
    }

    std::cout << "Shutting down..." << std::endl;

    // Очистка ресурсов
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Cleanup completed" << std::endl;
    return 0;
}
