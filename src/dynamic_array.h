#pragma once
#include <stddef.h>

/// Starting size of a dynamic array, redefine to customize if needed.
#define DYNAMIC_ARRAY_STARTING_CAPACITY 4

/// Macro that appends and item to an array, it reallocates the array automatically if needed.
/// @param array Struct with the following fields: "items", "count", "capacity".
/// @param item The item to add to the array
#define DYNAMIC_ARRAY_APPEND(array, item)\
    do {\
        if (array.count >= array.capacity) {\
            if (array.capacity == 0) array.capacity = DYNAMIC_ARRAY_STARTING_CAPACITY;\
            else array.capacity *= 2;\
            \
            array.items = realloc(array.items, array.capacity * sizeof(*array.items)); /* NOLINT(*-suspicious-realloc-usage) */ \
            }\
        array.items[array.count++] = item;\
    } while (false)
