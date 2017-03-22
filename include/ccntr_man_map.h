/**
 * @file
 * @brief     Container: key map (memory managed).
 * @author    王文佑
 * @date      2017/03/20
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_MAP_H_
#define _CCNTR_MAN_MAP_H_

#include "ccntr_config.h"
#include "ccntr_map.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_MAN_MAP_ENABLED

/**
 * @class ccntr_man_map_iter_t
 * @brief Iterator of key map.
 */
typedef struct ccntr_man_map_iter_t
{
    struct ccntr_man_map_t *container;
    ccntr_map_node_t       *node;
} ccntr_man_map_iter_t;

static inline
void ccntr_man_map_iter_init(ccntr_man_map_iter_t   *self,
                             struct ccntr_man_map_t *container,
                             ccntr_map_node_t       *node)
{
    self->container = container;
    self->node      = node;
}

static inline
bool ccntr_man_map_iter_have_value(const ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
    return self->node;
}

static inline
void ccntr_man_map_iter_move_prev(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = ccntr_map_node_get_prev(self->node);
}

static inline
void ccntr_man_map_iter_move_next(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = ccntr_map_node_get_next(self->node);
}

static inline
void* ccntr_man_map_iter_get_key(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Get key.
     *
     * @param self Object instance.
     * @return The key be pointed by the iterator.
     *
     * @attention Do NOT modify the key directly, except
     *            the key (and value) has already be popped from the container.
     *            If you want to modify a key in the container,
     *            you will be need to erase or pop it, then modify it, then insert again.
     */
    return self->node ? self->node->key : NULL;
}

void* ccntr_man_map_iter_get_value(ccntr_man_map_iter_t *self);

/**
 * @class ccntr_man_map_citer_t
 * @brief Constant iterator of key map.
 */
typedef struct ccntr_man_map_citer_t
{
    const struct ccntr_man_map_t *container;
    const ccntr_map_node_t       *node;
} ccntr_man_map_citer_t;

static inline
void ccntr_man_map_citer_init(ccntr_man_map_citer_t        *self,
                              const struct ccntr_man_map_t *container,
                              const ccntr_map_node_t       *node)
{
    self->container = container;
    self->node      = node;
}

static inline
bool ccntr_man_map_citer_have_value(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
    return self->node;
}

static inline
void ccntr_man_map_citer_move_prev(ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = ccntr_map_node_get_prev_c(self->node);
}

static inline
void ccntr_man_map_citer_move_next(ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = ccntr_map_node_get_next_c(self->node);
}

static inline
const void* ccntr_man_map_citer_get_key(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Get key.
     *
     * @param self Object instance.
     * @return The key be pointed by the iterator.
     */
    return self->node ? self->node->key : NULL;
}

const void* ccntr_man_map_citer_get_value(const ccntr_man_map_citer_t *self);

/**
 * @brief Release key.
 * @details Callback that will be called when container want release a key.
 *
 * @param key The key to be released.
 */
typedef void(*ccntr_man_map_release_key_t)(void *key);

/**
 * @brief Release value.
 * @details Callback that will be called when container want release a value.
 *
 * @param value The value to be released.
 */
typedef void(*ccntr_man_map_release_value_t)(void *value);

/**
 * @class ccntr_man_map_t
 * @brief Key map container.
 */
typedef struct ccntr_man_map_t
{
    ccntr_map_t super;

    ccntr_man_map_release_key_t   release_key;
    ccntr_man_map_release_value_t release_value;

} ccntr_man_map_t;

void ccntr_man_map_init(ccntr_man_map_t              *self,
                        ccntr_map_compare_keys_t      compare,
                        ccntr_man_map_release_key_t   release_key,
                        ccntr_man_map_release_value_t release_value);
void ccntr_man_map_destroy(ccntr_man_map_t *self);

static inline
unsigned ccntr_man_map_get_count(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get count of values it contained.
     *
     * @param self Object instance.
     * @return The count of values.
     */
    return ccntr_map_get_count(&self->super);
}

static inline
ccntr_man_map_iter_t ccntr_man_map_get_first(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_map_iter_t iter;
    ccntr_man_map_iter_init(&iter, self, ccntr_map_get_first(&self->super));

    return iter;
}

static inline
ccntr_man_map_citer_t ccntr_man_map_get_first_c(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_map_citer_t iter;
    ccntr_man_map_citer_init(&iter, self, ccntr_map_get_first_c(&self->super));

    return iter;
}

static inline
ccntr_man_map_iter_t ccntr_man_map_get_last(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_map_iter_t iter;
    ccntr_man_map_iter_init(&iter, self, ccntr_map_get_last(&self->super));

    return iter;
}

static inline
ccntr_man_map_citer_t ccntr_man_map_get_last_c(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_map_citer_t iter;
    ccntr_man_map_citer_init(&iter, self, ccntr_map_get_last_c(&self->super));

    return iter;
}

static inline
ccntr_man_map_iter_t ccntr_man_map_find(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return An iterator be pointed to the value if found;
     *         or an empty iterator if not found.
     */
    ccntr_man_map_iter_t iter;
    ccntr_man_map_iter_init(&iter, self, ccntr_map_find(&self->super, key));

    return iter;
}

static inline
ccntr_man_map_citer_t ccntr_man_map_find_c(const ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return An iterator be pointed to the value if found;
     *         or an empty iterator if not found.
     */
    ccntr_man_map_citer_t iter;
    ccntr_man_map_citer_init(&iter, self, ccntr_map_find_c(&self->super, key));

    return iter;
}

void* ccntr_man_map_find_value(ccntr_man_map_t *self, const void *key);
const void* ccntr_man_map_find_value_c(const ccntr_man_map_t *self, const void *key);

void ccntr_man_map_insert(ccntr_man_map_t *self, void *key, void *value);
void ccntr_man_map_erase(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos);
void ccntr_man_map_erase_by_key(ccntr_man_map_t *self, const void *key);
void ccntr_man_map_clear(ccntr_man_map_t *self);

void* ccntr_man_map_pop(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos);

#endif  // CCNTR_MAN_MAP_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
