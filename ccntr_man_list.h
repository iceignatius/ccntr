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
#include "ccntr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_man_list_iter_t
 * @brief Iterator of linked list.
 */
typedef struct ccntr_man_list_iter_t
{
    struct ccntr_man_list_t *container;
    ccntr_list_node_t       *node;
} ccntr_man_list_iter_t;

void ccntr_man_list_iter_init(ccntr_man_list_iter_t   *self,
                              struct ccntr_man_list_t *container,
                              ccntr_list_node_t       *node);

bool  ccntr_man_list_iter_have_value(const ccntr_man_list_iter_t *self);
void* ccntr_man_list_iter_get_value (      ccntr_man_list_iter_t *self);
void  ccntr_man_list_iter_move_prev (      ccntr_man_list_iter_t *self);
void  ccntr_man_list_iter_move_next (      ccntr_man_list_iter_t *self);

/**
 * @class ccntr_man_list_citer_t
 * @brief Constant iterator of linked list.
 */
typedef struct ccntr_man_list_citer_t
{
    const struct ccntr_man_list_t *container;
    const ccntr_list_node_t       *node;
} ccntr_man_list_citer_t;

void ccntr_man_list_citer_init(ccntr_man_list_citer_t        *self,
                               const struct ccntr_man_list_t *container,
                               const ccntr_list_node_t       *node);

bool        ccntr_man_list_citer_have_value(const ccntr_man_list_citer_t *self);
const void* ccntr_man_list_citer_get_value (const ccntr_man_list_citer_t *self);
void        ccntr_man_list_citer_move_prev (      ccntr_man_list_citer_t *self);
void        ccntr_man_list_citer_move_next (      ccntr_man_list_citer_t *self);

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

unsigned ccntr_man_list_get_count(const ccntr_man_list_t *self);

ccntr_man_list_iter_t  ccntr_man_list_get_first  (      ccntr_man_list_t *self);
ccntr_man_list_citer_t ccntr_man_list_get_first_c(const ccntr_man_list_t *self);
ccntr_man_list_iter_t  ccntr_man_list_get_last   (      ccntr_man_list_t *self);
ccntr_man_list_citer_t ccntr_man_list_get_last_c (const ccntr_man_list_t *self);

void ccntr_man_list_push_front(ccntr_man_list_t *self, void *value);
void ccntr_man_list_push_back (ccntr_man_list_t *self, void *value);
void ccntr_man_list_pop_first (ccntr_man_list_t *self);
void ccntr_man_list_pop_last  (ccntr_man_list_t *self);

void ccntr_man_list_insert(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos, void *value);
void ccntr_man_list_erase (ccntr_man_list_t *self, ccntr_man_list_iter_t *pos);
void ccntr_man_list_clear (ccntr_man_list_t *self);

void* ccntr_man_list_unlink(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
