#ifndef OPTIONS_H
#define OPTIONS_H

#include "raylib.h"

typedef struct {
    bool is_some;
    union {
        Vector2 value;
    };
} OptionVector2;

#define NONE_VECTOR2 ((OptionVector2){.is_some = false})
#define SOME_VECTOR2(...) \
    ((OptionVector2){.is_some = true, .value = (__VA_ARGS__)})

#endif  // !OPTIONS_H
