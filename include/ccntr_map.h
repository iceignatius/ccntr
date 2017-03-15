/**
 * @file
 * @brief     Container: map.
 * @author    王文佑
 * @date      2017/03/13
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAP_H_
#define _CCNTR_MAP_H_

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @class ccntr_map_node_t
 * @brief Node of map.
 */
typedef struct ccntr_map_node_t
{
    struct ccntr_map_node_t *parent;
    struct ccntr_map_node_t *left;
    struct ccntr_map_node_t *right;

    bool is_red;

    void *key;  ///< The key of the node.

} ccntr_map_node_t;

ccntr_map_node_t* ccntr_map_node_get_next(ccntr_map_node_t *self);
const ccntr_map_node_t* ccntr_map_node_get_next_c(const ccntr_map_node_t *self);
ccntr_map_node_t* ccntr_map_node_get_prev(ccntr_map_node_t *self);
const ccntr_map_node_t* ccntr_map_node_get_prev_c(const ccntr_map_node_t *self);

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
 * @brief Map container.
 */
typedef struct ccntr_map_t
{
    ccntr_map_node_t *root;
    unsigned          count;

    ccntr_map_compare_keys_t compare_keys;

} ccntr_map_t;

void ccntr_map_init(ccntr_map_t *self, ccntr_map_compare_keys_t compare_keys);

unsigned ccntr_map_get_count(const ccntr_map_t *self);

ccntr_map_node_t* ccntr_map_get_first(ccntr_map_t *self);
const ccntr_map_node_t* ccntr_map_get_first_c(const ccntr_map_t *self);
ccntr_map_node_t* ccntr_map_get_last(ccntr_map_t *self);
const ccntr_map_node_t* ccntr_map_get_last_c(const ccntr_map_t *self);

ccntr_map_node_t* ccntr_map_find(ccntr_map_t *self, const void *key);
const ccntr_map_node_t* ccntr_map_find_c(const ccntr_map_t *self, const void *key);

ccntr_map_node_t* ccntr_map_link(ccntr_map_t *self, ccntr_map_node_t *node);
void ccntr_map_unlink(ccntr_map_t *self, ccntr_map_node_t *node);

void ccntr_map_discard_all(ccntr_map_t *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
