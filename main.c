#include <stdio.h>
#include <stdbool.h>
#include "lib/ymStack.h"
#include "lib/ymSeqList.h"
#include "lib/ymString.h"

stack_def(char)

int main(int argc, char **argv) {
    char expression[100];
    char buffer[100];
    unsigned long long length; stack(char) stack;
    int i = 0, j = 0;   // i -> expression, j -> buffer
    scanf("%s", expression);
    length = strlen(expression);
    stack_init(char)(&stack);

    for (i = 0; i < length; i++) {
        char element = expression[i];
        if (element >= '0' && element <= '9') {
            buffer[j++] = element;
        } else if (element == '(') {
            stack_push(char)(&stack, element);
        } else if (element == ')') {
            char operator0;
            if (!stack_empty(char)(&stack)) {
                operator0 = stack_top(char)(&stack);
                while (operator0 != '(') {
                    buffer[j++] = stack_pop(char)(&stack);
                    if (!stack_empty(char)(&stack))
                        operator0 = stack_top(char)(&stack);
                }
            }
            stack_pop(char)(&stack);
        } else if (element == '*' || element == '/') {
            char operator1;
            if (!stack_empty(char)(&stack)) {
                operator1 = stack_top(char)(&stack);
                while (operator1 == '*' || operator1 == '/') {
                    buffer[j++] = stack_pop(char)(&stack);
                    if (!stack_empty(char)(&stack))
                        operator1 = stack_top(char)(&stack);
                }
            }
            stack_push(char)(&stack, element);
        } else if (element == '+' || element == '-') {
            char operator2;
            if (!stack_empty(char)(&stack)) {
                operator2 = stack_top(char)(&stack);
                while (operator2 == '*' || operator2 == '/' || operator2 == '+' || operator2 == '-') {
                    buffer[j++] = stack_pop(char)(&stack);
                    if (!stack_empty(char)(&stack))
                        operator2 = stack_top(char)(&stack);
                }
            }
            stack_push(char)(&stack, element);
        } else {
            buffer[j++] = element;
        }
    }
    while (!stack_empty(char)(&stack)) {
        buffer[j++] = stack_pop(char)(&stack);
    }
    for (int k = 0; k < j; ++k) {
        putchar(buffer[k]);
    }
    putchar('\n');
    system("pause");
    return 0;
}
