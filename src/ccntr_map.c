#include "ccntr_map.h"

typedef ccntr_map_node_t node_t;

//------------------------------------------------------------------------------
node_t* ccntr_map_node_get_next(node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node.
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_node_get_next_c(const node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node.
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     */
    return ccntr_map_node_get_next((node_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_node_get_prev(node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the previous node.
     *
     * @param self Object instance.
     * @return The previous node; or NULL if there does not have the previous node.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_node_get_prev_c(const node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the previous node.
     *
     * @param self Object instance.
     * @return The previous node; or NULL if there does not have the previous node.
     */
    return ccntr_map_node_get_prev((node_t*)self);
}
//------------------------------------------------------------------------------
//---- Container ---------------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_map_init(ccntr_map_t *self, ccntr_map_compare_keys_t compare_keys)
{
    /**
     * @memberof ccntr_map_t
     * @brief Constructor.
     *
     * @param self         Object instance.
     * @param compare_keys A function to be used to compare keys.
     */
    self->root  = NULL;
    self->count = 0;

    self->compare_keys = compare_keys;
}
//------------------------------------------------------------------------------
unsigned ccntr_map_get_count(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    return self->count;
}
//------------------------------------------------------------------------------
node_t* ccntr_map_get_first(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node.
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_get_first_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node.
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     */
    return ccntr_map_get_first((ccntr_map_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_get_last(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_get_last_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     */
    return ccntr_map_get_last((ccntr_map_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_link(ccntr_map_t *self, node_t *node)
{
    /**
     * @memberof ccntr_map_t
     * @brief Link a node into the container.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     *             If the container already have a node with the same key,
     *             then the old node will be pop out,
     *             and the new one will be saved.
     * @return A node be pop out which have the same key with the new node;
     *         or NULL if there do not have node with duplicated keys.
     */
}
//------------------------------------------------------------------------------
void ccntr_map_unlink(ccntr_map_t *self, node_t *node)
{
    /**
     * @memberof ccntr_map_t
     * @brief Unlink a node from the container.
     *
     * @param self Object instance.
     * @param node The node which is linked in the container.
     */
}
//------------------------------------------------------------------------------
node_t* ccntr_map_find(ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find node by key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_find_c(const ccntr_map_t *self, const void *key)
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
//------------------------------------------------------------------------------
void ccntr_map_discard_all(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    self->root  = NULL;
    self->count = 0;
}
//------------------------------------------------------------------------------
