#include "ymString.h"

const ymString_fun ymString = {
        .init   = ymString_init,
        .puts   = ymString_puts,
        .empty  = ymString_empty,
        .clear  = ymString_clear,
        .gets   = ymString_gets,
        .cat    = ymString_StrCat,
        .cpy    = ymString_StrCpy,
        .reverse= ymString_reverse,
        .append = ymString_append,
        .delChar= ymString_DelChar,
        .destruct = ymString_destructor,
};
