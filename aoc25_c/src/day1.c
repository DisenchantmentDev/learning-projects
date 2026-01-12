#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 6
#define NUM_LINES 4659

typedef struct
{
    char items[NUM_LINES][MAX_LINE];
    size_t count;
} Combination;

int
read_file (Combination *combo)
{
    FILE *fptr;
    fptr = fopen ("../resources/input_1.txt", "r");

    if (fptr == NULL)
        {
            printf ("Unable to read file.\n");
            return -1;
        }

    char line[MAX_LINE];
    size_t count = 0;

    while (fgets (line, sizeof (line), fptr) != NULL)
        {
            // da_append (combo, line);
            snprintf (combo->items[count], MAX_LINE, "%s", line);
            // printf ("Current line: %s", combo->items[count]);
            ++count;
            // printf ("Line appended: %s", combo->items[combo->count - 1]);
        }

    combo->count = count;
    fclose (fptr);
    // printf ("finished reading file");

    return 0;
}

int
part1 (Combination *combo)
{
    /* COMPLETE */
    int count = 0;
    int dial = 50;

    for (size_t i = 0; i < combo->count; ++i)
        {
            char direction = combo->items[i][0];

            char val[4];
            size_t k = 0;
            for (size_t j = 1; combo->items[i][j] && j < MAX_LINE; ++j)
                {
                    val[k++] = combo->items[i][j];
                }
            val[k] = '\0';

            int n = atoi (val);
            dial += (direction == 'R') ? n : -n;
            dial = ((dial % 100) + 100) % 100;
            if (dial == 0)
                ++count;
        }
    return count;
}

int
mod (int value, int divisor)
{
    return value - (divisor * (int)(value / divisor));
}

int
part2 (Combination *combo)
{
    /* WIP */
    int count = 0;
    int dial = 50;

    for (size_t i = 0; i < combo->count; ++i)
        {
            char direction = combo->items[i][0];

            char val[4];
            size_t k = 0;
            for (size_t j = 1; combo->items[i][j] && j < MAX_LINE; ++j)
                {
                    val[k++] = combo->items[i][j];
                }
            val[k] = '\0';

            int n = atoi (val);
            // int rotations = n / 100;
            int temp = dial;
            dial += (direction == 'R') ? n : -n;
            dial = mod (dial, 100);
            if (direction == 'R')
                {
                    int divisions = n / 100;
                    count += divisions;
                    int m = mod (n, 100);
                    if (temp + m >= 100)
                        count++;
                }
            else if (direction == 'L')
                {
                    int divisions = -n / -100;
                    count += divisions;
                    int m = mod (n, -100);
                    if (temp != 0 && temp - m <= 0)
                        count++;
                }
            if (i < 110)
                printf ("Starting Pos: %d\n\tInstruction: %s\tNew Pos (raw): "
                        "%d\n\tCount: %d\n",
                        temp, combo->items[i], dial, count);

            // int n = atoi (val);
            // int rotations = n / 100;
            // int distance = 100 - dial;
            // if (direction == 'R')
            //     {
            //         if (n >= distance)
            //             count += 1 + rotations;
            //         dial += n;
            //     }
            // else if (direction == 'L')
            //     {
            //         if (n <= distance)
            //             count += 1 + rotations;
            //         dial += -n;
            //     }
            // dial = ((dial % 100) + 100) % 100;
            // if (dial == 0)
            //     ++count;
        }
    return count;
}

int
main ()
{
    /* Problem:
     * We have a rotating lock of 99 numbers, 0-99
     * The dial starts at 50
     * The file provides us with movements to the dial
     * Left or Right, and then a number to move in that direction.
     * Each time that dial is exactly 0 is the answer.
     *
     * Solution idea:
     * Part 1 at least is doable entirely in read_file, but we can read out
     * into an array Might just return that
     * */

    Combination combo = { 0 };

    if (read_file (&combo) == -1)
        {
            printf ("There was an error reading file");
            return -1;
        }

    // char *val;
    // for (size_t i = 0; i < NUM_LINES; ++i)
    //     {
    //         val = combo.items[i];
    //         printf ("%s", val);
    //     }

    printf ("Solution to part 1: %d\n", part1 (&combo));
    printf ("Solution to part 2: %d\n", part2 (&combo));

    return 0;
}
