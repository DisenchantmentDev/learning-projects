#include <stdio.h>
#include <stdlib.h>

int
read_file ()
{

    FILE *fptr;
    fptr = fopen ("../resources/input_1.txt", "r");

    if (fptr == NULL)
        {
            printf ("Unable to read file.");
            return -1;
        }

    return 0;
}

int
main ()
{
    return 0;
}
