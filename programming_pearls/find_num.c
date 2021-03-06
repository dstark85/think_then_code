#include <stdio.h>
#include <limits.h>

#define SQUARE      65536
#define MISSING     0
#define DUPLICATE   1

/* 
    ** Program to either find a missing number or a duplicate number in a sea of numbers (approximately 4 billion) efficiently.
    ** Usage: ./prog NUM_FILE [-]
    ** Default use is to search for a missing number. Simply provide the dash to look for a duplicate instead.

        Implementation details
    ** Using 65536 (2^16) for the ranges. This cuts the number of file reads down to 2 (2^16 buckets w/capacity > 2^16)
*/


int
find_num(const char *file, int duplicate);

int
main(int argc, char *argv[])
{
    if (argc < 2) 
        printf("No file provided\n");
    else if(argc == 2)
        printf("A missing number is: %d\n", find_num(argv[1], MISSING));
    else if (argc == 3 && argv[2][0] == '-')
        printf("A duplicate number is %d\n", find_num(argv[1], DUPLICATE));
    else
        printf("No file provided\n");

}

/* Uses a range concept to efficiently find a missing (or duplicate) number in a file with ~4 billion 32 bit numbers. */
int
find_num(const char *file, int duplicate)
{
    FILE *f;
    int lower, r, ranges[SQUARE], upper, val;
    unsigned offset = 2147483648;

    /* Initialize array counts to 0 */
    for(int i = 0; i < SQUARE; i++)
        ranges[i] = 0;
    
    /* Open and read the file */
    f = fopen(file, "r");

    if(f == NULL)
        return -1;


    /* Populate range buckets */
    while((fscanf(f, "%d", &val)) != EOF)
        ranges[(val + offset) >> 16] += 1;           // Divide by 2^16

    fclose(f);

    /* Find the range without all numbers. If duplicate, find the range exceeding count */
    for (r = 0; r < SQUARE; r++)
        if ((duplicate && ranges[r] > SQUARE) || (!duplicate && ranges[r] < SQUARE))
            break;
            
    lower = (r << 16) + INT_MIN;
    upper = ((r + 1) << 16) + INT_MIN;
    
    /* Open and read the file, again */
    f = fopen(file, "r");

    /* Re-initialize ranges */
    for (int i = 0; i < SQUARE; i++)
        ranges[i] = 0;

    /* Populate number buckets */
    while (fscanf(f, "%d", &val) != EOF)
        if (val >= lower && val < upper)
            ranges[val - lower] += 1;

    /* Find the missing/duplicate number */
    for (r = 0; r < SQUARE; r++)
        if ((duplicate && ranges[r] > 1) || (!duplicate && ranges[r] == 0))
            return lower + r;

    fclose(f);

    return 42;
}
