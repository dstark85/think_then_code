/*,
.................................. 
..................................
..................................
. ................................
.. ...............................
.... .............................
...... ...........................
.......... .......................
............ .....................
................ .................
.................. ...............
...................... ...........
............................ .....
>....>>....>>>>....>>>>>>>>... *..
....<<<<<<<<........++++++++......

[-]
[]

+
[
.... And .... so, .... the first ..
+<<<<<<<< ** BrainSplat ** >>>>>>>>
-,B.,r.,a.,i.,n.,S.,p.,l.,a.,t.,..,
B.,S.>.............................
......[][][][][>+++<][>>++<<][][[]]
..............splatted itself...+*]
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CELLCOUNT       1024
#define MAXJUMPS        16
#define MAXSOURCE       1024

/* Commands */
#define MOVRIGHT        '>'
#define MOVLEFT         '<'
#define INCREMENT       '+'
#define DECREMENT       '-'
#define OUTPUT          '.'
#define INPUT           ','
#define JMP             '['
#define JMPBACK         ']'
#define PEEK            '*'
#define SNAPSHOT        '$'

#define NEWLINE         '\n'

#define APERTURE        5

/*
** Interpreter for a language called BrainSplat.
** Extends the functionality of BrainFuck by adding the ability to see the contents of cells (*)
** Specification:
    >   ===  Move the pointer to the right
    <   ===  Move the pointer to the left
    +   ===  Increment the memory cell at the pointer
    -   ===  Decrement the memory cell at the pointer
    .   ===  Output the character signalled by the cell at the pointer
    ,   ===  Input the character signalled by the cell at the pointer
    [   ===  Jump past the matching ] if the cell at the pointer is 0
    ]   ===  Jump back to the matching [ if the cell at the pointer is nonzero
    *   ===  Print the actual (decimal) value of the memory cell at the pointer
    $   ===  Retrieve a $nap$sot of current working area

    Some examples in traditional BF:
    MOV https://copy.sh/brainfuck/?c=IyBNb3ZlIHZhbHVlIGZyb20gYSB0byBiCgojIGEgPSA0CisrKysKCiMgYSA9IDAsIGIgPSA0IGFuZCBjID0gNApbPis-Kzw8LV0KCj4-Cls8PCs-Pi1dCg$$

    TODO Features:
        * Add brainsplat file reading options (.bs or .BS extensions) via the command line
        * Prompt the user with more options post writing a BS program: save to a file, enter a new BS, etc.
        
*/  

typedef struct JMP_PAIR {
    int start, close;
} Jump_pair;

/* Capture cell information around the pointer */
void
snapshot(int cells[], int pointer);

int
main(int arc, char *argv[])
{
    bool close_jump;
    char source[MAXSOURCE];
    int cells[CELLCOUNT], ch, command, jc, loc, pointer, sc;
    Jump_pair jump_table[MAXJUMPS];

    /* Initialize cells */
    for (int i = 0; i < CELLCOUNT; i++)
        cells[i] = 0;

    /* Default data */
    close_jump = false;
    jc = 0;
    loc = 0;
    pointer = 0;
    sc = 0;

    /* Read the file in to ram */
    while ((ch = getchar()) != EOF && sc < MAXSOURCE)       /* Requires 2 EOF to halt (?) */
        source[sc++] = ch;
    
    printf("\n<<<<<<<<<<<<<<<\nBrainSplat\n>>>>>>>>>>>>>>>\n");

    /* Spew it back out */
    for (int i = 0; i < sc; i++)
        printf("%c", source[i]);
    putchar('\n');

    /* Interpret */
    while (loc < sc) {
        command = source[loc];
        if (close_jump && (command != JMPBACK)) {
            loc++;
            continue;
        }
        switch (command) {
            case MOVRIGHT:      
                pointer++;
                break;
            case MOVLEFT:
                pointer--;
                break;
            case INCREMENT:
                cells[pointer]++;
                break;
            case DECREMENT:
                cells[pointer]--;
                break;
            case OUTPUT:
                putchar(cells[pointer]);
                break;
            case INPUT:
                cells[pointer] = source[++loc];
                break;
            case JMP:
                if (cells[pointer] == 0)    /* close the jump and continue */
                    close_jump = true;
                else if (jc >= MAXJUMPS) {
                    fprintf (stderr, "Jumping overboard: exceeded max jumps\n");
                    return EXIT_FAILURE;
                }
                else {                      /* log information and continue */
                    jump_table[jc].start = loc;
                    jump_table[jc].close = -1;
                    jc++;
                }
                break;
            case JMPBACK:
                if (close_jump) 
                    close_jump = false;
                else if (cells[pointer] != 0)      /* jump back */
                    loc = jump_table[jc-1].start;     
                else 
                    jump_table[--jc].close = loc;
                break;
            case PEEK:
                printf("%d\n", cells[pointer]);
                break;
            case SNAPSHOT:
                snapshot(cells, pointer);
                break; 
            case NEWLINE:
                break;
            case EOF:
            default:
                break;
        }    
        loc++;
        if ((loc >= CELLCOUNT) || (loc < 0))
            return 1;
            //loc = loc % CELLCOUNT;
    }

    return 0;
}

void
snapshot(int cells[], int pointer)
{
    int start, end;
    start = pointer - APERTURE;
    end = pointer + APERTURE;
    if (start < 0)
        start = 0;
    if (end > CELLCOUNT)
        end = CELLCOUNT;

    for ( ; start < end; start++)
        printf("%sLocation %d stores %d\n", start == pointer ? "Pointer ----> " : "", start, cells[start]);
}

