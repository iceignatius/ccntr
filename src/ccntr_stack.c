#include <assert.h>
#include "ccntr_stack.h"

typedef ccntr_stack_node_t node_t;

//------------------------------------------------------------------------------
void ccntr_stack_link(ccntr_stack_t *self, node_t *node)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Link a node into container.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     */
    node->prev = self->top;
    self->top = node;

    ++ self->count;
}
//------------------------------------------------------------------------------
node_t* ccntr_stack_unlink(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Unlink the current node from container.
     *
     * @param self Object instance.
     * @return The node which just be unlinked;
     *         or NULL if container is empty.
     */
    node_t *node = self->top;
    if( !node ) return NULL;

    self->top = node->prev;

    assert( self->count );
    -- self->count;

    return node;
}
//------------------------------------------------------------------------------
