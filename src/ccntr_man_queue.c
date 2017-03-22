#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_queue.h"

#warning Temporary hide!
//#ifdef CCNTR_MAN_QUEUE_ENABLED

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
}
//------------------------------------------------------------------------------
unsigned ccntr_man_queue_get_count(const ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Get count of values it contained.
     *
     * @param self Object instance.
     * @return The count of values.
     */
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
}
//------------------------------------------------------------------------------
const void* ccntr_man_queue_get_current_c(const ccntr_man_queue_t *self)
{
    /**
     * @memberof ccntr_man_queue_t
     * @brief Get the current value.
     *
     * @param self Object instance.
     * @return The current value;
     *         or NULL if container is empty.
     */
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
}
//------------------------------------------------------------------------------

#warning Temporary hide!
//#endif  // CCNTR_MAN_QUEUE_ENABLED
