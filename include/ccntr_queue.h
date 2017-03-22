/**
 * @file
 * @brief     Container: queue (first in, first out list).
 * @author    王文佑
 * @date      2017/03/22
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_QUEUE_H_
#define _CCNTR_QUEUE_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_queue_node_t
 * @brief Node of queue.
 */
typedef struct ccntr_queue_node_t
{
    struct ccntr_queue_node_t *next;
} ccntr_queue_node_t;

/**
 * @class ccntr_queue_t
 * @brief Queue container.
 */
typedef struct ccntr_queue_t
{
    ccntr_queue_node_t *first;
    ccntr_queue_node_t *last;
    unsigned            count;
} ccntr_queue_t;

static inline
void ccntr_queue_init(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Constructor.
     *
     * @param self Object instance.
     */
    self->first = NULL;
    self->last  = NULL;
    self->count = 0;
}

static inline
unsigned ccntr_queue_get_count(const ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    return self->count;
}

static inline
ccntr_queue_node_t* ccntr_queue_get_current(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->first;
}

static inline
const ccntr_queue_node_t* ccntr_queue_get_current_c(const ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->first;
}

void ccntr_queue_link(ccntr_queue_t *self, ccntr_queue_node_t *node);
ccntr_queue_node_t* ccntr_queue_unlink(ccntr_queue_t *self);

static inline
void ccntr_queue_discard_all(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    self->first = NULL;
    self->last  = NULL;
    self->count = 0;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
