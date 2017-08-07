/**
 * @file
 * @brief     Container: array (template).
 * @author    王文佑
 * @date      2017/08/06
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_ARRAY_TEMPLATE_H_
#define _CCNTR_ARRAY_TEMPLATE_H_

#include "ccntr_man_array.h"

#ifdef CCNTR_MAN_ARRAY_ENABLED

#define CCNTR_DECLARE_ARRAY(clsname, valtype, compare, release_value)           \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_t                                                      \
{                                                                               \
    ccntr_man_array_t super;                                                    \
} clsname##_t;                                                                  \
                                                                                \
static inline                                                                   \
void clsname##_init(clsname##_t *self)                                          \
{                                                                               \
    ccntr_man_array_init(&self->super, compare, release_value);                 \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_destroy(clsname##_t *self)                                       \
{                                                                               \
    ccntr_man_array_destroy(&self->super);                                      \
}                                                                               \
                                                                                \
static inline                                                                   \
unsigned clsname##_get_count(const clsname##_t *self)                           \
{                                                                               \
    return ccntr_man_array_get_count(&self->super);                             \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype* clsname##_baseptr(clsname##_t *self)                                   \
{                                                                               \
    return (valtype*) ccntr_man_array_baseptr(&self->super);                    \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype* clsname##_baseptr_c(const clsname##_t *self)                     \
{                                                                               \
    return (const valtype*) ccntr_man_array_baseptr_c(&self->super);            \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_get_first(clsname##_t *self)                                  \
{                                                                               \
    return (valtype) ccntr_man_array_get_first(&self->super);                   \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_get_first_c(const clsname##_t *self)                    \
{                                                                               \
    return (valtype) ccntr_man_array_get_first_c(&self->super);                 \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_get_last(clsname##_t *self)                                   \
{                                                                               \
    return (valtype) ccntr_man_array_get_last(&self->super);                    \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_get_last_c(const clsname##_t *self)                     \
{                                                                               \
    return (valtype) ccntr_man_array_get_last_c(&self->super);                  \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_get(clsname##_t *self, unsigned index)                        \
{                                                                               \
    return (valtype) ccntr_man_array_get(&self->super, index);                  \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_get_c(const clsname##_t *self, unsigned index)          \
{                                                                               \
    return (valtype) ccntr_man_array_get_c(&self->super, index);                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_insert(clsname##_t *self, unsigned index, valtype value)         \
{                                                                               \
    ccntr_man_array_insert(&self->super, index, (void*) value);                 \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_insert_first(clsname##_t *self, valtype value)                   \
{                                                                               \
    ccntr_man_array_insert_first(&self->super, (void*) value);                  \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_insert_last(clsname##_t *self, valtype value)                    \
{                                                                               \
    ccntr_man_array_insert_last(&self->super, (void*) value);                   \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_replace(clsname##_t *self, unsigned index, void *value)          \
{                                                                               \
    ccntr_man_array_replace(&self->super, index, (void*) value);                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase(clsname##_t *self, unsigned index)                         \
{                                                                               \
    ccntr_man_array_erase(&self->super, index);                                 \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase_first(clsname##_t *self)                                   \
{                                                                               \
    ccntr_man_array_erase_first(&self->super);                                  \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase_last(clsname##_t *self)                                    \
{                                                                               \
    ccntr_man_array_erase_last(&self->super);                                   \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_clear(clsname##_t *self)                                         \
{                                                                               \
    ccntr_man_array_clear(&self->super);                                        \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_sort(clsname##_t *self)                                          \
{                                                                               \
    ccntr_man_array_sort(&self->super);                                         \
}

#endif  // CCNTR_MAN_ARRAY_ENABLED

#endif
