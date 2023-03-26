#include "APP/ReversePolishNotation.h"

int main(int argc, char **argv) {
    ReversePolishNotation_t *object = NULL;
    object = ReversePolishNotation.init(object);

    if ( argc > 1 ) {
        if (!strcmp(*(argv + 1), "-help")) {
            printf("使用方法:\n");
            printf("DataStructure.exe\n");
            printf("\t终端直接调用,输入中缀表达式并获得结果,有详细输出\n");
            printf(("DataStructure.exe [command] [expression]\n"));
            printf("\t[command]\t-GUI\t给python脚本制作的GUI调用,仅输出求解结果\n");
            printf("\t[expression]\t中缀表达式\n");
            goto end;
        } else if (!strcmp(*(argv + 1), "-GUI")) {
            ymString.cpy(object->infix_expression, *((unsigned char**)(argv) + 2));
            ReversePolishNotation.InfixToPostfix(object);
            ReversePolishNotation.solve(object);
            printf("%g\n", object->result);
        } else {
            goto CommandError;
        }
    } else {
        printf("请输入中缀表达式:");
        ReversePolishNotation.gets(object);
        ReversePolishNotation.InfixToPostfix(object);
        printf("后缀表达式是:");
        ymString.puts(object->postfix_expression);
        ReversePolishNotation.solve(object);
        printf("结果是:%g\n", object->result);
    }

    end:
    ReversePolishNotation.destruct(object);
    return EXIT_SUCCESS;

    CommandError:
    fprintf( stderr, "'%s' 不是合法指令\n请使用 'DataStructure.exe -help' 查看 \n", *(argv + 1));
    goto finally;
    finally:
    ReversePolishNotation.destruct(object);
    return EXIT_FAILURE;
}
