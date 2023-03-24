/**
 * @file    ymstack.h
 * @brief   实现一个栈模板
 * @author  YinMing
 * @version 1.0
 * @date    2023-03-20
 */

/**
 * Sample Usage:
```c
#include "ymstack.h"
stack_def(int); // 这里不用';'也可以
int main() {
    stack(int) temp;
    stack_init(int)(&temp);
    return 0;
}
```
 */

#ifndef DATASTRUCT01_YMSTACK_H
#define DATASTRUCT01_YMSTACK_H

#pragma message ("compiling ymStack.h ...")

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * easy reference
 */
#define _stack(typename) _stack$_##typename##_$
#define stack(typename) _stack(typename)

/**
 * template
 */
#define _stack_DEF(typename) typedef        \
struct stack$_##typename##_$ {              \
    typename element;                       \
    struct stack$_##typename##_$ * next;    \
} stack(typename);                          \
                                            \
void stack_init$_##typename##_$ (stack(typename) * const this) {                    \
    this->next = NULL;}                     \
    \
bool stack_push$_##typename##_$ (stack(typename) * const this, typename element) {  \
    stack(typename) * node = (stack(typename) *)malloc(sizeof (stack(typename)));   \
    if (node == NULL) {                     \
        fprintf(stderr, "Value of errno: %d\n", errno);                             \
        fprintf(stderr, "Error stack_push(%s): %s\n", #typename, strerror(errno));  \
        exit(EXIT_FAILURE);                 \
    }                                       \
    node->element = element;                \
    node->next = this->next;                \
    this->next = node;                      \
    return true;}                           \
    \
bool stack_empty$_##typename##_$ (const stack(typename) * const this) {             \
    return this->next == NULL;}             \
    \
typename stack_pop$_##typename##_$ (stack(typename) * const this) {                 \
    stack(typename) * temp_node = this->next;                    \
    typename temp_element = temp_node->element;                  \
    this->next = this->next->next;          \
    free(temp_node);                        \
    return temp_element;}                   \
    \
typename stack_top$_##typename##_$ (const stack(typename) * const this) {           \
    if (!stack_empty(typename)(this))       \
        return this->next->element;         \
    else {                                  \
        fprintf(stderr, "Value of errno: %d\n", errno);                             \
        fprintf(stderr, "Error stack_top(%s): %s\n", #typename, strerror(errno));   \
        exit(EXIT_FAILURE);                 \
    }}

#define stack_def(typename) _stack_DEF(typename)

/**
 * easy references for functions
 */
#define stack_init(typename) stack_init$_##typename##_$
#define stack_push(typename) stack_push$_##typename##_$
#define stack_empty(typename) stack_empty$_##typename##_$
#define stack_pop(typename) stack_pop$_##typename##_$
#define stack_top(typename) stack_top$_##typename##_$

/**
 * test
 */
//stack_def(int)

//#define _STACK_TEMPLATE_(typename) typedef \
//struct STACK_$##typename##$                \
//{                                          \
//    typename element;                      \
//    struct STACK_$##typename##$ *next;     \
//} stack(typename); \
//
//#define stack_instantiation(typename) _STACK_TEMPLATE_(typename)
//
//stack_instantiation(char)

#endif //DATASTRUCT01_YMSTACK_H
