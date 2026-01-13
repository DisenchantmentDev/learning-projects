#include "../raylib/src/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAY_SIZE 500
#define CELL_SIZE 20.0
#define MAX_BODY 625

#define da_append(xs, x)                                                      \
    do                                                                        \
        {                                                                     \
            if ((xs)->count >= (xs)->capacity)                                \
                {                                                             \
                    if ((xs)->capacity == 0)                                  \
                        (xs)->capacity = 256;                                 \
                    else                                                      \
                        (xs)->capacity *= 2;                                  \
                    (xs)->items = realloc (                                   \
                        (xs)->items, (xs)->capacity * sizeof (*(xs)->items)); \
                }                                                             \
            (xs)->items[(xs)->count++] = (x);                                 \
        }                                                                     \
    while (0)

typedef struct
{
    Vector2 coords[MAX_BODY]; // A list of coords RELATIVE TO HEAD POSITION
    size_t count;
    size_t capacity;
    size_t front;
    size_t rear;
} SnakeBody;

typedef struct
{
    Vector2 head_pos;
    Vector2 body_segment_size;
    int num_body_segments; // number of body segments that isn't the head;
    SnakeBody body;
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
    while (!game->apple.is_present)
        {
            int new_x = rand () % PLAY_SIZE + 1;
            int new_y = rand () % PLAY_SIZE + 1;
            /* snap to the nearest coordinate, rounded down always */
            game->apple.pos.x = (int)(new_x / 20) * CELL_SIZE;
            game->apple.pos.y = (int)(new_y / 20) * CELL_SIZE;
            if (game->apple.pos.x < 500 && game->apple.pos.y < 500
                && game->apple.pos.x >= 0 && game->apple.pos.y >= 0)
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

/* Functions for Body Updating */
bool
is_body_full (Game_Ctx *game)
{
    return (
        (game->snake.body.front == 0 && game->snake.body.rear == MAX_BODY - 1)
        || (game->snake.body.front == game->snake.body.rear + 1));
}

bool
is_body_empty (Game_Ctx *game)
{
    return (game->snake.body.front == -1);
}

/* Insert element to the front of the deque
 * Called when snake eats an apple
 * Inserts an element behind the head of the snake*/
void
insert_front (Game_Ctx *game, Vector2 key)
{
    if (is_body_full (game))
        {
            // stop game
            return;
        }

    if (game->snake.body.front == -1)
        {
            game->snake.body.front = 0;
            game->snake.body.rear = 0;
        }
    else if (game->snake.body.front == 0)
        {
            game->snake.body.front = MAX_BODY - 1;
        }
    else
        {
            game->snake.body.front = game->snake.body.front - 1;
        }

    game->snake.body.coords[game->snake.body.front] = key;
    game->snake.body.count++;
    // printf ("Inserted item to front\n");
}

void
delete_rear (Game_Ctx *game)
{
    if (is_body_empty (game))
        return;

    if (game->snake.body.front == game->snake.body.rear)
        {
            game->snake.body.front = -1;
            game->snake.body.rear = -1;
        }
    else if (game->snake.body.rear == 0)
        {
            game->snake.body.rear = MAX_BODY - 1;
        }
    else
        {
            game->snake.body.rear = game->snake.body.rear - 1;
        }
    game->snake.body.count--;
    // printf ("deleted from back\n");
}

void
update_body (Game_Ctx *game)
{
    insert_front (game, game->snake.head_pos);

    if (game->apple.is_present)
        {
            delete_rear (game);
        }
}

void
draw_snake (Game_Ctx *game)
{
    /* Draw Snake Head */
    DrawRectangleV (game->snake.head_pos, game->snake.body_segment_size,
                    GREEN);

    int i = game->snake.body.front;

    while (1)
        {
            if (is_body_empty (game))
                break;

            DrawRectangleV (game->snake.body.coords[i],
                            game->snake.body_segment_size, GREEN);
            if (i == game->snake.body.rear)
                break;

            i = (i + 1) % MAX_BODY;
        }
}

void
initialize_game (Game_Ctx *game)
{
}

int
main (void)
{
    srand (time (NULL));
    const int screenWidth = 500;
    const int screenHeight = 500;

    Apple apple = { { 0, 0 }, false };
    SnakeBody snake_body = { 0 };
    snake_body.front = -1;
    snake_body.rear = -1;
    Snake snake = { { 200, 200 }, { CELL_SIZE, CELL_SIZE }, 1, snake_body };
    Game_Ctx game = { 0, 0, snake, apple };
    insert_front (&game, (Vector2){ 200, 200 });

    // initialize_game (&game);

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
                    // update_body (&game);
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

            draw_snake (&game);

            // DrawRectangleV (game.snake.head_pos,
            // game.snake.body_segment_size,
            //                 GREEN);

            EndDrawing (); /* DRAWING END */

            if (has_eaten (&game))
                game.apple.is_present = false;

            if (game.frame_count == 0 || game.frame_count == 30)
                update_body (&game);
        }

    CloseWindow ();

    return 0;
}
