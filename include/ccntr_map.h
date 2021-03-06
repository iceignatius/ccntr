/**
 * @file
 * @brief     Container: key map.
 * @author    王文佑
 * @date      2017/03/13
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAP_H_
#define _CCNTR_MAP_H_

#include <stddef.h>
#include <stdbool.h>
#include "ccntr_spinlock.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_map_node_t
 * @brief Node of key map.
 */
typedef struct ccntr_map_node_t
{
    struct ccntr_map_node_t *parent;
    struct ccntr_map_node_t *left;
    struct ccntr_map_node_t *right;

    bool is_red;

    /**
     * Key of the node.
     *
     * @attention:
     * The @a key member need to be set and managed by user manually,
     * but do not modify it when the node is linked in a container.
     */
    void *key;

} ccntr_map_node_t;

ccntr_map_node_t* ccntr_map_node_get_next(ccntr_map_node_t *self);
ccntr_map_node_t* ccntr_map_node_get_prev(ccntr_map_node_t *self);
ccntr_map_node_t* ccntr_map_node_get_next_postorder(ccntr_map_node_t *self);

static inline
const ccntr_map_node_t* ccntr_map_node_get_next_c(const ccntr_map_node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node (in order).
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the smaller key to the larger.
     */
    return ccntr_map_node_get_next((ccntr_map_node_t*)self);
}

static inline
const ccntr_map_node_t* ccntr_map_node_get_prev_c(const ccntr_map_node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the previous node (in order).
     *
     * @param self Object instance.
     * @return The previous node; or NULL if there does not have the previous node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the larger key to the smaller.
     */
    return ccntr_map_node_get_prev((ccntr_map_node_t*)self);
}

/**
 * A function that compare two keys.
 *
 * @param key1 The first key.
 * @param key2 The second key.
 * @retval NEGATIVE The first key goes before the second key.
 * @retval ZERO     The first key is equivalent to the second key.
 * @retval POSITIVE The first key goes after the second key.
 */
typedef int(*ccntr_map_compare_keys_t)(const void *key1, const void *key2);

/**
 * @class ccntr_map_t
 * @brief Key map container.
 */
typedef struct ccntr_map_t
{
    ccntr_map_node_t *root;
    unsigned          count;

    ccntr_map_compare_keys_t compare;

    CCNTR_DECLARE_SPINLOCK(lock);

} ccntr_map_t;

void ccntr_map_init(ccntr_map_t *self, ccntr_map_compare_keys_t compare);

static inline
unsigned ccntr_map_get_count(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
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

ccntr_map_node_t* ccntr_map_get_first(ccntr_map_t *self);
ccntr_map_node_t* ccntr_map_get_last(ccntr_map_t *self);
ccntr_map_node_t* ccntr_map_get_first_postorder(ccntr_map_t *self);

static inline
const ccntr_map_node_t* ccntr_map_get_first_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node (in order).
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the first node will have the smallest key.
     */
    return ccntr_map_get_first((ccntr_map_t*)self);
}

static inline
const ccntr_map_node_t* ccntr_map_get_last_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the last node will have the largest key.
     */
    return ccntr_map_get_last((ccntr_map_t*)self);
}

ccntr_map_node_t* ccntr_map_find(ccntr_map_t *self, const void *key);

static inline
const ccntr_map_node_t* ccntr_map_find_c(const ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find node by key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    return ccntr_map_find((ccntr_map_t*)self, key);
}

ccntr_map_node_t* ccntr_map_find_nearest_less(ccntr_map_t *self, const void *key);

static inline
const ccntr_map_node_t* ccntr_map_find_nearest_less_c(const ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find nearest node which is less or equal than the specified key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    return ccntr_map_find_nearest_less((ccntr_map_t*)self, key);
}

ccntr_map_node_t* ccntr_map_find_nearest_great(ccntr_map_t *self, const void *key);

static inline
const ccntr_map_node_t* ccntr_map_find_nearest_great_c(const ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find nearest node which is greater or equal than the specified key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    return ccntr_map_find_nearest_great((ccntr_map_t*)self, key);
}

ccntr_map_node_t* ccntr_map_link(ccntr_map_t *self, ccntr_map_node_t *node);
void ccntr_map_unlink(ccntr_map_t *self, ccntr_map_node_t *node);
ccntr_map_node_t* ccntr_map_unlink_by_key(ccntr_map_t *self, const void *key);

static inline
void ccntr_map_discard_all(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    ccntr_spinlock_lock(&self->lock);

    self->root  = NULL;
    self->count = 0;

    ccntr_spinlock_unlock(&self->lock);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
