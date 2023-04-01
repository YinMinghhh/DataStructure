//
// Created by 28197 on 2023/3/21.
//

#ifndef DATASTRUCT01_REVERSEPOLISHNOTATION_H
#define DATASTRUCT01_REVERSEPOLISHNOTATION_H

#include "../lib/ymString.h"
#include "../lib/ymStack.h"
#include <ctype.h>

typedef struct {
    ymString_t *infix_expression;   /* 中缀表达式串 */
    ymString_t *prefix_expression;  /* 后缀表达式串 */
    ymString_t *postfix_expression; /* 后缀表达式串 */
    ymStack_t(uint8)     *op_stack; /* 操作符栈,仅在中缀转后缀时使用 */
    ymStack_t(double)   *res_stack; /* 操作数栈,仅在求解后缀式时使用 */
    double result;                  /* 求解后缀式后得出的结果 */
} ReversePolishNotation_t;


static int ReversePolishNotation_priority         (unsigned char ch);
static double ReversePolishNotation_calc(double a, double b, unsigned char operator);

static ReversePolishNotation_t *ReversePolishNotation_init(ReversePolishNotation_t *this);
static bool ReversePolishNotation_infix_gets      (ReversePolishNotation_t *this);
static void ReversePolishNotation_InfixToPostfix  (ReversePolishNotation_t *this);
static void ReversePolishNotation_InfixToPrefix  (ReversePolishNotation_t *this);
static double ReversePolishNotation_solve(ReversePolishNotation_t *this);
static void ReversePolishNotation_destruct(ReversePolishNotation_t *this);
static void ReversePolishNotation_OperatorToStack_ITP(ReversePolishNotation_t *this, unsigned char ch);
static void ReversePolishNotation_pretreatment(ReversePolishNotation_t *this);

typedef struct {
    ReversePolishNotation_t* (*init)(ReversePolishNotation_t*);
    bool    (*const gets)           (ReversePolishNotation_t*);
    void    (*const InfixToPostfix) (ReversePolishNotation_t*);
    void    (*const InfixToPrefix)  (ReversePolishNotation_t*);
    double  (*const solve)          (ReversePolishNotation_t*);
    void    (*const destruct)       (ReversePolishNotation_t*);
} ReversePolishNotation_fun;

extern const ReversePolishNotation_fun ReversePolishNotation;

static ReversePolishNotation_t* ReversePolishNotation_init(ReversePolishNotation_t * this)
{
    this = (ReversePolishNotation_t *) malloc(sizeof (ReversePolishNotation_t));
    if (this == NULL) goto MallocError;
    else;
    this->infix_expression      =   NULL;
    this->postfix_expression    =   NULL;
    this->prefix_expression     =   NULL;
    this->op_stack  =   NULL;
    this->res_stack =   NULL;
    this->infix_expression      =   ymString.init(this->infix_expression, 20);
    this->postfix_expression    =   ymString.init(this->postfix_expression, 30);
    this->prefix_expression     =   ymString.init(this->prefix_expression, 30);
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
    ymString.gets(this->infix_expression);
    ReversePolishNotation_pretreatment(this);
    ymString.puts(this->infix_expression);
    return true;
}

