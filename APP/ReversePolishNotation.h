//
// Created by 28197 on 2023/3/21.
//

#ifndef DATASTRUCT01_REVERSEPOLISHNOTATION_H
#define DATASTRUCT01_REVERSEPOLISHNOTATION_H

#include "../lib/ymString.h"
#include "../lib/ymStack.h"
#include <ctype.h>

typedef struct {
    ymString_t *infix_expression;   /* 前缀表达式 */
    ymString_t *postfix_expression; /* 后缀表达式 */
    ymStack_t(uint8)     *op_stack;
    ymStack_t(double)   *res_stack;
    double result;
} ReversePolishNotation_t;


static int ReversePolishNotation_priority         (unsigned char ch);
static double ReversePolishNotation_calc(double a, double b, unsigned char operator);

static ReversePolishNotation_t *ReversePolishNotation_init(ReversePolishNotation_t *this);
static bool ReversePolishNotation_infix_gets      (ReversePolishNotation_t *this);
static void ReversePolishNotation_InfixToPostfix  (ReversePolishNotation_t *this);
static double ReversePolishNotation_solve(ReversePolishNotation_t *this);

typedef struct {
    ReversePolishNotation_t* (*init)(ReversePolishNotation_t*);
    bool    (*const gets)           (ReversePolishNotation_t*);
    void    (*const InfixToPostfix) (ReversePolishNotation_t*);
    double  (*const solve)          (ReversePolishNotation_t*);
} ReversePolishNotation_fun;

extern const ReversePolishNotation_fun ReversePolishNotation;

static ReversePolishNotation_t* ReversePolishNotation_init(ReversePolishNotation_t * this)
{
    this = (ReversePolishNotation_t *) malloc(sizeof (ReversePolishNotation_t));
    if (this == NULL) goto MallocError;
    else;
    this->infix_expression      =   NULL;
    this->postfix_expression    =   NULL;
    this->op_stack  =   NULL;
    this->res_stack =   NULL;
    this->infix_expression      =   ymString.init(this->infix_expression, 20);
    this->postfix_expression    =   ymString.init(this->postfix_expression, 20);
    this->op_stack  =   ymStack(uint8).init(this->op_stack);
    this->res_stack =   ymStack(double).init(this->res_stack);
    return this;

    MallocError:
    fprintf( stderr, "Error %s %s: malloc error\n", __FILE__, __FUNCTION__ );
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

static bool ReversePolishNotation_infix_gets(ReversePolishNotation_t *this)
{
    return ymString.gets(this->infix_expression);
}

static double ReversePolishNotation_solve(ReversePolishNotation_t *const this)
{
    if (ymString.empty(this->postfix_expression)) goto EmptyPostfixExpressionError;
    else;
    unsigned char *ptr = this->postfix_expression->ptr;
    char *p_ptr = NULL;
    for (int i = 0; i < this->postfix_expression->length; ++i, ++ptr) {
        unsigned char ch = *ptr;
        if (isdigit(ch)) {
            ymStack(double).push(this->res_stack, strtod((char*)ptr, &p_ptr));
            ptr = (unsigned char *)p_ptr;
        } else if (ReversePolishNotation_priority(ch)) {
            double num2 = ymStack(double).pop(this->res_stack);
            double num1 = ymStack(double).pop(this->res_stack);
            ymStack(double).push(this->res_stack, ReversePolishNotation_calc(num1, num2, ch));
        }
    }
    this->result = ymStack(double).top(this->res_stack);
    return this->result;

    EmptyPostfixExpressionError:
    fprintf( stderr, "Error %s %s: empty postfix expression\n", __FILE__, __FUNCTION__ );
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

static void ReversePolishNotation_InfixToPostfix(ReversePolishNotation_t *const this)
{
    for (size_t i = 0; i < this->infix_expression->length; i++) {
        unsigned char ch = *(this->infix_expression->ptr + i);
        if (isdigit(ch)) {
            ymString.append(this->postfix_expression, ch);
        } else {
            switch (ch) {
                case '(':
                    ymStack(uint8).push(this->op_stack, ch);
                    break;
                case ')':
                    while (!ymStack(uint8).empty(this->op_stack) && ymStack(uint8).top(this->op_stack) != '(') {
                        ymString.append(this->postfix_expression, ' ');
                        ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
                    }
                    ymStack(uint8).pop(this->op_stack);
                    break;
                default:
                    if (ReversePolishNotation_priority(ch)) {
                        /* + - * / */
                        while (
                                !ymStack(uint8).empty(this->op_stack) &&
                                ReversePolishNotation_priority(ymStack(uint8).top(this->op_stack)) >=
                                ReversePolishNotation_priority(ch) &&
                                ymStack(uint8).top(this->op_stack) != '('
                        ) {
                            ymString.append(this->postfix_expression, ' ');
                            ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
                        }
                        ymString.append(this->postfix_expression, ' ');
                        ymStack(uint8).push(this->op_stack, ch);
                    } else;
            }
        }
    }
    while (!ymStack(uint8).empty(this->op_stack)) {
        ymString.append(this->postfix_expression, ' ');
        ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
    }
}

//  私有的"成员函数"

static int ReversePolishNotation_priority( unsigned char ch )
{
    switch (ch) {
        case '+':
        case '-':return 1;
        case '*':
        case '/':return 2;
        case '(':
        case ')':return 3;
        default:
            return 0;
    }
}

static double ReversePolishNotation_calc(double a, double b, unsigned char operator)
{
    switch (operator) {
        case '+':return a + b;
        case '-':return a - b;
        case '*':return a * b;
        case '/':
            if ( b != 0 ) {
                return a / b;
            } else goto ArithmeticError;
        default:
            goto OperatorError;
    }

    ArithmeticError:
    fprintf( stderr, "Error %s %s: arithmetic error\n", __FILE__, __FUNCTION__ );
    fprintf( stderr, "The divisor is zero\n");
    goto finally;
    OperatorError:
    fprintf( stderr, "Error %s %s: operator error\n", __FILE__, __FUNCTION__ );
    fprintf( stderr, "Operator is '%c'\n", operator);
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

#endif //DATASTRUCT01_REVERSEPOLISHNOTATION_H
