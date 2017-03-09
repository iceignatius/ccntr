/**
 * @file
 * @brief     Container: linked list (memory managed).
 * @author    王文佑
 * @date      2017/03/03
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_LIST_H_
#define _CCNTR_MAN_LIST_H_

#include <stdbool.h>
#include "ccntr_config.h"
#include "ccntr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_MAN_LIST_ENABLED

/**
 * @class ccntr_man_list_iter_t
 * @brief Iterator of linked list.
 */
typedef struct ccntr_man_list_iter_t
{
    struct ccntr_man_list_t *container;
    ccntr_list_node_t       *node;
} ccntr_man_list_iter_t;

static inline
void ccntr_man_list_iter_init(ccntr_man_list_iter_t   *self,
                              struct ccntr_man_list_t *container,
                              ccntr_list_node_t       *node)
{
    self->container = container;
    self->node      = node;
}

static inline
bool ccntr_man_list_iter_have_value(const ccntr_man_list_iter_t *self)
{
    /**
     * @memberof ccntr_man_list_iter_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
    return self->node;
}

static inline
void ccntr_man_list_iter_move_prev(ccntr_man_list_iter_t *self)
{
    /**
     * @memberof ccntr_man_list_iter_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = self->node->prev;
}

static inline
void ccntr_man_list_iter_move_next(ccntr_man_list_iter_t *self)
{
    /**
     * @memberof ccntr_man_list_iter_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = self->node->next;
}

void* ccntr_man_list_iter_get_value(ccntr_man_list_iter_t *self);

/**
 * @class ccntr_man_list_citer_t
 * @brief Constant iterator of linked list.
 */
typedef struct ccntr_man_list_citer_t
{
    const struct ccntr_man_list_t *container;
    const ccntr_list_node_t       *node;
} ccntr_man_list_citer_t;

static inline
void ccntr_man_list_citer_init(ccntr_man_list_citer_t        *self,
                               const struct ccntr_man_list_t *container,
                               const ccntr_list_node_t       *node)
{
    self->container = container;
    self->node      = node;
}

static inline
bool ccntr_man_list_citer_have_value(const ccntr_man_list_citer_t *self)
{
    /**
     * @memberof ccntr_man_list_citer_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
    return self->node;
}

static inline
void ccntr_man_list_citer_move_prev(ccntr_man_list_citer_t *self)
{
    /**
     * @memberof ccntr_man_list_citer_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = self->node->prev;
}

static inline
void ccntr_man_list_citer_move_next(ccntr_man_list_citer_t *self)
{
    /**
     * @memberof ccntr_man_list_citer_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
    if( self->node )
        self->node = self->node->next;
}

const void* ccntr_man_list_citer_get_value(const ccntr_man_list_citer_t *self);

/**
 * @brief Release value.
 * @details Callback that will be called when container want release a value.
 *
 * @param value The value to be released.
 */
typedef void(*ccntr_man_list_on_value_release_t)(void *value);

/**
 * @class ccntr_list_t
 * @brief Linked list container.
 */
typedef struct ccntr_man_list_t
{
    ccntr_list_t super;

    ccntr_man_list_on_value_release_t on_value_release;

} ccntr_man_list_t;

void ccntr_man_list_init(ccntr_man_list_t                  *self,
                         ccntr_man_list_on_value_release_t  on_value_release);
void ccntr_man_list_destroy(ccntr_man_list_t *self);

static inline
unsigned ccntr_man_list_get_count(const ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Get count of values it contained.
     *
     * @param self Object instance.
     * @return The count of values.
     */
    return ccntr_list_get_count(&self->super);
}

static inline
ccntr_man_list_iter_t ccntr_man_list_get_first(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_list_iter_t iter;
    ccntr_man_list_iter_init(&iter, self, ccntr_list_get_first(&self->super));

    return iter;
}

static inline
ccntr_man_list_citer_t ccntr_man_list_get_first_c(const ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_list_citer_t iter;
    ccntr_man_list_citer_init(&iter, self, ccntr_list_get_first_c(&self->super));

    return iter;
}

static inline
ccntr_man_list_iter_t ccntr_man_list_get_last(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_list_iter_t iter;
    ccntr_man_list_iter_init(&iter, self, ccntr_list_get_last(&self->super));

    return iter;
}

static inline
ccntr_man_list_citer_t ccntr_man_list_get_last_c(const ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
    ccntr_man_list_citer_t iter;
    ccntr_man_list_citer_init(&iter, self, ccntr_list_get_last_c(&self->super));

    return iter;
}

void ccntr_man_list_push_front(ccntr_man_list_t *self, void *value);
void ccntr_man_list_push_back (ccntr_man_list_t *self, void *value);
void ccntr_man_list_pop_first (ccntr_man_list_t *self);
void ccntr_man_list_pop_last  (ccntr_man_list_t *self);

void ccntr_man_list_insert(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos, void *value);
void ccntr_man_list_erase (ccntr_man_list_t *self, ccntr_man_list_iter_t *pos);
void ccntr_man_list_clear (ccntr_man_list_t *self);

void* ccntr_man_list_unlink(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos);

#endif  // CCNTR_MAN_LIST_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
