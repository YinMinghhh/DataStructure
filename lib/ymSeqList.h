/**
 * @file    ymSeqList.h
 * @brief   实现了一个动态顺序表模板
 * @author  YinMing
 * @version 1.0
 * @date    2023-03-21
 */

#ifndef DATASTRUCT01_YMSEQLIST_H
#define DATASTRUCT01_YMSEQLIST_H

#pragma message ("compiling ymSeqList.h ...")

#include <stddef.h>
#include <errno.h>
#include <stdbool.h>



#ifndef SEQUENTIAL_LIST_DEFAULT_PARAMETERS
#define SEQUENTIAL_LIST_DEFAULT_PARAMETERS
#define SEQLIST_DEFAULT_CAPACITY        ((size_t) 10)
#define SEQLIST_DEFAULT_DELTA_CAPACITY  ((size_t) 5 )

#elif !(defined SEQLIST_DEFAULT_CAPACITY) || !(defined SEQLIST_DEFAULT_DELTA_CAPACITY)
#pragma message ("please define both SEQLIST_DEFAULT_CAPACITY and SEQLIST_DEFAULT_DELTA_CAPACITY")
#error Error SeqList: no DEFAULT PARAMENTERS

#endif



#define _SeqList(typename) SeqList$_##typename##_$
#define SeqList(typename) _SeqList(typename)

#define _SeqList_DEF(typename) typedef  \
struct SeqList$_##typename##_$ {        \
    typename *ptr;                      \
    size_t  size;                       \
    size_t  capacity;                   \
} SeqList(typename);                    \
                                        \
bool SeqList_init$_##typename##_$ (SeqList(typename) *const this) { \
    this->ptr = (typename *) malloc(sizeof (typename) * SEQLIST_DEFAULT_CAPACITY); \
    if (this->ptr == NULL) goto SeqListMallocError;                 \
    this->size = ((size_t) 0);          \
    this->capacity = SEQLIST_DEFAULT_CAPACITY;                      \
    return true;                        \
                                        \
    SeqListMallocError:                 \
    fprintf(stderr, "Value of errno: %d\n", errno);                 \
    fprintf(stderr, "Error SeqList_init(%s): malloc error: %s\n", #typename, strerror(errno)); \
    goto finally;                       \
                                        \
    finally:                            \
    exit(EXIT_FAILURE);                 \
}                                       \
                                        \
bool SeqList_insert$_##typename##_$ (SeqList(typename) *const this, typename element, size_t index) { \
    /** @param  index   位序, 相当于数组下标, 从 0 开始 */              \
    if (index < 0) goto SeqListIndexNegativeError;                  \
    if (index > this->size) goto SeqListIndexOverflowError;         \
                                        \
    if (this->size == this->capacity)   \
        this->ptr =                     \
        (typename *) realloc(this->ptr, sizeof (typename) * (this->capacity + SEQLIST_DEFAULT_DELTA_CAPACITY)); \
    if (this->ptr == NULL) goto SeqListReallocError;                \
                                        \
    for (size_t i = this->size; i > index; i--)                     \
        *(this->ptr + i) = *(this->ptr + i - 1);                    \
    *(this->ptr + index) = element;     \
    this->size += 1;                    \
    return true;                        \
                                        \
    SeqListReallocError:                \
    fprintf(stderr, "Value of errno: %d\n", errno);                 \
    fprintf(stderr, "Error SeqList_insert(%s): ReAlloc error: %s", #typename, strerror(errno));\
    goto finally;                       \
                                        \
    SeqListIndexNegativeError:          \
    fprintf(stderr, "Value of errno: %d\n", errno);                 \
    fprintf(stderr, "Error SeqList_insert(%s): index is negative: %s", #typename, strerror(errno)); \
    goto finally;                       \
                                        \
    SeqListIndexOverflowError:          \
    fprintf(stderr, "Error SeqList_insert(%s): index is overflow: %s", #typename, strerror(errno));  \
    goto finally;                       \
                                        \
    finally:                            \
    exit(EXIT_FAILURE);\
}                                       \
                                        \
bool SeqList_empty$_##typename##_$ (SeqList(typename) *const this) {\
    return this->size == 0;             \
}                                       \
                                        \

#define SeqList_def(typename) _SeqList_DEF(typename)

SeqList_def(int)

#endif //DATASTRUCT01_YMSEQLIST_H
