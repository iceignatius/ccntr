#include <assert.h>
#include "ccntr_queue.h"

typedef ccntr_queue_node_t node_t;

//------------------------------------------------------------------------------
void ccntr_queue_init(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Constructor.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
unsigned ccntr_queue_get_count(const ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
}
//------------------------------------------------------------------------------
node_t* ccntr_queue_get_current(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_queue_get_current_c(const ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
void ccntr_queue_link(ccntr_queue_t *self, node_t *node)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Link a node into container.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     */
}
//------------------------------------------------------------------------------
node_t* ccntr_queue_unlink(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Unlink the current node from container.
     *
     * @param self Object instance.
     * @return The node which just be unlinked;
     *         or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
void ccntr_queue_discard_all(ccntr_queue_t *self)
{
    /**
     * @memberof ccntr_queue_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
