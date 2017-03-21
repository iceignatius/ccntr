/**
 * @file
 * @brief     Container: key map (template).
 * @author    王文佑
 * @date      2017/03/20
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAP_TEMPLATE_H_
#define _CCNTR_MAP_TEMPLATE_H_

#include "ccntr_man_map.h"

#warning Temporary hide!
//#ifdef CCNTR_MAN_MAP_ENABLED

#define CCNTR_DECLARE_MAP(clsname, keytype, valtype, compare, release_key, release_value) \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_iter_t                                                 \
{                                                                               \
    ccntr_man_map_iter_t super;                                                 \
} clsname##_iter_t;                                                             \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_iter_init(ccntr_man_map_iter_t src)                  \
{                                                                               \
    clsname##_iter_t iter = {src};                                              \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
bool clsname##_iter_have_value(const clsname##_iter_t *self)                    \
{                                                                               \
    return ccntr_man_map_iter_have_value(&self->super);                         \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_iter_move_prev(clsname##_iter_t *self)                           \
{                                                                               \
    ccntr_man_map_iter_move_prev(&self->super);                                 \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_iter_move_next(clsname##_iter_t *self)                           \
{                                                                               \
    ccntr_man_map_iter_move_next(&self->super);                                 \
}                                                                               \
                                                                                \
static inline                                                                   \
const keytype clsname##_iter_get_key(const clsname##_iter_t *self)              \
{                                                                               \
    return (const keytype) ccntr_man_map_iter_get_key(&self->super);            \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_iter_get_value(clsname##_iter_t *self)                        \
{                                                                               \
    return (valtype) ccntr_man_map_iter_get_value(&self->super);                \
}                                                                               \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_citer_t                                                \
{                                                                               \
    ccntr_man_map_citer_t super;                                                \
} clsname##_citer_t;                                                            \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_citer_init(ccntr_man_map_citer_t src)               \
{                                                                               \
    clsname##_citer_t iter = {src};                                             \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
bool clsname##_citer_have_value(const clsname##_citer_t *self)                  \
{                                                                               \
    return ccntr_man_map_citer_have_value(&self->super);                        \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_citer_move_prev(clsname##_citer_t *self)                         \
{                                                                               \
    ccntr_man_map_citer_move_prev(&self->super);                                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_citer_move_next(clsname##_citer_t *self)                         \
{                                                                               \
    ccntr_man_map_citer_move_next(&self->super);                                \
}                                                                               \
                                                                                \
static inline                                                                   \
const keytype clsname##_citer_get_key(const clsname##_citer_t *self)            \
{                                                                               \
    return (const keytype) ccntr_man_map_citer_get_key(&self->super);           \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_citer_(const clsname##_citer_t *self)                   \
{                                                                               \
    return (const valtype) ccntr_man_map_citer_get_value(&self->super);         \
}                                                                               \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_t                                                      \
{                                                                               \
    ccntr_man_map_t super;                                                      \
} clsname##_t;                                                                  \
                                                                                \
static inline                                                                   \
void clsname##_init(clsname##_t *self)                                          \
{                                                                               \
    ccntr_man_map_init(&self->super, compare, release_key, release_value);      \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_destroy(clsname##_t *self)                                       \
{                                                                               \
    ccntr_man_map_destroy(&self->super);                                        \
}                                                                               \
                                                                                \
static inline                                                                   \
unsigned clsname##_get_count(const clsname##_t *self)                           \
{                                                                               \
    return ccntr_man_map_get_count(&self->super);                               \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_get_first(clsname##_t *self)                         \
{                                                                               \
    return clsname##_iter_init(ccntr_man_map_get_first(&self->super));          \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_get_first_c(const clsname##_t *self)                \
{                                                                               \
    return clsname##_citer_init(ccntr_man_map_get_first_c(&self->super));       \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_get_last(clsname##_t *self)                          \
{                                                                               \
    return clsname##_iter_init(ccntr_man_map_get_last(&self->super));           \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_get_last_c(const clsname##_t *self)                 \
{                                                                               \
    return clsname##_citer_init(ccntr_man_map_get_last_c(&self->super));        \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_find(clsname##_t *self, const keytype key)           \
{                                                                               \
    return clsname##_iter_init(ccntr_man_map_find(&self->super, (const void*)key)); \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_find_c(const clsname##_t *self, const keytype key)  \
{                                                                               \
    return clsname##_citer_init(ccntr_man_map_find_c(&self->super, (const void*)key)); \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_find_value(clsname##_t *self, const keytype key)              \
{                                                                               \
    return (valtype) ccntr_man_map_find_value(&self->super, (const void*)key);  \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_find_value_c(const clsname##_t *self, const keytype key) \
{                                                                               \
    return (const valtype) ccntr_man_map_find_calue_c(&self->super, (const void*)key); \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_insert(clsname##_t *self, keytype key, valtype value)            \
{                                                                               \
    ccntr_man_map_insert(&self->super, (void*)key, (void*)value);               \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase(clsname##_t *self, clsname##_iter_t *pos)                  \
{                                                                               \
    ccntr_man_map_erase(&self->super, &pos->super);                             \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase_by_key(clsname##_t *self, const keytype key)               \
{                                                                               \
    ccntr_man_map_erase_by_key(&self->super, (const void*)key);                 \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_clear(clsname##_t *self)                                         \
{                                                                               \
    ccntr_man_map_clear(&self->super);                                          \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_pop(clsname##_t *self, clsname##_iter_t *pos)                 \
{                                                                               \
    return (valtype) ccntr_man_map_pop(&self->super, &pos->super);              \
}

#warning Temporary hide!
//#endif  // CCNTR_MAN_MAP_ENABLED

#endif
