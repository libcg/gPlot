#ifndef COMMON_H
#define COMMON_H

#include <glib2d.h>

// Turn this to 'double' if you need slow accuracy.
typedef float FTYPE;

#define F_COLOR(n) \
    (n == 0 ? RED : \
    (n == 1 ? MAGENTA : \
    (n == 2 ? BLUE : \
    (n == 3 ? GREEN : \
    (0)))))

#endif
