#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_list.h"

#ifdef CCNTR_MAN_LIST_ENABLED

typedef ccntr_list_node_t node_t;

typedef struct element_t
{
    node_t  node;
    void   *value;
} element_t;

//------------------------------------------------------------------------------
//---- Element -----------------------------------------------------------------
//------------------------------------------------------------------------------
static
element_t* element_create(void *value)
{
    element_t *ele = malloc(sizeof(element_t));
    if( !ele ) abort_message("ERROR: Cannot allocate more memory!\n");

    ele->value = value;

    return ele;
}
//------------------------------------------------------------------------------
static
void element_release(element_t *ele, ccntr_man_list_release_value_t release_value)
{
    release_value(ele->value);
    free(ele);
}
//------------------------------------------------------------------------------
static
void* element_release_but_keep_value(element_t *ele)
{
    void *value = ele;
    free(ele);

    return value;
}
//------------------------------------------------------------------------------
//---- Iterator ----------------------------------------------------------------
//------------------------------------------------------------------------------
void* ccntr_man_list_iter_get_value(ccntr_man_list_iter_t *self)
{
    /**
     * @memberof ccntr_man_list_iter_t
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
const void* ccntr_man_list_citer_get_value(const ccntr_man_list_citer_t *self)
{
    /**
     * @memberof ccntr_man_list_citer_t
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
//---- List --------------------------------------------------------------------
//------------------------------------------------------------------------------
static
void release_value_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_list_init(ccntr_man_list_t               *self,
                         ccntr_man_list_release_value_t  release_value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Constructor.
     *
     * @param self          Object instance.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @remarks Object must be initialised (and once only) before using.
     */
    ccntr_list_init(&self->super);

    self->release_value = release_value ? release_value : release_value_default;
}
//------------------------------------------------------------------------------
void ccntr_man_list_destroy(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @remarks Object must be destructed to finish using,
     *          and must not make any operation to the object after it be destructed.
     */
    ccntr_man_list_clear(self);
}
//------------------------------------------------------------------------------
void ccntr_man_list_insert(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos, void *value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Insert a value to the specific position.
     *
     * @param self  Object instance.
     * @param pos   The position to insert value.
     * @param value The value to be added.
     */
    if( pos->container != self )
        abort_message("ERROR: Operator iterator with different container!\n");

    element_t *ele = element_create(value);

    ccntr_list_link(&self->super, pos->node, &ele->node);
}
//------------------------------------------------------------------------------
void ccntr_man_list_insert_first(ccntr_man_list_t *self, void *value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Insert a value to the first position.
     *
     * @param self  Object instance.
     * @param value The value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_list_link_first(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void ccntr_man_list_insert_last(ccntr_man_list_t *self, void *value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Insert a value to the last position.
     *
     * @param self  Object instance.
     * @param value The value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_list_link_last(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void ccntr_man_list_erase(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Erase value at the specific position.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     */
    if( pos->container != self )
        abort_message("ERROR: Operator iterator with different container!\n");

    node_t *node = pos->node;
    if( !node ) return;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_list_erase_first(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Erase value at the first position.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_list_get_first(&self->super);
    if( !node ) return;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_list_erase_last(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Erase value at the last position.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_list_get_last(&self->super);
    if( !node ) return;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_list_clear(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_list_get_first(&self->super);
    while( node )
    {
        element_t *ele = container_of(node, element_t, node);
        node = node->next;

        element_release(ele, self->release_value);
    }

    ccntr_list_discard_all(&self->super);
}
//------------------------------------------------------------------------------
void* ccntr_man_list_pop(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Pop value from container.
     * @details Similarly to ccntr_man_list_t::ccntr_man_list_erase,
     *          but just remove the value from container,
     *          and will not release the value.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     * @return The value be removed form container.
     */
    if( pos->container != self )
        abort_message("ERROR: Operator iterator with different container!\n");

    node_t *node = pos->node;
    if( !node ) return NULL;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    return element_release_but_keep_value(ele);
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_LIST_ENABLED
