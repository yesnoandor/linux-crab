#include "stdlib.h"
#include "generator.h"

int main(int argc, char *argv[])
{
    generator gen;
    if (argv[1] != NULL && argv[2] != NULL && (atoi(argv[1]) > 0) && (atoi(argv[2]) > 0))
    {
        gen.generate(atoi(argv[1]), atoi(argv[2]));
    }
    else
    {
        int linenumber = LOGLINENUMBER;
        gen.generate(linenumber);
    }
}
