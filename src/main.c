#include "raylib.h"
#include <stdio.h>

int main(void)
{
    const int width = 1080;
    const int height = 720;
    const int play_height = 50;

    ///////////////////////////////////////////////////////////////////////////
    // Game State
    int player_left_score = 0;
    int player_right_score = 0;

    Rectangle paddle_left = {
        .x = 0.03f * width,
        .y = 0.5f * height,
        .width = 0.01f * width,
        .height = 0.15f * height,
    };

    Rectangle paddle_right = {
        .x = 0.97f * width,
        .y = 0.5f * height,
        .width = 0.01f * width,
        .height = 0.15f * height,
    };

    float ball_radius = 10;
    Vector2 ball_position = {
        .x = 0.5f * width,
        .y = 0.5f * height,
    };
    Vector2 ball_velocity = {
        .x = 1.0f,
        .y = 0.0f,
    };

    ///////////////////////////////////////////////////////////////////////////
    // Game Loop
    InitWindow(width, height, "Raylib Template");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        ///////////////////////////////////////////////////////////////////////
        // Update

        ///////////////////////////////////////////////////////////////////////
        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        // Render Game
        DrawRectangleRec(paddle_left, WHITE);
        DrawRectangleRec(paddle_right, WHITE);
        DrawCircleV(ball_position, ball_radius, WHITE);

        // Render Header
        DrawLine(0, play_height, width, play_height, WHITE);

        char buffer[4];
        sprintf(buffer, "%d", player_left_score);
        DrawText(buffer, 100, 20, 20, WHITE);

        sprintf(buffer, "%d", player_right_score);
        DrawText(buffer, 980, 20, 20, WHITE);

        DrawText("Pong", 536, 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
