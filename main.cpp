#include <raylib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

const int WIDTH = 640;
const int HEIGHT = 480;

enum Scene
{
    MENU,
    PLAY
};

struct Snake
{
    Vector2 pos;

    Snake(float x, float y)
    {
        this->pos = Vector2(x, y);
    }
};

int main()
{
    InitWindow(WIDTH, HEIGHT, "SnaKEKW");
    SetTargetFPS(60);

    // grid
    int cellSize = 20;
    std::vector<Vector2> gridPos;
    for (int row = 0; row < WIDTH / cellSize; row++)
    {
        for (int col = 0; col < HEIGHT / cellSize; col++)
        {
            gridPos.push_back(Vector2(col * cellSize, row * cellSize));
        }
    }

    // init random generator
    srand(time(0));
    // food
    Vector2 food;
    food.x = gridPos[0 + rand() % WIDTH].x;
    food.y = gridPos[0 + rand() % HEIGHT].y;

    // snake
    std::vector<Snake *> snakes;
    Vector2 dir = Vector2(1, 0);
    snakes.push_back(new Snake(WIDTH / 2, HEIGHT / 2));
    snakes.push_back(new Snake(WIDTH / 2, HEIGHT / 2));

    int score = 0;
    float lastUpdate = GetTime();
    bool dead = false;
    Scene currentScene = MENU;

    while (!WindowShouldClose())
    {

        ClearBackground(BLACK);
        BeginDrawing();

        switch (currentScene)
        {
        case MENU:
            DrawText("SnaKEKW", GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 4.0f, 30, YELLOW);
            DrawText("Tekan \"Space\" untuk restart", 170, GetScreenHeight() / 2.0f, 20, WHITE);
            DrawText("created by aji mustofa @pepega", 160, HEIGHT - 30, 20, WHITE);

            if (IsKeyDown(KEY_SPACE))
            {
                currentScene = PLAY;
            }

            break;
        case PLAY:
            if (IsKeyDown(KEY_UP) && dir.y != 1)
            {
                dir = Vector2(0, -1);
            }
            else if (IsKeyDown(KEY_RIGHT) && dir.x != -1)
            {
                dir = Vector2(1, 0);
            }
            else if (IsKeyDown(KEY_DOWN) && dir.y != -1)
            {
                dir = Vector2(0, 1);
            }
            else if (IsKeyDown(KEY_LEFT) && dir.x != 1)
            {
                dir = Vector2(-1, 0);
            }

            if (IsKeyDown(KEY_R) && dead)
            {
                snakes.clear();
                snakes.push_back(new Snake(WIDTH / 2, HEIGHT / 2));
                snakes.push_back(new Snake(WIDTH / 2, HEIGHT / 2));
                score = 0;
                dir = Vector2(1, 0);
                food.x = gridPos[0 + rand() % WIDTH].x;
                food.y = gridPos[0 + rand() % HEIGHT].y;
                dead = false;
            }

            if (GetTime() - lastUpdate > 0.1 && !dead)
            {
                // moving snake
                Snake *newH = new Snake(snakes.front()->pos.x + dir.x * 20., snakes.front()->pos.y + dir.y * 20.);
                snakes.insert(snakes.begin(), newH);

                if (snakes.front()->pos.x == food.x && snakes.front()->pos.y == food.y)
                {
                    food.x = gridPos[0 + rand() % WIDTH].x;
                    food.y = gridPos[0 + rand() % HEIGHT].y;
                    score += 1;
                }
                else
                    snakes.pop_back();

                // check collision snake dengan dirinya sendiri
                for (int i = 1; i < snakes.size(); i++)
                {
                    if (newH->pos.x == snakes.at(i)->pos.x && newH->pos.y == snakes.at(i)->pos.y && !dead)
                        dead = true;
                }

                // check collision snake dengan wall
                if (snakes.front()->pos.x > WIDTH - cellSize || snakes.front()->pos.x < 0)
                    // gameOver = true;
                    dead = true;

                if (snakes.front()->pos.y > HEIGHT - cellSize || snakes.front()->pos.y < 0)
                    // gameOver = true;
                    dead = true;

                lastUpdate = GetTime();
            }

            // draw food
            DrawRectangle(food.x, food.y, 20, 20, ORANGE);

            // draw snake
            for (int i = 0; i < snakes.size(); i++)
            {
                // DrawCircleV(snakes[i]->pos, 10., RED);
                DrawRectangle(snakes[i]->pos.x, snakes[i]->pos.y, 20, 20, RED);
                DrawRectangleLines(snakes[i]->pos.x, snakes[i]->pos.y, 20, 20, WHITE);
            }

            // draw score
            std::string scoreText = "Score = " + std::to_string(score);
            DrawText(scoreText.c_str(), 10, 10, 20, WHITE);

            // draw game over text
            if (dead)
            {
                DrawText("Game Over", GetScreenWidth() / 2.0f - 75, GetScreenHeight() / 4.0f, 30, WHITE);
                DrawText("Tekan \"R\" untuk restart", 200, GetScreenHeight() / 2.0f, 20, WHITE);
            }
            break;
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}