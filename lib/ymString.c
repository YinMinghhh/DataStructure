#include "ymString.h"

const ymString_fun ymString = {
        .init   = ymString_init,
        .puts   = ymString_puts,
        .empty  = ymString_empty,
        .clear  = ymString_clear,
        .gets   = ymString_gets,
        .cat    = ymString_StrCat,
        .append = ymString_append,
        .destruct = ymString_destructor,
};