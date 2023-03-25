#include "APP/ReversePolishNotation.h"

int main(int argc, char **argv) {
    ReversePolishNotation_t *object = NULL;
    object = ReversePolishNotation.init(object);
    ReversePolishNotation.gets(object);
    ReversePolishNotation.InfixToPostfix(object);
    ymString.puts(object->postfix_expression);
    ReversePolishNotation.solve(object);
    printf("the result is %lf\n", object->result);

//    putchar('\n');
    system("pause");
    return 0;
}
