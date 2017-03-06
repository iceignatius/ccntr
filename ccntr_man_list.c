#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_list.h"

typedef ccntr_list_node_t node_t;

typedef struct element_t
{
    node_t  node;
    void   *value;
} element_t;

#warning Remember to extract inlin functions!

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
void element_release(element_t *ele, ccntr_man_list_on_value_release_t on_value_release)
{
    on_value_release(ele->value);
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
void ccntr_man_list_iter_init(ccntr_man_list_iter_t *self,
                              ccntr_man_list_t      *container,
                              ccntr_list_node_t     *node)
{
    self->container = container;
    self->node      = node;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//---- Constant Iterator -------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_list_citer_init(ccntr_man_list_citer_t  *self,
                               const ccntr_man_list_t  *container,
                               const ccntr_list_node_t *node)
{
    self->container = container;
    self->node      = node;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
//---- List --------------------------------------------------------------------
//------------------------------------------------------------------------------
static
void on_value_release_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_list_init(ccntr_man_list_t                  *self,
                         ccntr_man_list_on_value_release_t  on_value_release)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Constructor.
     *
     * @param self             Object instance.
     * @param on_value_release Callback to release contained values,
     *                         and can be NULL to do nothing.
     *
     * @remarks Object must be initialised (and once only) before using.
     */
    ccntr_list_init(&self->super);

    self->on_value_release = on_value_release ? on_value_release : on_value_release_default;
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
void ccntr_man_list_push_front(ccntr_man_list_t *self, void *value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Push value to the last position.
     *
     * @param self  Object instance.
     * @param value The value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_list_link_front(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void ccntr_man_list_push_back(ccntr_man_list_t *self, void *value)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Push value to the first position.
     *
     * @param self  Object instance.
     * @param value The value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_list_link_back(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void ccntr_man_list_pop_first(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Pop value at the first position.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_list_get_first(&self->super);
    if( !node ) return;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->on_value_release);
}
//------------------------------------------------------------------------------
void ccntr_man_list_pop_last(ccntr_man_list_t *self)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Pop value at the last position.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_list_get_last(&self->super);
    if( !node ) return;

    ccntr_list_unlink(&self->super, node);

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->on_value_release);
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
    element_release(ele, self->on_value_release);
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

        element_release(ele, self->on_value_release);
    }

    ccntr_list_discard_all(&self->super);
}
//------------------------------------------------------------------------------
void* ccntr_man_list_unlink(ccntr_man_list_t *self, ccntr_man_list_iter_t *pos)
{
    /**
     * @memberof ccntr_man_list_t
     * @brief Remove value from container.
     * @details Similarly to ccntr_man_list_t::ccntr_man_list_erase,
     *          but just remove the value from container,
     *          and will not release the value.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     * @return The value.
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
