/**
 * @file
 * @brief     Container: stack (last in, first out list).
 * @author    王文佑
 * @date      2017/03/22
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_STACK_H_
#define _CCNTR_STACK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_stack_node_t
 * @brief Node of stack.
 */
typedef struct ccntr_stack_node_t
{
    struct ccntr_stack_node_t *prev;
} ccntr_stack_node_t;

/**
 * @class ccntr_stack_t
 * @brief Stack container.
 */
typedef struct ccntr_stack_t
{
    ccntr_stack_node_t *top;
    unsigned            count;
} ccntr_stack_t;

void ccntr_stack_init(ccntr_stack_t *self);

unsigned ccntr_stack_get_count(const ccntr_stack_t *self);

ccntr_stack_node_t* ccntr_stack_get_current(ccntr_stack_t *self);
const ccntr_stack_node_t* ccntr_stack_get_current_c(const ccntr_stack_t *self);

void ccntr_stack_link(ccntr_stack_t *self, ccntr_stack_node_t *node);
ccntr_stack_node_t* ccntr_stack_unlink(ccntr_stack_t *self);

void ccntr_stack_discard_all(ccntr_stack_t *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
