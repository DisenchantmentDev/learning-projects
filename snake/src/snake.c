#include "../raylib/src/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAY_SIZE 500
#define CELL_SIZE 20.0

typedef struct
{
    Vector2 head_pos;
    Vector2 body_segment_size;
    int num_body_segments;
    /* dynamic array that tracks body segments positions; deque?*/
} Snake;

typedef struct
{
    Vector2 pos;
    bool is_present;
} Apple;

typedef struct
{
    int frame_count;
    int cached_key;
    Snake snake;
    Apple apple;
} Game_Ctx;

void
gen_apple (Game_Ctx *game)
{
    if (!game->apple.is_present)
        {
            int new_x = rand () % PLAY_SIZE + 1;
            int new_y = rand () % PLAY_SIZE + 1;
            /* snap to the nearest coordinate, rounded down always */
            game->apple.pos.x = (int)(new_x / 20) * CELL_SIZE;
            game->apple.pos.y = (int)(new_y / 20) * CELL_SIZE;
            game->apple.is_present = true;
        }
}

bool
has_eaten (Game_Ctx *game)
{
    if (game->snake.head_pos.x == game->apple.pos.x
        && game->snake.head_pos.y == game->apple.pos.y)
        {
            return true;
        }

    return false;
}

void
move_snake (Game_Ctx *game)
{
    if (game->cached_key == KEY_UP)
        game->snake.head_pos.y -= CELL_SIZE;
    if (game->cached_key == KEY_DOWN)
        game->snake.head_pos.y += CELL_SIZE;
    if (game->cached_key == KEY_LEFT)
        game->snake.head_pos.x -= CELL_SIZE;
    if (game->cached_key == KEY_RIGHT)
        game->snake.head_pos.x += CELL_SIZE;

    /* check for illegal coordinates and wrap accordingly */
    if (game->snake.head_pos.x >= PLAY_SIZE)
        game->snake.head_pos.x = 0;
    if (game->snake.head_pos.x < 0)
        game->snake.head_pos.x += 500;

    if (game->snake.head_pos.y >= PLAY_SIZE)
        game->snake.head_pos.y = 0;
    if (game->snake.head_pos.y < 0)
        game->snake.head_pos.y += PLAY_SIZE;
}

void
draw_snake (Game_Ctx *game)
{
    if (game->apple.is_present)
        {
            /* draw background-colored rect at tail's location if an apple was
             * not eaten last turn*/
        }

    DrawRectangleV (game->snake.head_pos, game->snake.body_segment_size,
                    GREEN);
}

int
main (void)
{
    srand (time (NULL));
    const int screenWidth = 500;
    const int screenHeight = 500;

    Apple apple = { { 0, 0 }, false };
    Snake snake = { { 200, 200 }, { CELL_SIZE, CELL_SIZE }, 1 };
    Game_Ctx game = { 0, 0, snake, apple };

    InitWindow (PLAY_SIZE, PLAY_SIZE, "Snake!");
    SetTargetFPS (60);

    while (!WindowShouldClose ())
        {
            /* Every frame, we check how many frames have passed
             * For every frame, we check what the most recently input key was
             * if, say, 30 frames have passed, we check the chached key input
             * we apply that to snake input, check of wrapping, and continue
             * from there
             * After we draw, we check if we are on top of an apple, and if
             * we are then we extend body on next move
             * Make sure we draw apple first, and then snake, so if snake moves
             * over apple that take priority*/

            if (!game.apple.is_present)
                gen_apple (&game);

            game.frame_count = (game.frame_count + 1) % 60;
            if (game.frame_count == 0 || game.frame_count == 30)
                {
                    move_snake (&game);
                }

            if (IsKeyPressed (KEY_UP))
                game.cached_key = KEY_UP;
            if (IsKeyPressed (KEY_DOWN))
                game.cached_key = KEY_DOWN;
            if (IsKeyPressed (KEY_LEFT))
                game.cached_key = KEY_LEFT;
            if (IsKeyPressed (KEY_RIGHT))
                game.cached_key = KEY_RIGHT;

            char buff[100];
            sprintf (buff, "Current position: %.2f, %2.f",
                     game.snake.head_pos.x, game.snake.head_pos.y);

            BeginDrawing (); /* DRAWING BEGIN */

            ClearBackground (BLACK); // Might not even need honestly

            DrawText (buff, 10, 10, 10, WHITE); // Debugging text

            DrawRectangleV (game.apple.pos, game.snake.body_segment_size, RED);

            DrawRectangleV (game.snake.head_pos, game.snake.body_segment_size,
                            GREEN);

            EndDrawing (); /* DRAWING END */

            if (has_eaten (&game))
                game.apple.is_present = false;
        }

    CloseWindow ();

    return 0;
}
