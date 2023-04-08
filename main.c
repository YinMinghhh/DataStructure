#include "APP/ReversePolishNotation.h"

bool if_continue();

int main(int argc, char **argv) {
    ReversePolishNotation_t *object = NULL;
    object = ReversePolishNotation.init(object);

    if ( argc > 1 ) {
        if (!strcmp(*(argv + 1), "-help")) {
//            -help 帮助文档
            printf("'Calculator.exe' by YinMing\n\n");
            printf("使用方法:\n\n");

            printf("Calculator [command]\n");
            printf("[command]\n");
            printf("\t无参数\t无未知数的表达式计算\n");
            printf("\t-U\t带未知数的表达式计算\n\n");

            printf(("Calculator [command] [expression]\n"));
            printf("[command]\n");
            printf("\t-GUI\t给python脚本制作的GUI调用,终端调用无需关心\n");
            printf("[expression]\n");
            printf("\t中缀表达式\n\n");
        } else if (!strcmp(*(argv + 1), "-GUI")) {
//            -GUI  GUI.py使用此进行计算
            ymString.cpy(object->infix_expression, *((unsigned char**)(argv) + 2));
            ReversePolishNotation.substitute(object);
            ReversePolishNotation.InfixToPostfix(object);
            ReversePolishNotation.solve(object);
            printf("%g\n", object->result);
        } else if (!strcmp(*(argv + 1), "-U")) {
//            -U 带未知量的表达式计算
            printf("input your expression:\n>>> ");
            ReversePolishNotation.gets(object);
            printf("Your infix expression: ");
            ymString.puts(object->infix_expression);
            if (object->var_count > 0) {
                printf("Parsing complete.\nYour expression have %zu variable", object->var_count);
                if (object->var_count > 1) { putchar('s'); }
                printf(".\n\n");
            } else {
                printf("Warning:\n");
                printf("\tYour expression does not contain variables.\n");
                printf("\tYou may exit and use the parameterless invocation method, such as:\n");
                printf("\t\"Calculator\"\n\n");
            }

            while (1) {
                ReversePolishNotation.substitute(object);
                ymString.clear(object->postfix_expression, object->postfix_expression->capacity);
                ReversePolishNotation.InfixToPostfix(object);
                ReversePolishNotation.solve(object);
                printf("result: %g\n\n", object->result);

                if (!if_continue()) { break; }
            }
        } else {
//            错误参数
            goto CommandError;
        }
    } else {
//        无参调用
        while (1) {
            printf("input infix expression:\n>>> ");
            ReversePolishNotation.gets(object);
            ReversePolishNotation.substitute(object);
            ymString.clear(object->postfix_expression, object->postfix_expression->capacity);
            ReversePolishNotation.InfixToPostfix(object);
            printf("Your post expression is: ");
            ymString.puts(object->postfix_expression);
            ReversePolishNotation.solve(object);
            printf("result: %g\n\n", object->result);
            if (!if_continue()) { break; }
        }
    }

    ReversePolishNotation.destruct(object);
    return EXIT_SUCCESS;

    CommandError:
    fprintf( stderr, "'%s' 不是合法指令\n请使用 'Calculator -help' 查看帮助文档 \n", *(argv + 1));
    goto finally;
    finally:
    ReversePolishNotation.destruct(object);
    return EXIT_FAILURE;
}

bool if_continue()
{
    printf("Continue? [Y/N]\n>>> ");
    while (1) {
        fflush(stdin);
        char ch = (char)getchar();
        fflush(stdin);
        switch (ch) {
            case 'Y':
            case 'y':
                return true;
            case 'N':
            case 'n':
                return false;
            default:
                printf("input 'Y' or 'N'\n>>> ");
        }
    }
}
