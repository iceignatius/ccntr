#include <assert.h>
#include "ccntr_queue.h"

typedef ccntr_queue_node_t node_t;

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
    node->next = NULL;

    if( self->last ) self->last->next = node;
    self->last = node;

    if( !self->first ) self->first = node;

    ++ self->count;
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
    node_t *node = self->first;
    if( !node ) return NULL;

    self->first = node->next;
    if( !self->first ) self->last = NULL;

    assert( self->count );
    -- self->count;

    return node;
}
//------------------------------------------------------------------------------
