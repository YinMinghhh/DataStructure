#include "ReversePolishNotation.h"
const ReversePolishNotation_fun ReversePolishNotation = {
        .init   =   ReversePolishNotation_init,
        .gets   =   ReversePolishNotation_infix_gets,
        .InfixToPostfix =   InfixToPostfix,
};
