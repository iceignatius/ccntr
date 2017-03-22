/**
 * @file
 * @brief     Container: queue (template).
 * @author    王文佑
 * @date      2017/03/22
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_QUEUE_TEMPLATE_H_
#define _CCNTR_QUEUE_TEMPLATE_H_

#include "ccntr_man_queue.h"

#ifdef CCNTR_MAN_QUEUE_ENABLED

#define CCNTR_DECLARE_QUEUE(clsname, valtype, release_value)                    \
                                                                                \
                                                                                \
                                                                                \
                                                                                \
typedef struct clsname##_t                                                      \
{                                                                               \
    ccntr_man_queue_t super;                                                    \
} clsname##_t;                                                                  \
                                                                                \
static inline                                                                   \
void clsname##_init(clsname##_t *self)                                          \
{                                                                               \
    ccntr_man_queue_init(&self->super, release_value);                          \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_destroy(clsname##_t *self)                                       \
{                                                                               \
    ccntr_man_queue_destroy(&self->super);                                      \
}                                                                               \
                                                                                \
static inline                                                                   \
unsigned clsname##_get_count(const clsname##_t *self)                           \
{                                                                               \
    return ccntr_man_queue_get_count(&self->super);                             \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_get_current(clsname##_t *self)                                \
{                                                                               \
    return (valtype) ccntr_man_queue_get_current(&self->super);                 \
}                                                                               \
                                                                                \
static inline                                                                   \
const valtype clsname##_get_current_c(const clsname##_t *self)                  \
{                                                                               \
    return (const valtype) ccntr_man_queue_get_current_c(&self->super);         \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_push(clsname##_t *self, valtype value)                           \
{                                                                               \
    ccntr_man_queue_push(&self->super, (void*) value);                          \
}                                                                               \
                                                                                \
static inline                                                                   \
valtype clsname##_pop(clsname##_t *self)                                        \
{                                                                               \
    return (valtype) ccntr_man_queue_pop(&self->super);                         \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_erase_current(clsname##_t *self)                                 \
{                                                                               \
    ccntr_man_queue_erase_current(&self->super);                                \
}                                                                               \
                                                                                \
static inline                                                                   \
void clsname##_clear(clsname##_t *self)                                         \
{                                                                               \
    ccntr_man_queue_clear(&self->super);                                        \
}

#endif  // CCNTR_MAN_QUEUE_ENABLED

#endif
