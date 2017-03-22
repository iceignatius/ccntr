#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_stack.h"

#ifdef CCNTR_MAN_STACK_ENABLED

typedef ccntr_stack_node_t node_t;

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
void element_release(element_t *ele, ccntr_man_stack_release_value_t release_value)
{
    release_value(ele->value);
    free(ele);
}
//------------------------------------------------------------------------------
static
void* element_release_but_keep_value(element_t *ele)
{
    void *value = ele->value;
    free(ele);

    return value;
}
//------------------------------------------------------------------------------
//---- Stack -------------------------------------------------------------------
//------------------------------------------------------------------------------
static
void release_value_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_stack_init(ccntr_man_stack_t *self, ccntr_man_stack_release_value_t release_value)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Constructor.
     *
     * @param self Object instance.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @attention Object must be initialised (and once only) before using.
     */
    ccntr_stack_init(&self->super);

    self->release_value = release_value ? release_value : release_value_default;
}
//------------------------------------------------------------------------------
void ccntr_man_stack_destroy(ccntr_man_stack_t *self)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @attention Object must be destructed to finish using,
     *            and must not make any operation to the object after it be destructed.
     */
    ccntr_man_stack_clear(self);
}
//------------------------------------------------------------------------------
void* ccntr_man_stack_get_current(ccntr_man_stack_t *self)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Get the current value.
     *
     * @param self Object instance.
     * @return The current value;
     *         or NULL if container is empty.
     */
    node_t *node = ccntr_stack_get_current(&self->super);
    if( !node ) return NULL;

    element_t *ele = container_of(node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
void ccntr_man_stack_push(ccntr_man_stack_t *self, void *value)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Push a value into the container.
     *
     * @param self  Object instance.
     * @param value The new value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_stack_link(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void* ccntr_man_stack_pop(ccntr_man_stack_t *self)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Get and pop the current value.
     *
     * @param self Object instance.
     * @return The current value;
     *         or NULL if container is empty.
     *
     * @remarks The value returned will not be released by container,
     *          and that means user will be responsible for that.
     */
    node_t *node = ccntr_stack_unlink(&self->super);
    if( !node ) return NULL;

    element_t *ele = container_of(node, element_t, node);
    return element_release_but_keep_value(ele);
}
//------------------------------------------------------------------------------
void ccntr_man_stack_erase_current(ccntr_man_stack_t *self)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Erase the current value.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_stack_unlink(&self->super);
    if( !node ) return;

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_value);
}
//------------------------------------------------------------------------------
void ccntr_man_stack_clear(ccntr_man_stack_t *self)
{
    /**
     * @memberof ccntr_man_stack_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_stack_get_current(&self->super);
    while( node )
    {
        element_t *ele = container_of(node, element_t, node);
        node = node->prev;

        element_release(ele, self->release_value);
    }

    ccntr_stack_discard_all(&self->super);
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_STACK_ENABLED
