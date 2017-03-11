/**
 * @file
 * @brief     Container: linked list (template).
 * @author    王文佑
 * @date      2017/03/03
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_LIST_TEMPLATE_H_
#define _CCNTR_LIST_TEMPLATE_H_

#include "ccntr_man_list.h"

#ifdef CCNTR_MAN_LIST_ENABLED

#define CCNTR_LIST_TEMPLATE(clsname, valtype, on_value_release)                 \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_iter_t                                                 \
{                                                                               \
    ccntr_man_list_iter_t super;                                                \
} clsname##_iter_t;                                                             \
                                                                                \
static inline                                                                   \
bool clsname##_iter_have_value(const clsname##_iter_t *self)                    \
{                                                                               \
    return ccntr_man_list_iter_have_value(&self->super);                        \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_iter_get_value(clsname##_iter_t *self)                        \
{                                                                               \
    return (valtype) ccntr_man_list_iter_get_value(&self->super);               \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_iter_move_prev(clsname##_iter_t *self)                           \
{                                                                               \
    ccntr_man_list_iter_move_prev(&self->super);                                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_iter_move_next(clsname##_iter_t *self)                           \
{                                                                               \
    ccntr_man_list_iter_move_next(&self->super);                                \
}                                                                               \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_citer_t                                                \
{                                                                               \
    ccntr_man_list_citer_t super;                                               \
} clsname##_citer_t;                                                            \
                                                                                \
static inline                                                                   \
bool clsname##_citer_have_value(const clsname##_citer_t *self)                  \
{                                                                               \
    return ccntr_man_list_citer_have_value(&self->super);                       \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_citer_get_value(const clsname##_citer_t *self)          \
{                                                                               \
    return (valtype) ccntr_man_list_citer_get_value(&self->super);              \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_citer_move_prev(clsname##_citer_t *self)                         \
{                                                                               \
    ccntr_man_list_citer_move_prev(&self->super);                               \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_citer_move_next(clsname##_citer_t *self)                         \
{                                                                               \
    ccntr_man_list_citer_move_next(&self->super);                               \
}                                                                               \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_t                                                      \
{                                                                               \
    ccntr_man_list_t super;                                                     \
} clsname##_t;                                                                  \
                                                                                \
static inline                                                                   \
void clsname##_init(clsname##_t *self)                                          \
{                                                                               \
    ccntr_man_list_init(&self->super, on_value_release);                        \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_destroy(clsname##_t *self)                                       \
{                                                                               \
    ccntr_man_list_destroy(&self->super);                                       \
}                                                                               \
                                                                                \
static inline                                                                   \
unsigned clsname##_get_count(const clsname##_t *self)                           \
{                                                                               \
    return ccntr_man_list_get_count(&self->super);                              \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_get_first(clsname##_t *self)                         \
{                                                                               \
    clsname##_iter_t iter;                                                      \
    iter.super = ccntr_man_list_get_first(&self->super);                        \
                                                                                \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_get_first_c(const clsname##_t *self)                \
{                                                                               \
    clsname##_citer_t iter;                                                     \
    iter.super = ccntr_man_list_get_first_c(&self->super);                      \
                                                                                \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_iter_t clsname##_get_last(clsname##_t *self)                          \
{                                                                               \
    clsname##_iter_t iter;                                                      \
    iter.super = ccntr_man_list_get_last(&self->super);                         \
                                                                                \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
clsname##_citer_t clsname##_get_last_c(const clsname##_t *self)                 \
{                                                                               \
    clsname##_citer_t iter;                                                     \
    iter.super = ccntr_man_list_get_last_c(&self->super);                       \
                                                                                \
    return iter;                                                                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_push_front(clsname##_t *self, valtype value)                     \
{                                                                               \
    ccntr_man_list_push_front(&self->super, (void*) value);                     \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_push_back(clsname##_t *self, valtype value)                      \
{                                                                               \
    ccntr_man_list_push_back(&self->super, (void*) value);                      \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_pop_first(clsname##_t *self)                                     \
{                                                                               \
    ccntr_man_list_pop_first(&self->super);                                     \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_pop_last(clsname##_t *self)                                      \
{                                                                               \
    ccntr_man_list_pop_last(&self->super);                                      \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_insert(clsname##_t *self, clsname##_iter_t *pos, valtype value)  \
{                                                                               \
    ccntr_man_list_insert(&self->super, &pos->super, (void*)value);             \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase(clsname##_t *self, clsname##_iter_t *pos)                  \
{                                                                               \
    ccntr_man_list_erase(&self->super, &pos->super);                            \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_clear(clsname##_t *self)                                         \
{                                                                               \
    ccntr_man_list_clear(&self->super);                                         \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_unlink(clsname##_t *self, clsname##_iter_t *pos)              \
{                                                                               \
    return (valtype) ccntr_man_list_unlink(&self->super, &pos->super);          \
}

#endif  // CCNTR_MAN_LIST_ENABLED

#endif
