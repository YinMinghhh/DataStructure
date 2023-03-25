#include "lib/ymStack.h"
#include "lib/ymString.h"

int main(int argc, char **argv) {
    ymString_t *buffer = NULL, *expression = NULL;
    expression  =   ymString.init(expression, 10);
    buffer      =   ymString.init(buffer, 10);
    ymString.gets(expression);
//    ymString.puts(expression);

    ;
    putchar('\n');
    system("pause");
    return 0;
}
