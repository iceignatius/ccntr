#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_map.h"

#ifdef CCNTR_MAN_MAP_ENABLED

typedef ccntr_map_node_t node_t;

typedef struct element_t
{
    node_t  node;
    void   *value;
} element_t;

//------------------------------------------------------------------------------
//---- Element -----------------------------------------------------------------
//------------------------------------------------------------------------------
static
element_t* element_create(void *key, void *value)
{
    element_t *ele = malloc(sizeof(element_t));
    if( !ele ) abort_message("ERROR: Cannot allocate more memory!\n");

    ele->node.key = key;
    ele->value = value;

    return ele;
}
//------------------------------------------------------------------------------
static
void element_release(element_t                    *ele,
                     ccntr_man_map_release_key_t   release_key,
                     ccntr_man_map_release_value_t release_value)
{
    release_key(ele->node.key);
    release_value(ele->value);
    free(ele);
}
//------------------------------------------------------------------------------
static
void* element_release_but_keep_key_and_value(element_t *ele)
{
    void *value = ele->value;
    free(ele);

    return value;
}
//------------------------------------------------------------------------------
//---- Iterator ----------------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_map_iter_init(ccntr_man_map_iter_t   *self,
                             struct ccntr_man_map_t *container,
                             ccntr_map_node_t       *node)
{
    self->container = container;
    self->node      = node;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
    if( !self->node ) return NULL;

    element_t *ele = container_of(self->node, element_t, node);
    return ele->node.key;
}
//------------------------------------------------------------------------------
void* ccntr_man_map_iter_get_value(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Get value.
     *
     * @param self Object instance.
     * @return The value be pointed by the iterator.
     */
    if( !self->node ) return NULL;

    element_t *ele = container_of(self->node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
//---- Constant Iterator -------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_map_citer_init(ccntr_man_map_citer_t        *self,
                              const struct ccntr_man_map_t *container,
                              const ccntr_map_node_t       *node)
{
    self->container = container;
    self->node      = node;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
const void* ccntr_man_map_citer_get_key(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Get key.
     *
     * @param self Object instance.
     * @return The key be pointed by the iterator.
     */
    if( !self->node ) return NULL;

    const element_t *ele = container_of(self->node, element_t, node);
    return ele->node.key;
}
//------------------------------------------------------------------------------
const void* ccntr_man_map_citer_get_value(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Get value.
     *
     * @param self Object instance.
     * @return The value be pointed by the iterator.
     */
    if( !self->node ) return NULL;

    const element_t *ele = container_of(self->node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
//---- Key Map Container -------------------------------------------------------
//------------------------------------------------------------------------------
static
void release_key_default(void *key)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
static
void release_value_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_map_init(ccntr_man_map_t              *self,
                        ccntr_map_compare_keys_t      compare,
                        ccntr_man_map_release_key_t   release_key,
                        ccntr_man_map_release_value_t release_value)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Constructor.
     *
     * @param self          Object instance.
     * @param compare       A function to be used to compare keys.
     *                      If this parameter is NULL, then
     *                      all keys will be treated as integral values.
     * @param release_key   Callback to release contained keys,
     *                      and can be NULL to do nothing.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @attention Object must be initialised (and once only) before using.
     */
    ccntr_map_init(&self->super, compare);

    self->release_key = release_key ? release_key : release_key_default;
    self->release_value = release_value ? release_value : release_value_default;
}
//------------------------------------------------------------------------------
void ccntr_man_map_destroy(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @attention Object must be destructed to finish using,
     *            and must not make any operation to the object after it be destructed.
     */
    ccntr_man_map_clear(self);
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
void* ccntr_man_map_find_value(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return The value if found; or NULL if not found.
     */
    node_t *node = ccntr_map_find(&self->super, key);
    if( !node ) return NULL;

    element_t *ele = container_of(node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
const void* ccntr_man_map_find_value_c(const ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return The value if found; or NULL if not found.
     */
    const node_t *node = ccntr_map_find_c(&self->super, key);
    if( !node ) return NULL;

    const element_t *ele = container_of(node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
void ccntr_man_map_insert(ccntr_man_map_t *self, void *key, void *value)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Insert a value.
     *
     * @param self  Object instance.
     * @param key   Key of the value to be inserted.
     * @param value The value to be inserted.
     *
     * @remarks If the container already have a value with the same key, then
     *          the old value (and key) will be replaced by the new one.
     */
    element_t *ele = element_create(key, value);

    node_t *node = ccntr_map_link(&self->super, &ele->node);
    if( node )
    {
        element_t *duplicated = container_of(node, element_t, node);
        element_release(duplicated, self->release_key, self->release_value);
    }
}
//------------------------------------------------------------------------------
void ccntr_man_map_erase(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase value.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     */
    if( pos->container != self )
        abort_message("ERROR: Operator iterator with different container!\n");

    node_t *node = pos->node;
    if( !node ) return;

    ccntr_map_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_key, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_map_erase_by_key(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase value.
     *
     * @param self Object instance.
     * @param key  Key of the value.
     */
    node_t *node = ccntr_map_find(&self->super, key);
    if( !node ) return;

    ccntr_map_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_key, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_map_clear(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_map_get_first_postorder(&self->super);
    while( node )
    {
        element_t *ele = container_of(node, element_t, node);
        node = ccntr_map_node_get_next_postorder(node);

        element_release(ele, self->release_key, self->release_value);
    }

    ccntr_map_discard_all(&self->super);
}
//------------------------------------------------------------------------------
void* ccntr_man_map_pop(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Pop value from container.
     * @details Similarly to ccntr_man_map_t::ccntr_man_map_erase,
     *          but just remove the value and key from the container,
     *          and will not release them.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     * @return The value be removed from container;
     *         or NULL if no value available.
     */
    if( pos->container != self )
        abort_message("ERROR: Operator iterator with different container!\n");

    node_t *node = pos->node;
    if( !node ) return NULL;

    ccntr_map_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    return element_release_but_keep_key_and_value(ele);
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_MAP_ENABLED
