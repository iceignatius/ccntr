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

static inline
void ccntr_stack_init(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Constructor.
     *
     * @param self Object instance.
     */
    self->top   = NULL;
    self->count = 0;
}

static inline
unsigned ccntr_stack_get_count(const ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    return self->count;
}

static inline
ccntr_stack_node_t* ccntr_stack_get_current(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->top;
}

static inline
const ccntr_stack_node_t* ccntr_stack_get_current_c(const ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->top;
}

void ccntr_stack_link(ccntr_stack_t *self, ccntr_stack_node_t *node);
ccntr_stack_node_t* ccntr_stack_unlink(ccntr_stack_t *self);

static inline
void ccntr_stack_discard_all(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    self->top   = NULL;
    self->count = 0;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
