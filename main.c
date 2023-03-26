#include "APP/ReversePolishNotation.h"

int main(int argc, char **argv) {
    ReversePolishNotation_t *object = NULL;
    object = ReversePolishNotation.init(object);

    if ( argc > 1 ) {
        if (!strcmp(*(argv + 1), "-help")) {
            printf("ʹ�÷���:\n");
            printf("DataStructure.exe\n");
            printf("\t�ն�ֱ�ӵ���,������׺���ʽ����ý��,����ϸ���\n");
            printf(("DataStructure.exe [command] [expression]\n"));
            printf("\t[command]\t-GUI\t��python�ű�������GUI����,����������\n");
            printf("\t[expression]\t��׺���ʽ\n");
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
        printf("��������׺���ʽ:");
        ReversePolishNotation.gets(object);
        ReversePolishNotation.InfixToPostfix(object);
        printf("��׺���ʽ��:");
        ymString.puts(object->postfix_expression);
        ReversePolishNotation.solve(object);
        printf("�����:%g\n", object->result);
    }

    end:
    ReversePolishNotation.destruct(object);
    return EXIT_SUCCESS;

    CommandError:
    fprintf( stderr, "'%s' ���ǺϷ�ָ��\n��ʹ�� 'DataStructure.exe -help' �鿴 \n", *(argv + 1));
    goto finally;
    finally:
    ReversePolishNotation.destruct(object);
    return EXIT_FAILURE;
}
