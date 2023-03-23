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

#define STRING_DEFAULT_DELTA_CAPACITY  ((size_t) 5 )

struct StringUtil_functions;
typedef struct StringUtil_functions StringUtil_functions;

/**
 * @struct  StringUtil  实现了一个比较好用的String类型
 * @param   ptr         指向字符们被存储的区域
 * @param   length      表示储存的有效字符的个数, 不包括所有的'\0'
 * @param   capacity    表示可储存的有效字符的个数, 不包括最后的'\0', \n
 *                      所以ptr指向的区域最终占用的空间是 sizeof(unsigned char) * (capacity + 1)
 * @param   vptr        指向方法结构体
 * @author  YinMing
 */
typedef struct StringUtil {
    unsigned char *ptr;
    size_t length;
    size_t capacity;
    const StringUtil_functions *vptr;
} StringUtil;

static bool init(StringUtil *this, size_t capacity);
static bool StrPuts (const StringUtil *this);
static bool empty(const StringUtil *this);

struct StringUtil_functions {
    bool    (*init)     (StringUtil*, size_t);
    bool    (*puts)     (const StringUtil*);
    bool    (*empty)    (const StringUtil*);
} StringUtilFunctions = {
        .init   = init,
        .puts   = StrPuts,
        .empty  = empty,
};

static bool init (StringUtil *const this, size_t capacity)
{
    if (this->ptr != NULL) { free(this->ptr); this->ptr = NULL; }
    this->ptr = (unsigned char *) malloc( sizeof (unsigned char ) * (capacity + 1));
    if (this->ptr == NULL) goto MallocError;
    this->ptr = (unsigned char *) memset(this->ptr, '\0', capacity + 1);
    this->length = 0;
    this->capacity = capacity;
    this->vptr = &StringUtilFunctions;
    return true;

    MallocError:
    fprintf(stderr, "Error %s %s : malloc error\n", __FILE__, __FUNCTION__);
    exit(EXIT_FAILURE);
}

static bool clear (StringUtil *const this, size_t capacity)
{
    if (this->ptr != NULL) free(this->ptr);
    this->ptr = NULL;
    this->ptr = (unsigned char *) malloc( sizeof (unsigned char ) * (capacity + 1));
    if (this->ptr == NULL) goto MallocError;
    this->ptr = (unsigned char *) memset(this->ptr, '\0', capacity + 1);
    this->capacity = capacity;
    this->length = 0;
    return true;

    MallocError:
    fprintf(stderr, "Error %s %s : malloc error\n", __FILE__, __FUNCTION__);
    return false;
}

static void StrGets (StringUtil *const this)
{
    char *p = (char *) malloc(sizeof (char));
    if (p == NULL) goto MallocError;
    size_t len = strlen(p);
    clear(this, len);
    for (int i = 0; i < len; ++i) {
        *(this->ptr + i) = *p;
        p += 1;
    }
    free(p);
    p == NULL;
    return;

    MallocError:
    return;
}

static bool StrPuts (const StringUtil *const this)
{
    if (empty(this)) goto EmptyError;
    puts( (char *)this->ptr);
    return true;
    EmptyError:
    fprintf( stderr, "Error %s %s: empty string error", __FILE__, __FUNCTION__ );
    return false;
}

static bool empty(const StringUtil *const this)
{
    return !this->length;
}

static bool StrCat (StringUtil *const this, unsigned char str[])
{
    size_t str_length = strlen( (char *) str);
    size_t tot_length = this->length + str_length;
    if (tot_length > this->capacity) {
        this->ptr = (unsigned char *) realloc(this->ptr, sizeof (unsigned char) * (tot_length + 1));
        if (this->ptr == NULL) goto ReAllocError;
        this->capacity = tot_length;
    }
    strcat( (char *) this->ptr, (char *) str);
    this->length = tot_length;
    return true;

    ReAllocError:
    fprintf( stderr, "Error %s %s: reAlloc error", __FILE__, __FUNCTION__);
    exit(EXIT_FAILURE);
}

static bool append (StringUtil *const this, unsigned char element)
{
    ;
    return true;
}

#endif //DATASTRUCT01_YMSTRING_H
