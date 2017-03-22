#include <assert.h>
#include "ccntr_stack.h"

typedef ccntr_stack_node_t node_t;

//------------------------------------------------------------------------------
void ccntr_stack_init(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Constructor.
     *
     * @param self Object instance.
     */
    self->top   = NULL;
    self->count = 0;
}
//------------------------------------------------------------------------------
unsigned ccntr_stack_get_count(const ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    return self->count;
}
//------------------------------------------------------------------------------
node_t* ccntr_stack_get_current(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->top;
}
//------------------------------------------------------------------------------
const node_t* ccntr_stack_get_current_c(const ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Get the current node.
     *
     * @param self Object instance.
     * @return The current node in container;
     *         or NULL if container is empty.
     */
    return self->top;
}
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
void ccntr_stack_discard_all(ccntr_stack_t *self)
{
    /**
     * @memberof ccntr_stack_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    self->top   = NULL;
    self->count = 0;
}
//------------------------------------------------------------------------------