static double ReversePolishNotation_solve(ReversePolishNotation_t *const this)
{
    if (ymString.empty(this->postfix_expression)) goto EmptyPostfixExpressionError;
    else;
    unsigned char *ptr = this->postfix_expression->ptr;
    char *p_ptr = NULL;
    for (int i = 0; i < this->postfix_expression->length; ++i, ++ptr) {
        unsigned char ch = *ptr;
        if (isdigit(ch) || ( (ch == '-' || ch =='+') && isdigit(*(ptr + 1)))) {
//            对数字处理, 考虑特殊情况下带有正负号的数字
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

static void ReversePolishNotation_pretreatment(ReversePolishNotation_t *const this)
{
    for (size_t i = 0; i < this->infix_expression->length; ++i) {
        uint8 ch = *(this->infix_expression->ptr + i);
        switch (ch) {
            case '+':
            case '-': {
                bool is_negative = ch == '-' ? true : false;
                size_t j = i + 1;
                for (; j < this->infix_expression->length; ++j) {
                    uint8 tmp_ch = *(this->infix_expression->ptr + j);
                    if (tmp_ch == '-') is_negative = is_negative ? false : true;
                    else if (tmp_ch == '+') ;
                    else if (ReversePolishNotation_priority(tmp_ch) || isdigit(tmp_ch))
                        break;
                    else;
                    *(this->infix_expression->ptr + j) = ' ';
                }
                *(this->infix_expression->ptr + i) = is_negative ? '-' : '+';
                i = j - 1;
            }
                break;
            default:;
        }
    }
    ymString.delChar(this->infix_expression, ' ');
}

static void ReversePolishNotation_InfixToPostfix(ReversePolishNotation_t *const this)
{
    uint8 last_ch = '\0';
    uint8 next_ch;
    for (size_t i = 0; i < this->infix_expression->length; i++) {
        uint8 ch = *(this->infix_expression->ptr + i);
        if ( isdigit(ch)
            || ( ch == '-' || ch == '+' )
            && (i == 0 || (ReversePolishNotation_priority(last_ch) && last_ch != ')'))
            ) {
            switch (ch) {
                case '+':
                    break;
                case '-': {
                    for (size_t j = i + 1; j < this->infix_expression->length; ++j) {
                        next_ch = *(this->infix_expression->ptr + j);
                        if (next_ch == '(') {
                            ymString.cat(this->postfix_expression, (uint8 *) "0 ");

                            ReversePolishNotation_t *tmp_infix = NULL;
                            tmp_infix = ReversePolishNotation.init(tmp_infix);
                            ymString.append(tmp_infix->infix_expression, '(');
                            size_t count_of_left_parenthesis = 1,
                                    count_of_right_parenthesis = 0;
                            for (size_t k = j + 1; k < this->infix_expression->length; ++k) {
                                uint8 tmp_ch;
                                tmp_ch = *(this->infix_expression->ptr + k);
                                if (tmp_ch == '(') count_of_left_parenthesis += 1;
                                else if (tmp_ch == ')') count_of_right_parenthesis += 1;
                                else;
                                ymString.append(tmp_infix->infix_expression, tmp_ch);
                                if (count_of_left_parenthesis == count_of_right_parenthesis) {
                                    i = k;
                                    break;
                                } else;
                            }
                            ReversePolishNotation_InfixToPostfix(tmp_infix);
                            ymString.cat(this->postfix_expression, tmp_infix->postfix_expression->ptr);
                            ReversePolishNotation.destruct(tmp_infix);
                            ymString.cat(this->postfix_expression, (uint8 *) "- ");
                            break;
                        } else break;
                    }   // end of for
                }
                default:
                    //            遇到正负号或者数字字符时,连续读掉后面一整个数字
                {
                    if (isdigit(ch) || ch == '+' || ch == '-') {
                        ymString.append(this->postfix_expression, ch);
                        size_t j = i + 1;
                        for (; j < this->infix_expression->length; j++) {
                            next_ch = *(this->infix_expression->ptr + j);
                            if (ReversePolishNotation_priority(next_ch)) break;
                            else if (isdigit(next_ch) || next_ch == '.') {
                                last_ch = next_ch;
                                ymString.append(this->postfix_expression, next_ch);
                            } else { ;
                            }
                        }
                        ymString.append(this->postfix_expression, ' ');
                        i = j - 1;
                    }
                }
            }


        } else if (ReversePolishNotation_priority(ch)) {
            /* '+' '-' '*' '/' '(' ')' */
//                        遇到加减乘除'+' '-' '*' '/'运算符,弹运算符栈入后缀表达式串,至栈顶运算符优先级小于ch或清空栈为止,再压入ch
//                        同时给后缀表达式字符串加一个空格' ',因为在中缀表达式中,这些双目运算符的左边必定是一个运算数的结尾
            ReversePolishNotation_OperatorToStack_ITP(this, ch);
            last_ch = ch;
        } else if (ch == '.') {     // 小数点'.'的处理,直接加入后缀表达式字符串
            ymString.append(this->postfix_expression, ch);
            last_ch = ch;
        } else;
    }

    while (!ymStack(uint8).empty(this->op_stack)) {
        ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
        ymString.append(this->postfix_expression, ' ');
    }
}

/**
 * @brief 这个函数只处理 '(' , ')' , '*' , '/' 和作为双目运算符的 '-' , '+'
 * @param this
 * @param ch
 * @note    前缀转后缀用
 */
static void ReversePolishNotation_OperatorToStack_ITP(ReversePolishNotation_t *const this, unsigned char ch)
{
    if ( ch == '(' ) {
        ymStack(uint8).push(this->op_stack, ch);
    } else if ( ch == ')' ) {
        while ( !ymStack(uint8).empty(this->op_stack) && ymStack(uint8).top(this->op_stack) != '(') {
            ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
            ymString.append(this->postfix_expression, ' ');
        }
        ymStack(uint8).pop(this->op_stack);
    } else {
        while (
                !ymStack(uint8).empty(this->op_stack)
                && ReversePolishNotation_priority(ymStack(uint8).top(this->op_stack)) >=
                   ReversePolishNotation_priority(ch)
                ) {
            ymString.append(this->postfix_expression, ymStack(uint8).pop(this->op_stack));
        }
        ymStack(uint8).push(this->op_stack, ch);
    }
}

static void ReversePolishNotation_InfixToPrefix  (ReversePolishNotation_t *const this)
{
    for (size_t i = this->infix_expression->length; i > 0 ; ) {
        i -= 1;
        uint8 ch = *(this->infix_expression->ptr + i);
        if (isdigit(ch)) {
            ymString.append(this->prefix_expression, ch);
        } else {
            switch (ch) {
                case ')':
                    ymStack(uint8).push(this->op_stack, ch);
                    break;
                case '(':
                    while ( !ymStack(uint8).empty(this->op_stack) && ymStack(uint8).top(this->op_stack) != ')' ) {
                        ymString.append(this->prefix_expression, ' ');
                        ymString.append(this->prefix_expression, ymStack(uint8).pop(this->op_stack));
                    }
                    ymString.append(this->prefix_expression, ' ');
                    ymStack(uint8).pop(this->op_stack);
                    break;
                default:
                    if (ReversePolishNotation_priority(ch)) {
                        /* + - * / */
                        if (ch == '-' || ch == '+') {
                            unsigned char *pre_ch = this->infix_expression->ptr + i;
                            while ( i == 0 ||--pre_ch >= this->infix_expression->ptr) {
                                if (isdigit(*pre_ch)) {     // 说明是运算符
                                    break;
                                } else if (ReversePolishNotation_priority(*pre_ch)) {   // 说明是正负号
                                    ymString.append(this->prefix_expression, ch);
                                    goto label;
                                } else;
                            }
                        }
                        while ( !ymStack(uint8).empty(this->op_stack) &&
                                ReversePolishNotation_priority(ymStack(uint8).top(this->op_stack)) >
                                ReversePolishNotation_priority(ch) &&
                                ymStack(uint8).top(this->op_stack) != ')') {
                            ymString.append(this->prefix_expression, ymStack(uint8).pop(this->op_stack));
                        }
                        ymString.append(this->prefix_expression, ' ');
                        ymStack(uint8).push(this->op_stack, ch);
                    } else label: continue;
            }
        }
    }
    while (!ymStack(uint8).empty(this->op_stack)) {
        ymString.append(this->prefix_expression, ' ');
        ymString.append(this->prefix_expression, ymStack(uint8).pop(this->op_stack));
    }
    ymString.reverse(this->prefix_expression);
}

static void ReversePolishNotation_destruct(ReversePolishNotation_t *this)
{
    if (this != NULL) {
        if (this->infix_expression != NULL) {
            ymString.destruct(this->infix_expression);
        } else;
        if (this->postfix_expression != NULL) {
            ymString.destruct(this->postfix_expression);
        } else;
        if (this->prefix_expression != NULL) {
            ymString.destruct(this->prefix_expression);
        } else;
        if (this->op_stack != NULL) {
            ymStack(uint8).destruct(this->op_stack);
        } else;
        if (this->res_stack != NULL) {
            ymStack(double).destruct(this->res_stack);
        } else;
    } else;
}

//  私有的"成员函数"

static int ReversePolishNotation_priority( unsigned char ch )
{
    switch (ch) {
        case '(':
        case ')':return 1;
        case '+':
        case '-':return 2;
        case '*':
        case '/':return 3;

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
