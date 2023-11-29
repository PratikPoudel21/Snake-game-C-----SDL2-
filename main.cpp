#include <SDL.h>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>

#define HEIGHT 600
#define WIDTH 600
#define SIZE 20

struct Apple { int x, y; };
struct snakeSegment { int x, y; };
enum Direction { UP, DOWN, LEFT, RIGHT };

void draw(SDL_Renderer *renderer, std::vector<snakeSegment> snake, Apple apple)
{
    SDL_Rect rect;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    for ( int i = 0; i < snake.size(); i++ )
    {
        rect = {snake[i].x, snake[i].y, SIZE, SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    rect = {apple.x, apple.y, SIZE, SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}



int main(int argc, char *argv[])
{
    system("cls");
    SDL_Init(SDL_INIT_VIDEO);

    std::vector<snakeSegment> snake = { {100, 100} };
    int snake_length = snake.size();
    Direction snake_dir = RIGHT;

    srand(time(0));

    Apple apple = { (rand() % (WIDTH / SIZE - 1) + 1) * SIZE, (rand() % (HEIGHT / SIZE - 1) + 1) * SIZE};

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
        std::cerr << "Window and renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_SetWindowTitle(window, "Snake game");

    SDL_Event e;
    
    bool run = true;
    while (run)
    {
        SDL_Delay(200);
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        run = false;
                        break;
                    case SDLK_UP:
                        snake_dir = UP;
                        break;   
                    case SDLK_DOWN:
                        snake_dir = DOWN;
                        break;   
                    case SDLK_LEFT:
                        snake_dir = LEFT;
                        break;   
                    case SDLK_RIGHT:
                        snake_dir = RIGHT;
                        break;   
                }
            }
        }


        for ( int i = snake.size() - 1; i > 0; i-- )
        {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }

        switch (snake_dir) 
        {
            case UP:
                snake[0].y -= SIZE;
                break;
            case DOWN:
                snake[0].y += SIZE;
                break;
            case LEFT:
                snake[0].x -= SIZE;
                break;
            case RIGHT:
                snake[0].x += SIZE;
                break;
        }

        SDL_Rect snake_head = {snake[0].x, snake[0].y, SIZE, SIZE};
        SDL_Rect food = {apple.x, apple.y, SIZE, SIZE};

        if (SDL_HasIntersection(&snake_head, &food))
        {
            apple.x = ((rand() % WIDTH / SIZE - 1) + 1) * SIZE;
            apple.y = ((rand() % HEIGHT / SIZE - 1) + 1) * SIZE;
            snake.push_back({snake[snake.size() - 1].x, snake[snake.size() - 1].y});
        }

        if (snake[0].x > WIDTH) snake[0].x = 0;
        if (snake[0].y > HEIGHT) snake[0].y = 0;
        if (snake[0].x < 0) snake[0].x = WIDTH;
        if (snake[0].y < 0) snake[0].y = HEIGHT;

        draw(renderer, snake, apple);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}