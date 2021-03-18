#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Practice of Programming:
    Exercise 2-6 * Implement the necessary changes to addname and delname to delete items 
                    by marking items as unused. How isolated is the rest of the program?
*/

typedef struct Nameval Nameval;
struct Nameval {
    char    *name;
    int     value; 
};

struct NVtab {
    int     nval;       /* current number of values */
    int     max;        /* allocated number of values */
    Nameval *nameval;   /* array of name-value pairs */
} nvtab;

enum { NVINIT = 1, NVGROW = 2 };

int addname(Nameval newname);

int delname(char *name);

void listnames(void);

int
main(void)
{
    printf("Runnner program for addname and delname\n");

    Nameval a, b, c, d, e, f, g, h;   

    a.name = "Brian";
    b.name = "Rob";
    c.name = "Ken";
    d.name = "Dennis";
    e.name = "Paul";
    f.name = "Derek";
    g.name = "Oz";
    h.name = "Elliott";

    addname(a);
    addname(b);
    addname(c);
    addname(d);

    /* Show stats on the nvtab structure */
    printf("nvtab: Size: %d\tCapacity: %d\n", nvtab.nval, nvtab.max);

    /* Grow some more */
    addname(e);
    addname(f);
    
    listnames();
    printf("nvtab: Size: %d\tCapacity: %d\n", nvtab.nval, nvtab.max);
    
    /* Deleting some non-expert C programmers */
    delname("Derek");
    delname("Paul");

    listnames();
    printf("After deleting some names: nvtab: Size: %d\tCapacity: %d\n", nvtab.nval, nvtab.max);

    /* Add in a few more C experts */
    addname(g);
    addname(h);
     
    listnames();
    printf("Added back some names: nvtab: Size: %d\tCapacity: %d\n", nvtab.nval, nvtab.max);
}

/* addname: add new name and value to nvtab */
int addname(Nameval newname)
{
    Nameval *nvp;

    if (nvtab.nameval == NULL) {    /* first time */
        nvtab.nameval = (Nameval *) malloc(NVINIT * sizeof(Nameval));
        if (nvtab.nameval == NULL)
            return -1;
        nvtab.max = NVINIT;
        nvtab.nval = 0;
    } else if (nvtab.nval >= nvtab.max) {   /* grow */
        nvp = (Nameval *) realloc(nvtab.nameval, (NVGROW * nvtab.max) * sizeof(Nameval));
        if (nvp == NULL)
            return -1;
        nvtab.max *= NVGROW;
        nvtab.nameval = nvp;
    }
    /* Search for any unused locations */
    for (int i = 0; i < nvtab.nval; i++) {
        if (nvtab.nameval[i].name == NULL) {
            nvtab.nameval[i] = newname;
            return i;
        }
    }
    nvtab.nameval[nvtab.nval] = newname;
    return nvtab.nval++;
} 


/* delname: remove first matching nameval from nvtab */
int delname(char *name)
{
    int i;
    
    for (i = 0; i < nvtab.nval; i++)
        if (nvtab.nameval[i].name && strcmp(nvtab.nameval[i].name, name) == 0) {
            /* The old way */
            //memmove(nvtab.nameval+i, nvtab.nameval+i+1, (nvtab.nval-(i+1)) * sizeof(Nameval));
            nvtab.nameval[i].name = NULL;       
            /* No need to change now */
            //nvtab.nval--;
            return 1;
        }
    return 0;
} 

/* listnames: list the nameval pairs that remain in nvtab */
void listnames(void)
{
    printf("Names:\t");
    for (int i = 0; i < nvtab.nval; i++)
        if (nvtab.nameval[i].name)
            printf("%s\t", nvtab.nameval[i].name); 
    putchar('\n');
}
