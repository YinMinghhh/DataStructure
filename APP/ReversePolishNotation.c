#include "ReversePolishNotation.h"
const ReversePolishNotation_fun ReversePolishNotation = {
        .init   =   ReversePolishNotation_init,
        .gets   =   ReversePolishNotation_infix_gets,
        .InfixToPostfix =   ReversePolishNotation_InfixToPostfix,
        .substitute     =   ReversePolishNotation_substitution,
        .InfixToPrefix  =   ReversePolishNotation_InfixToPrefix,
        .solve  =   ReversePolishNotation_solve,
        .destruct = ReversePolishNotation_destruct,
};
