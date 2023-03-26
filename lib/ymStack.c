#include "ymStack.h"


#define ymStack_fun_instantiation(typename) const \
ymStack_fun(typename) ymStack(typename) = {       \
    .init   =   ymStack_init    (typename),       \
    .push   =   ymStack_push    (typename),       \
    .empty  =   ymStack_empty   (typename),       \
    .top    =   ymStack_top     (typename),       \
    .pop    =   ymStack_pop     (typename),       \
    .destruct = ymStack_destruct(typename),       \
}

ymStack_fun_instantiation(uint8);
ymStack_fun_instantiation(double);
