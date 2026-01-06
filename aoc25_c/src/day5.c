#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    long start;
    long end;
} Range;

typedef struct
{
    Range *items;
    size_t count;
    size_t capacity;
} RangeArray;

typedef struct
{
    long *items;
    size_t count;
    size_t capacity;
} Inventory;

typedef struct
{
    RangeArray ranges;
    Inventory inv;
} FoodData;

/* type agnostic array append */
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

#define MAX_LINE 128

int
read_file (RangeArray *ranges, Inventory *inv)
{
    FILE *fptr;
    fptr = fopen ("../resources/input_5.txt", "r");

    if (fptr == NULL)
        {
            printf ("Unable to read file.");
            return -1;
        }

    char line[MAX_LINE];

    while (fgets (line, sizeof (line), fptr))
        {
            long a, b;
            Range r;

            if (sscanf (line, "%ld-%ld", &r.start, &r.end) == 2)
                {
                    da_append (ranges, r);
                }
            else if (sscanf (line, "%ld", &a) == 1)
                {
                    da_append (inv, a);
                }
        }

    fclose (fptr);

    return 1;
}

int
sort_ranges (RangeArray *ranges)
{
    // Selection sort of ranges based on first key
    int i, j, imin;
    for (i = 0; i < ranges->count; ++i)
        {
            imin = i;
            for (j = i + 1; j < ranges->count; ++j)
                {
                    if (ranges->items[j].start < ranges->items[imin].start)
                        imin = j;
                }

            Range temp;
            temp = ranges->items[i];
            ranges->items[i] = ranges->items[imin];
            ranges->items[imin] = temp;
        }
    return 0;
}

int
main ()
{
    /* Part 1 Solution: */
    RangeArray ranges = { 0 };
    Inventory inv = { 0 };
    if (read_file (&ranges, &inv) == -1)
        {
            return 1;
        }
    int total = 0;
    for (size_t i = 0; i < inv.count; ++i)
        {
            long item = inv.items[i];
            for (size_t j = 0; j < ranges.count; ++j)
                {
                    long a = ranges.items[j].start;
                    long b = ranges.items[j].end;
                    if (item >= a && item <= b)
                        {
                            ++total;
                            break;
                        }
                }
        }

    printf ("Total number of fresh items is: %d\n", total);

    /* Part 2 Solution: */

    sort_ranges (&ranges);

    long total_inv = 0;
    long running_start = ranges.items[0].start;
    long running_end = ranges.items[0].end;

    for (size_t i = 1; i < ranges.count; ++i)
        {
            Range r = ranges.items[i];
            if (r.start <= running_end)
                {
                    if (r.end > running_end)
                        running_end = r.end;
                }
            else
                {
                    total_inv = total_inv + (running_end - running_start + 1);
                    running_start = r.start;
                    running_end = r.end;
                }
        }

    total_inv = total_inv + (running_end - running_start + 1);
    printf ("Total number of valid ids: %ld\n", total_inv);

    free (ranges.items);
    free (inv.items);

    return 0;
}
