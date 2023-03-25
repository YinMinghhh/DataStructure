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

///**
// * easy reference
// */
//#define _stack(typename) _stack$_##typename##_$
//#define stack(typename) _stack(typename)
//
///**
// * template
// */
//#define _stack_DEF(typename) typedef        \
//struct stack$_##typename##_$ {              \
//    typename element;                       \
//    struct stack$_##typename##_$ * next;    \
//} stack(typename);                          \
//                                            \
//void stack_init$_##typename##_$ (stack(typename) * const this) {                    \
//    this->next = NULL;}                     \
//    \
//bool stack_push$_##typename##_$ (stack(typename) * const this, typename element) {  \
//    stack(typename) * node = (stack(typename) *)malloc(sizeof (stack(typename)));   \
//    if (node == NULL) {                     \
//        fprintf(stderr, "Value of errno: %d\n", errno);                             \
//        fprintf(stderr, "Error stack_push(%s): %s\n", #typename, strerror(errno));  \
//        exit(EXIT_FAILURE);                 \
//    }                                       \
//    node->element = element;                \
//    node->next = this->next;                \
//    this->next = node;                      \
//    return true;}                           \
//    \
//bool stack_empty$_##typename##_$ (const stack(typename) * const this) {             \
//    return this->next == NULL;}             \
//    \
//typename stack_pop$_##typename##_$ (stack(typename) * const this) {                 \
//    stack(typename) * temp_node = this->next;                    \
//    typename temp_element = temp_node->element;                  \
//    this->next = this->next->next;          \
//    free(temp_node);                        \
//    return temp_element;}                   \
//    \
//typename stack_top$_##typename##_$ (const stack(typename) * const this) {           \
//    if (!stack_empty(typename)(this))       \
//        return this->next->element;         \
//    else {                                  \
//        fprintf(stderr, "Value of errno: %d\n", errno);                             \
//        fprintf(stderr, "Error stack_top(%s): %s\n", #typename, strerror(errno));   \
//        exit(EXIT_FAILURE);                 \
//    }}
//
//#define stack_def(typename) _stack_DEF(typename)
//
///**
// * easy references for functions
// */
//#define stack_init(typename) stack_init$_##typename##_$
//#define stack_push(typename) stack_push$_##typename##_$
//#define stack_empty(typename) stack_empty$_##typename##_$
//#define stack_pop(typename) stack_pop$_##typename##_$
//#define stack_top(typename) stack_top$_##typename##_$
//
///**
// * test
// */
////stack_def(int)

#define ymStack_t(typename) STACK_$##typename##$
#define ymStack(typename)   STACK_fun_$##typename##$

#define ymStack_init(typename)  ymStack_$##typename##$_init
#define ymStack_push(typename)  ymStack_$##typename##$_push
#define ymStack_empty(typename) ymStack_$##typename##$_empty
#define ymStack_top(typename)   ymStack_$##typename##$_top
#define ymStack_pop(typename)   ymStack_$##typename##$_pop


#define _STACK_TEMPLATE_(typename) typedef \
struct STACK_$##typename##$                \
{                                          \
    typename element;                      \
    struct STACK_$##typename##$ *next;     \
} ymStack_t(typename);                     \
                                           \
\
static ymStack_t(typename) *ymStack_$##typename##$_init (ymStack_t(typename) *this); \
static bool ymStack_$##typename##$_push(ymStack_t(typename) *this, typename element);\
static bool ymStack_$##typename##$_empty(const ymStack_t(typename) * this);          \
static typename ymStack_$##typename##$_top(const ymStack_t(typename) * this);        \
static typename ymStack_$##typename##$_pop(ymStack_t(typename) * this);              \
\
                                           \
struct {                                   \
    ymStack_t(typename) *(*init)(ymStack_t(typename)*);                              \
    bool    (*push)     (ymStack_t(typename)*, typename);                            \
    bool    (*empty)    (const ymStack_t(typename)*);                                \
    typename(*top)      (const ymStack_t(typename)*);                                \
    typename(*pop)      (ymStack_t(typename)*);                                      \
    \
} ymStack(typename) = {                    \
    .init   =   ymStack_init    (typename),\
    .push   =   ymStack_push    (typename),\
    .empty  =   ymStack_empty   (typename),\
    .top    =   ymStack_top     (typename),\
    .pop    =   ymStack_pop     (typename),\
};                       \
                                           \
static ymStack_t(typename) *ymStack_$##typename##$_init (ymStack_t(typename) *const this) \
{                                          \
    this->next = NULL;                     \
    return this;                           \
}                                          \
                                           \
static bool ymStack_$##typename##$_push(ymStack_t(typename) *const this, typename element)\
{                                          \
    ymStack_t(typename) *new_node = (ymStack_t(typename)*) malloc( \
    sizeof (ymStack_t(typename)));         \
    if (new_node == NULL) goto MallocError;\
    else;                                  \
    new_node->element = element;           \
    new_node->next = this->next;           \
    this->next = new_node;                 \
    return true;                           \
                                           \
    MallocError:                           \
    fprintf( stderr, "Error %s %s: malloc error\n", __FILE__, __FUNCTION__ ); \
    goto finally;                          \
    finally:                               \
    exit(EXIT_FAILURE);\
}                                          \
                                           \
static bool ymStack_$##typename##$_empty(const ymStack_t(typename) *const this)           \
{                                          \
    return this->next == NULL;             \
}                                          \
                                           \
static typename ymStack_$##typename##$_top(const ymStack_t(typename) *const this)         \
{                                          \
    if (!ymStack_empty(typename)(this)) {  \
        return this->next->element;        \
    } else goto EmptyStackError;           \
                                           \
    EmptyStackError:                       \
    fprintf( stderr, "Error %s %s: empty stack error\n", __FILE__, __FUNCTION__ );        \
    goto finally;                          \
    finally:                               \
    exit(EXIT_FAILURE);                    \
}                                          \
                                           \
static typename ymStack_$##typename##$_pop(ymStack_t(typename) *const this)               \
{                                          \
    if (ymStack_empty(typename)(this)) goto EmptyStackError;                              \
    else;                                  \
    ymStack_t(typename) *tmp_node = this->next;                                           \
    typename tmp_element = tmp_node->element;                                             \
    this->next = this->next->next;         \
    if (tmp_node != NULL) { free(tmp_node); tmp_node = NULL; }                            \
    else;                                  \
    return tmp_element;                    \
                                           \
    EmptyStackError:                       \
    fprintf( stderr, "Error %s %s: empty stack error\n", __FILE__, __FUNCTION__ );        \
    goto finally;                          \
    finally:                               \
    exit(EXIT_FAILURE);                    \
}\


#define stack_instantiation(typename) _STACK_TEMPLATE_(typename)

stack_instantiation(char)
stack_instantiation(int)
stack_instantiation(float)
stack_instantiation(double)

//#undef ymStack_empty
//#undef ymStack_init
//#undef ymStack_push
//#undef ymStack_empty
//#undef ymStack_top
//#undef ymStack_pop

#endif //DATASTRUCT01_YMSTACK_H
