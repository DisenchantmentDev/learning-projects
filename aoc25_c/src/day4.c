#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAX_LINE 256
#define NUM_LINES 136

typedef struct
{
    char line[MAX_LINE];
} Line;

typedef struct
{
    size_t count;
    size_t capacity;
    char items[NUM_LINES][MAX_LINE];
} Lines;

int
read_file (Lines *lines)
{

    FILE *fptr;
    fptr = fopen ("../resources/day4.txt", "r");

    if (fptr == NULL)
        {
            printf ("Unable to read file.");
            return -1;
        }

    char l[MAX_LINE];
    size_t i = 0;
    while (fgets (l, MAX_LINE, fptr) != NULL)
        {
            // printf ("%s\n", l);
            strcpy (lines->items[i], l);
            printf ("Current line: %s\n", lines->items[i]);
            ++i;
            // da_append (lines, l);
        }

    fclose (fptr);
    return 1;
}

int
main ()
{
    Lines lines = { 0 };
    if (read_file (&lines) == -1)
        {
            return 1;
        }

    for (size_t i = 0; i < NUM_LINES; ++i)
        {
            printf ("%s", lines.items[i]);
        }
}
