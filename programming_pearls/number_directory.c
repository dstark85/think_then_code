#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXNAME     20

/* 
** Programming Pearls chapter 2 problem 6
** Count how many names would have the same number via a "user operated directory assistance".
** [ABC] -> 2 [DEF] -> 3 [GHI] -> 4 [JKL] -> 5 [MNO] -> 6 [PQRS] -> 7 [TUV] -> 8 [WXYZ] -> 9 [OPERATOR] -> 0
** Usage: ./prog [Name file]
** Approaches: 
**  1) Write the signatures to STDOUT and then use system utilities to count the repeat values (sort + uniq)
    2) Store the signatures in RAM and then sort. Linearly scan through counting the repeat.
    The first is probably more robust. RAM would run out well before disk. However, it is certainly the slower of the two.
*/

/* Using the mapping most phones go by, convert names into a sequence of numbers */
int
str_to_signature(const char *s, char buf[]);

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Please provide a file to read\n");
        return EXIT_FAILURE;
    }

    char buf[MAXNAME], current[MAXNAME];
    FILE *infile;

    infile = fopen(argv[1], "r");
       
    if (infile == NULL) {
        fprintf (stderr, "Error reading file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Convert each name to its signature and write out to a temp file */
    while (fscanf(infile, "%s", current) != EOF) {
        str_to_signature(current, buf); 
        printf("%s\n", buf);
    }
    
    fclose(infile);
}

int
str_to_signature(const char *s, char buf[])
{
    char ch;
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        ch = tolower(s[i]); 
        switch (ch) {
            case 'a': case 'b': case 'c':           buf[i] = '2';   break;
            case 'd': case 'e': case 'f':           buf[i] = '3';   break;
            case 'g': case 'h': case 'i':           buf[i] = '4';   break;
            case 'j': case 'k': case 'l':           buf[i] = '5';   break;
            case 'm': case 'n': case 'o':           buf[i] = '6';   break;
            case 'p': case 'q': case 'r': case 's': buf[i] = '7';   break;
            case 't': case 'u': case 'v':           buf[i] = '8';   break;
            case 'w': case 'x': case 'y': case 'z': buf[i] = '9';   break;
            default:                                buf[i] = '?';
        }
    } 
    buf[i] = '\0';
    return 0;
}
