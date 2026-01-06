#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *items;
    size_t count;
    size_t capacity;
} Bank;

typedef struct
{
    Bank *items;
    size_t count;
    size_t capacity;
} BankArray;

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

#define MAX_LINE 101

int
read_file (BankArray *b_array)
{
    FILE *fptr;
    fptr = fopen ("../resources/input_3.txt", "r");

    if (fptr == NULL)
        return -1;

    char line[MAX_LINE];

    while (fgets (line, sizeof (line), fptr))
        {
            Bank b = { 0 };
            // printf ("%s", line);
            for (size_t i = 0; i < MAX_LINE; ++i)
                {
                    da_append (&b, line[i]);
                }
            da_append (b_array, b);
        }

    return 1;
}

int
main ()
{
    /* - Puzzle:
     * - Each line of digits is a bank of batteries
     * - Each digit is a respective battery joltage
     * - The total joltage of a bank is the concatination of battery joltages
     * you have turned on
     * - Task is to find the largest possible joltage each bank can produce
     * with just two batteries*/
    BankArray bank = { 0 };
    if (read_file (&bank) == -1)
        return -1;

    for (size_t i = 0; i < bank.count; ++i)
        {
            printf ("%s", bank.items[i].items);
        }

    return 0;
}
