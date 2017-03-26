/**
 * @file
 * @brief     Container: linked list.
 * @author    王文佑
 * @date      2017/03/03
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_LIST_H_
#define _CCNTR_LIST_H_

#include <stddef.h>
#include "ccntr_spinlock.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_list_node_t
 * @brief Node of linked list.
 */
typedef struct ccntr_list_node_t
{
    struct ccntr_list_node_t *prev;     ///< The previous node. READ ONLY for user!
    struct ccntr_list_node_t *next;     ///< The next node. READ ONLY for user!
} ccntr_list_node_t;

/**
 * @class ccntr_list_t
 * @brief Linked list container.
 */
typedef struct ccntr_list_t
{
    ccntr_list_node_t *first;
    ccntr_list_node_t *last;
    unsigned           count;

    CCNTR_DECLARE_SPINLOCK(lock);

} ccntr_list_t;

static inline
void ccntr_list_init(ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Constructor.
     *
     * @param self Object instance.
     */
    self->first = NULL;
    self->last  = NULL;
    self->count = 0;

    ccntr_spinlock_init(&self->lock);
}

static inline
unsigned ccntr_list_get_count(const ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    ccntr_spinlock_lock( (ccntr_spinlock_t*) &self->lock );
    unsigned count = self->count;
    ccntr_spinlock_unlock( (ccntr_spinlock_t*) &self->lock );

    return count;
}

static inline
ccntr_list_node_t* ccntr_list_get_first(ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Get the first node.
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     */
    ccntr_spinlock_lock(&self->lock);
    ccntr_list_node_t *node = self->first;
    ccntr_spinlock_unlock(&self->lock);

    return node;
}

static inline
const ccntr_list_node_t* ccntr_list_get_first_c(const ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Get the first node.
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     */
    return ccntr_list_get_first((ccntr_list_t*)self);
}

static inline
ccntr_list_node_t* ccntr_list_get_last(ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     */
    ccntr_spinlock_lock(&self->lock);
    ccntr_list_node_t *node = self->last;
    ccntr_spinlock_unlock(&self->lock);

    return node;
}

static inline
const ccntr_list_node_t* ccntr_list_get_last_c(const ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     */
    return ccntr_list_get_last((ccntr_list_t*)self);
}

void ccntr_list_link  (ccntr_list_t *self, ccntr_list_node_t *pos, ccntr_list_node_t *node);
void ccntr_list_unlink(ccntr_list_t *self, ccntr_list_node_t *node);

static inline
void ccntr_list_link_first(ccntr_list_t *self, ccntr_list_node_t *node)
{
    /**
     * @memberof ccntr_list_t
     * @brief Link a node to the first position.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     */
    return ccntr_list_link(self, ccntr_list_get_first(self), node);
}

static inline
void ccntr_list_link_last(ccntr_list_t *self, ccntr_list_node_t *node)
{
    /**
     * @memberof ccntr_list_t
     * @brief Link a node to the last position.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     */
    return ccntr_list_link(self, NULL, node);
}

static inline
void ccntr_list_discard_all(ccntr_list_t *self)
{
    /**
     * @memberof ccntr_list_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    ccntr_spinlock_lock(&self->lock);

    self->first = NULL;
    self->last  = NULL;
    self->count = 0;

    ccntr_spinlock_unlock(&self->lock);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
