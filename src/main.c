#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(void)
{
    ///////////////////////////////////////////////////////////////////////////
    // Game State
    const int screen_width = 1080;
    const int screen_height = 720;
    const int play_start_height = 50;

    bool paused = false;
    int player_left_score = 0;
    int player_right_score = 0;

    Rectangle player_left_paddle = {
        .x = 0.05f * screen_width,
        .y = 0.40f * screen_height,
        .width = 0.01f * screen_width,
        .height = 0.2f * screen_height,
    };

    Rectangle player_right_paddle = {
        .x = 0.95f * screen_width,
        .y = 0.40f * screen_height,
        .width = 0.01f * screen_width,
        .height = 0.2f * screen_height,
    };

    Vector2 ball_position = {
        .x = 0.5 * screen_width,
        .y = 0.5 * screen_height,
    };
    float ball_radius = screen_height * 0.015f;

    Vector2 ball_velocity = {
        .x = -300.f,
        .y = 0.0f,
    };

    ///////////////////////////////////////////////////////////////////////////
    // Running the Game
    InitWindow(screen_width, screen_height, "Pong");
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
            // handle key presses from players
            const float paddle_mod = 10.f;
            if (IsKeyDown(KEY_W))
            {
                player_left_paddle.y =
                    MAX(play_start_height, player_left_paddle.y - paddle_mod);
            }

            if (IsKeyDown(KEY_S))
            {
                player_left_paddle.y =
                    MIN(screen_height - player_left_paddle.height,
                        player_left_paddle.y + paddle_mod);
            }

            if (IsKeyDown(KEY_UP))
            {
                player_right_paddle.y =
                    MAX(play_start_height, player_right_paddle.y - paddle_mod);
            }

            if (IsKeyDown(KEY_DOWN))
            {
                player_right_paddle.y =
                    MIN(screen_height - player_right_paddle.height,
                        player_right_paddle.y + paddle_mod);
            }

            const float dt = GetFrameTime();
            ball_position.x += ball_velocity.x * dt;
            ball_position.y += ball_velocity.y * dt;

            // handle collisions
            if (ball_velocity.x < 0)
            {
                if (CheckCollisionCircleRec(ball_position, ball_radius,
                                            player_left_paddle))
                {
                    float hit_pos = (ball_position.y - player_left_paddle.y) /
                                    player_left_paddle.height;
                    hit_pos = Clamp(hit_pos, 0.f, 1.f);

                    const float a = (hit_pos - 0.5f) * PI / 4;
                    const float speed = Vector2Length(ball_velocity) * 1.05f;
                    ball_velocity.x = speed * cosf(a);
                    ball_velocity.y = speed * sinf(a);
                }
            }
            else
            {
                if (CheckCollisionCircleRec(ball_position, ball_radius,
                                            player_right_paddle))
                {
                    float hit_pos = (ball_position.y - player_right_paddle.y) /
                                    player_right_paddle.height;
                    hit_pos = Clamp(hit_pos, 0.f, 1.f);

                    const float a = PI - (hit_pos - 0.5f) * PI / 4;
                    const float speed = Vector2Length(ball_velocity) * 1.05f;
                    ball_velocity.x = speed * cosf(a);
                    ball_velocity.y = speed * sinf(a);
                }
            }

            // collision with top and bottom wall
            if (ball_velocity.y > 0)
            {
                if (ball_position.y >= screen_height - ball_radius)
                {
                    ball_velocity.y *= -1;
                    ball_position.y = screen_height - ball_radius;
                }
            }
            else
            {
                if (ball_position.y <= play_start_height + ball_radius)
                {
                    ball_velocity.y *= -1;
                    ball_position.y = play_start_height + ball_radius;
                }
            }

            // handle scoring
            if (ball_position.x < 0)
            {
                ++player_right_score;

                ball_velocity.x = -300.f;
                ball_velocity.y = 0.f;
                ball_position.x = 0.5f * screen_width;
                ball_position.y = 0.5f * screen_height;
            }
            else if (ball_position.x > screen_width)
            {
                ++player_left_score;

                ball_velocity.x = 300.f;
                ball_velocity.y = 0.f;
                ball_position.x = 0.5f * screen_width;
                ball_position.y = 0.5f * screen_height;
            }
        }

        ///////////////////////////////////////////////////////////////////////
        //// Render
        BeginDrawing();
        ClearBackground(BLACK);

        // render game
        DrawRectangleRec(player_left_paddle, WHITE);
        DrawRectangleRec(player_right_paddle, WHITE);
        DrawCircleV(ball_position, ball_radius, WHITE);

        // render header
        const int header_y = 17;
        const int font_size = 30;
        const char *title = "Pong";
        const int title_width = MeasureText(title, font_size);
        DrawText("Pong", (screen_width - title_width) / 2, header_y, font_size,
                 WHITE);

        char buffer[4];
        sprintf(buffer, "%d", player_left_score);
        DrawText(buffer, (int)(0.1f * screen_width), header_y, font_size,
                 WHITE);

        sprintf(buffer, "%d", player_right_score);
        DrawText(buffer, (int)(0.9f * screen_width), header_y, font_size,
                 WHITE);

        DrawLine(0, play_start_height, screen_width, play_start_height, WHITE);

        // render pause, if necessary
        if (paused)
        {
            const int paused_font_size = 40;
            const char *pause_text = "Paused";
            const int pause_width = MeasureText(pause_text, paused_font_size);
            const int p_x = (screen_width - pause_width) / 2;

            const Rectangle rec = {
                .x = p_x - 10,
                .y = screen_height / 2 - 30,
                .width = pause_width + 20,
                .height = 60,
            };

            DrawRectangleRec(rec, WHITE);
            DrawText(pause_text, p_x, screen_height / 2 - 20, paused_font_size,
                     BLACK);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
