/* INCOMPLETE */
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

#define MAX_LINE 102

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

    fclose (fptr);

    return 1;
}

int
find_max_n (char *items, size_t n)
{
    char out[n];
    int indexes[n];
    size_t count = 0;

    for (char i = '9'; i > '0'; --i)
        {
            for (int j = 100; j > 0; --j)
                {
                    if (items[j] == i && count < n)
                        {
                            count++;
                            out[n - count] = i;
                            indexes[n - count] = j;
                        }
                }
            if (count == n)
                break;
        }
    int i, j, imin;
    for (i = 0; i < n; ++i)
        {

            for (j = i + 1; j < n; ++j)
                {
                    if (indexes[j] < indexes[imin])
                        imin = j;
                }

            out[j] ^= out[imin];
            out[imin] ^= out[j];
            out[j] ^= out[imin];

            indexes[j] ^= indexes[imin];
            indexes[imin] ^= indexes[j];
            indexes[j] ^= indexes[imin];
        }

    return atoi (out);
}

int
find_max (char *items)
{
    /* find the indexes of the highest two digits
     * store those indexes in size_t variables
     * then get the digits those indexes point to
     * put them into a two character long *char
     * parse that char into an int with atoi and return*/
    char a = '0';
    char b = '0';
    size_t ai = 0;
    size_t bi = 0;
    char max = '0';
    char out[2];

    for (size_t i = 100; i > 0; --i)
        {
            if (items[i] > items[ai])
                {
                    if (items[i] > items[bi])
                        {
                            bi = i;
                            continue;
                        }
                    ai = i;
                }
        }

    if (ai < bi)
        {
            out[0] = items[ai];
            out[1] = items[bi];
        }
    else
        {
            out[1] = items[ai];
            out[0] = items[bi];
        }
    return atoi (out);

    // for (size_t i = 0; i < 100; ++i)
    //     {
    //         char temp = items[i];
    //         if (temp > a)
    //             {
    //                 if (temp > b)
    //                     {
    //                         b = temp;
    //                         bi = i;
    //                         continue;
    //                     }
    //                 a = temp;
    //             }
    //         if (temp == a && i > bi && b > temp)
    //             {
    //                 a ^= b;
    //                 b ^= a;
    //                 a ^= b;
    //             }
    //     }

    // for (char *p = items; *p; ++p)
    //     {
    //         if (*p > a)
    //             {
    //                 if (*p > b)
    //                     {
    //                         b = *p;
    //                         continue;
    //                     }
    //                 a = *p;
    //             }
    //         else if (*p == a && b > a)
    //             {
    //                 // swap
    //                 a ^= b;
    //                 b ^= a;
    //                 a ^= b;
    //             }
    //     }

    // out[0] = a;
    // out[1] = b;
    // return atoi (out);
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

    int total = 0;
    for (size_t i = 0; i < bank.count; ++i)
        {
            char *items = bank.items[i].items;
            int max = find_max_n (items, 2);
            total += max;
            printf ("Line joltage: %d\n", max);
            printf ("Current running total: %d\n", total);
            // printf ("%s", bank.items[i].items);
            // printf ("%s Total: %d\n", items, find_max (items));
        }
    printf ("Total joltage: %d\n", total);
    return 0;
}
