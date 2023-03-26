//
// Created by 28197 on 2023/3/21.
//

#ifndef DATASTRUCT01_YMSTRING_H
#define DATASTRUCT01_YMSTRING_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#define STRING_DEFAULT_DELTA_CAPACITY ((size_t) 10)

struct StringUtil_functions;
typedef struct StringUtil_functions StringUtil_functions;

/**
 * @struct  ymString_t
 * @brief   实现了一个比较好用的String类型
 * @param   ptr         指向字符们被存储的区域
 * @param   length      表示储存的有效字符的个数, 不包括所有的'\0'
 * @param   capacity    表示可储存的有效字符的个数, 不包括最后的'\0', \n
 *                      所以ptr指向的区域最终占用的空间是 sizeof(unsigned char) * (capacity + 1)
 * @param   vptr        指向方法结构体
 * @author  YinMing
 * @note    1. 没写自动缩容功能, 在意空间可以destruct掉,  重新init
 * @note    2. 注意存的字符都是 unsigned char
 */
typedef struct {
    unsigned char *ptr;
    size_t length;
    size_t capacity;
//    const StringUtil_functions *vptr;
} ymString_t;

static ymString_t * ymString_init    (ymString_t *this, size_t capacity);
static bool ymString_puts   (const ymString_t *this);
static bool ymString_empty  (const ymString_t *this);
static bool ymString_clear  (ymString_t *this, size_t capacity);
static bool ymString_gets   (ymString_t *this);
static bool ymString_StrCat (ymString_t *this, const unsigned char str[]);
static bool ymString_StrCpy (ymString_t *this, const unsigned char str[]);
static bool ymString_append (ymString_t *this, unsigned char element);
static void ymString_destructor (ymString_t *this);

typedef struct {
    ymString_t*    (*init)      (ymString_t*, size_t);
    bool    (*const puts)       (const ymString_t*);
    bool    (*const empty)      (const ymString_t*);
    bool    (*const clear)      (ymString_t*, size_t);
    bool    (*const gets)       (ymString_t*);
    bool    (*const cat)        (ymString_t*, const unsigned char[]);
    bool    (*const cpy)        (ymString_t*, const unsigned char[]);
    bool    (*const append)     (ymString_t*, const unsigned char);
    void    (*const destruct)   (ymString_t*);
} ymString_fun;

extern const ymString_fun ymString;

