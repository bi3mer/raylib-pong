#include "raylib.h"
#include <stdio.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(void)
{
    const int width = 1080;
    const int height = 720;
    const int play_height = 50;

    ///////////////////////////////////////////////////////////////////////////
    // Game State
    bool paused = false;
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
        .x = -4.0f,
        .y = 0.0f,
    };

    ///////////////////////////////////////////////////////////////////////////
    // Game Loop
    InitWindow(width, height, "Pong");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        ///////////////////////////////////////////////////////////////////////
        // Update
        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }

        if (!paused)
        {
            if (IsKeyDown(KEY_W))
            {
                paddle_left.y = MAX(play_height, paddle_left.y - 10);
            }
            if (IsKeyDown(KEY_S))
            {
                paddle_left.y =
                    MIN(height - paddle_left.height, paddle_left.y + 10);
            }

            if (IsKeyDown(KEY_UP))
            {
                paddle_right.y = MAX(play_height, paddle_right.y - 10);
            }
            if (IsKeyDown(KEY_DOWN))
            {
                paddle_right.y =
                    MIN(height - paddle_left.height, paddle_right.y + 10);
            }

            if (ball_velocity.y > 0)
            {
                if (ball_position.y >= height - ball_radius)
                {
                    ball_velocity.y *= -1;
                    ball_position.y = height - ball_radius;
                }
            }
            else
            {
                if (ball_position.y <= play_height + ball_radius)
                {
                    ball_velocity.y *= -1;
                    ball_position.y = play_height + ball_radius;
                }
            }

            if (ball_position.x < 0)
            {
                ++player_right_score;

                ball_position.x = 0.5f * width;
                ball_position.y = 0.5f * height;
                ball_velocity.x = -4.0f;
                ball_velocity.y = 0.0f;
            }
            else if (ball_position.x > width)
            {
                ++player_left_score;

                ball_position.x = 0.5f * width;
                ball_position.y = 0.5f * height;
                ball_velocity.x = 4.0f;
                ball_velocity.y = 0.0f;
            }

            ball_position.x += ball_velocity.x;
            ball_position.y += ball_velocity.y;
        }

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

        // Render paused
        if (paused)
        {
            const int font_size = 40;
            const int text_width = MeasureText("Paused", font_size);
            const int start_x = (width - text_width) / 2;

            DrawRectangle(start_x - 10, 330, text_width + 20, 55, WHITE);
            DrawText("Paused", start_x, 340, font_size, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
