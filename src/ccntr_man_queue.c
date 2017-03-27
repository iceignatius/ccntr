#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_queue.h"

#ifdef CCNTR_MAN_QUEUE_ENABLED

typedef ccntr_queue_node_t node_t;

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
void element_release(element_t *ele, ccntr_man_queue_release_value_t release_value)
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
//---- Queue -------------------------------------------------------------------
//------------------------------------------------------------------------------
static
void release_value_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_queue_init(ccntr_man_queue_t *self, ccntr_man_queue_release_value_t release_value)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Constructor.
     *
     * @param self Object instance.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @attention Object must be initialised (and once only) before using.
     */
    ccntr_queue_init(&self->super);

    self->release_value = release_value ? release_value : release_value_default;
}
//------------------------------------------------------------------------------
void ccntr_man_queue_destroy(ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @attention Object must be destructed to finish using,
     *            and must not make any operation to the object after it be destructed.
     */
    ccntr_man_queue_clear(self);
}
//------------------------------------------------------------------------------
void* ccntr_man_queue_get_current(ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Get the current value.
     *
     * @param self Object instance.
     * @return The current value;
     *         or NULL if container is empty.
     */
    node_t *node = ccntr_queue_get_current(&self->super);
    if( !node ) return NULL;

    element_t *ele = container_of(node, element_t, node);
    return ele->value;
}
//------------------------------------------------------------------------------
void ccntr_man_queue_push(ccntr_man_queue_t *self, void *value)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Push a value into the container.
     *
     * @param self  Object instance.
     * @param value The new value to be added.
     */
    element_t *ele = element_create(value);

    ccntr_queue_link(&self->super, &ele->node);
}
//------------------------------------------------------------------------------
void* ccntr_man_queue_pop(ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Get and pop the current value.
     *
     * @param self Object instance.
     * @return The current value;
     *         or NULL if container is empty.
     *
     * @remarks The value returned will not be released by container,
     *          and that means user will be responsible for that.
     */
    node_t *node = ccntr_queue_unlink(&self->super);
    if( !node ) return NULL;

    element_t *ele = container_of(node, element_t, node);
    return element_release_but_keep_value(ele);
}
//------------------------------------------------------------------------------
void ccntr_man_queue_erase_current(ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Erase the current value.
     *
     * @param self Object instance.
     */
    node_t *node = ccntr_queue_unlink(&self->super);
    if( !node ) return;

    element_t *ele = container_of(node, element_t, node);
    element_release(ele, self->release_value);
}
//------------------------------------------------------------------------------
static
void move_contents_to_shadow_object(ccntr_man_queue_t *shadow, ccntr_man_queue_t *src)
{
    ccntr_spinlock_lock(&src->super.lock);

    *shadow = *src;
    ccntr_spinlock_init(&shadow->super.lock);

    src->super.first = NULL;
    src->super.last  = NULL;
    src->super.count = 0;

    ccntr_spinlock_unlock(&src->super.lock);
}
//------------------------------------------------------------------------------
void ccntr_man_queue_clear(ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
    ccntr_man_queue_t shadow;
    move_contents_to_shadow_object(&shadow, self);

    node_t *node = ccntr_queue_get_current(&shadow.super);
    while( node )
    {
        element_t *ele = container_of(node, element_t, node);
        node = node->next;

        element_release(ele, shadow.release_value);
    }
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_QUEUE_ENABLED