/**
* @brief StringUtil对象初始化
* @param this
* @param capacity  容量, 不知道给几就给0, 后面函数遇到容量不足会自动扩充
* @return
* @note    1. 进入此函数前必须将this提前置为NULL, 否则认为出现第2条的错误
* @note    2. 已经经过此函数初始化过的this禁止再次初始化, 否则造成内存泄漏(已处理此错误)
*/
static ymString_t *ymString_init (ymString_t * this, size_t capacity)
{
    if (this != NULL) goto ReInitError;
    else;
    this = (ymString_t *) malloc(sizeof (ymString_t));
    this->ptr = (unsigned char *) malloc( sizeof (unsigned char ) * (capacity + 1));
    if (this->ptr == NULL) goto MallocError;
    else;
    this->ptr = (unsigned char *) memset(this->ptr, '\0', capacity + 1);
    this->length = 0;
    this->capacity = capacity;
//    this->vptr = &StringUtilFunctions;
    return this;

    MallocError:
    fprintf(stderr, "Error %s %s : malloc error\n", __FILE__, __FUNCTION__ );
    goto finally;
    ReInitError:
    fprintf(stderr, "Error %s %s : do not ymString_init again\n", __FILE__, __FUNCTION__ );
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

/**
 * @brief String内字符全部置为'\0',并且可以重新设置容量
 * @param this
 * @param capacity  新的容量. \n 如果不希望改变原来的容量请使用this->capacity.
 * @return
 */
static bool ymString_clear (ymString_t *const this, size_t capacity)
{
    if (capacity != this->capacity) {
        if (this->ptr != NULL) {
            free(this->ptr);
            this->ptr = NULL;   // 可以不需要, 但是习惯上保留了
        }
        else;
        this->ptr = (unsigned char *) malloc( sizeof (unsigned char ) * (capacity + 1));
        if (this->ptr == NULL) goto MallocError;
        this->capacity = capacity;
    } else;
    this->ptr = (unsigned char *) memset(this->ptr, '\0', capacity + 1);
    this->length = 0;
    return true;

    MallocError:
    fprintf(stderr, "Error %s %s : malloc error\n", __FILE__, __FUNCTION__);
    return false;
}

/**
 * @brief 从stdin读取一行字符串(可读' ', 遇到'\\n'停止, 不读入'\\n')
 * @param this
 * @note    1. 如果读入串的长度大于this的容量, 那么容量被重设为读入串的长度, 否则容量不变
 * @note    2. 如果this里原本储存有字符串, 原内容会被擦除
 */
static bool ymString_gets (ymString_t *const this)
{
    char *p = (char *) malloc(sizeof (char) * 1000);
    if (p == NULL) goto MallocError;
    else;
    scanf("%[^\n]", p);
    size_t len = strlen(p);
    if (len > this->capacity) {
        ymString_clear(this, len);
    } else {
        ymString_clear(this, this->capacity);
    }
    this->length = len;
    for (int i = 0; i < len; ++i) {
        *(this->ptr + i) = *(p + i);
    }
    free(p);
    p = NULL;
    return true;

    MallocError:
    fprintf(stderr, "Error %s %s: MallocError", __FILE__, __FUNCTION__ );
    exit(EXIT_FAILURE);
}

/**
 * @brief 输出储存的字符串
 * @param this
 * @return
 */
static bool ymString_puts (const ymString_t *const this)
{
    if (ymString_empty(this)) goto EmptyError;
    else;
    puts( (char *)this->ptr);
    return true;
    EmptyError:
    fprintf( stderr, "Error %s %s: ymString_empty string error", __FILE__, __FUNCTION__ );
    return false;
}

static bool ymString_empty(const ymString_t *const this)
{
    return !this->length;
}

static bool ymString_StrCat (ymString_t *this, const unsigned char str[])
{
    size_t str_length = strlen( (char *) str);
    size_t tot_length = this->length + str_length;
    if (tot_length > this->capacity) {
        this->ptr = (unsigned char *) realloc(this->ptr, sizeof (unsigned char) * (tot_length + 1));
        if (this->ptr == NULL) goto ReAllocError;
        else;
        this->capacity = tot_length;
    } else;
    strcat( (char *) this->ptr, (char *) str);
    this->length = tot_length;
    return true;

    ReAllocError:
    fprintf( stderr, "Error %s %s: reallocate error", __FILE__, __FUNCTION__);
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

static bool ymString_StrCpy (ymString_t *this, const unsigned char str[])
{
    size_t str_length = strlen((char*)str);
    if (str_length > this->capacity) {
        free(this->ptr);
        this->ptr = (unsigned char *) malloc(sizeof (unsigned char ) * (str_length + 1));
        if (this->ptr == NULL) goto MallocError;
        memset(this->ptr, '\0', str_length + 1);
        this->capacity = str_length;
    }
    strcpy((char*)this->ptr, (char*)str);
    this->length = str_length;
    return true;

    MallocError:
    fprintf( stderr, "Error %s %s: malloc error", __FILE__, __FUNCTION__);
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

static bool ymString_append (ymString_t *const this, const unsigned char element)
{
    if (this->length == this->capacity) {
        this->ptr = (unsigned char *) realloc(
                this->ptr,
                sizeof (unsigned char ) * (this->capacity + STRING_DEFAULT_DELTA_CAPACITY)
                );
        if (this->ptr == NULL) goto ReAllocError;
        else this->capacity += STRING_DEFAULT_DELTA_CAPACITY;
        for (size_t i = this->length + 1; i < this->capacity + 1; ++i) {
            *(this->ptr + i) = '\0';
        }
    }
    *(this->ptr + this->length) = element;
    this->length += 1;
    return true;

    ReAllocError:
    fprintf( stderr, "Error %s %s: reallocate error\n", __FILE__, __FUNCTION__ );
    goto finally;
    finally:
    exit(EXIT_FAILURE);
}

/**
 * @brief 析构函数, 释放资源
 * @param this
 */
static void ymString_destructor (ymString_t *this)
{
    if (this != NULL) {
        if (this->ptr != NULL) {
            free(this->ptr);
            this->ptr = NULL;
        } else;
//        if (this->vptr != NULL) {
//            this->vptr = NULL;
//        }
        free(this);
        this = NULL;
    }
}

#endif //DATASTRUCT01_YMSTRING_H
