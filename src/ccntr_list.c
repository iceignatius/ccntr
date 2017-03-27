#include <assert.h>
#include "ccntr_list.h"

typedef ccntr_list_node_t node_t;

//------------------------------------------------------------------------------
void ccntr_list_link(ccntr_list_t *self, node_t *pos, node_t *node)
{
    /**
     * @memberof ccntr_list_t
     * @brief Link a node to the specific position.
     *
     * @param self Object instance.
     * @param pos  The specific node which already linked to the container.
     *             And this parameter can be NULL to link new node to
     *             the last position for default.
     * @param node The new node to be linked.
     */
    ccntr_spinlock_lock(&self->lock);

    node_t *node_prev = pos ? pos->prev : self->last;
    node_t *node_next = pos;
    node_t *node_new  = node;

    node_new->prev = node_prev;
    node_new->next = node_next;

    if( node_prev )
        node_prev->next = node_new;
    else
        self->first = node_new;

    if( node_next )
        node_next->prev = node_new;
    else
        self->last = node_new;

    ++ self->count;

    ccntr_spinlock_unlock(&self->lock);
}
//------------------------------------------------------------------------------
void ccntr_list_unlink(ccntr_list_t *self, node_t *node)
{
    /**
     * @memberof ccntr_list_t
     * @brief Unlink a node from the container.
     *
     * @param self Object instance.
     * @param node The node which is linked in the container.
     */
    ccntr_spinlock_lock(&self->lock);

    node_t *node_prev = node->prev;
    node_t *node_next = node->next;

    if( node_prev )
        node_prev->next = node_next;
    else
        self->first = node_next;

    if( node_next )
        node_next->prev = node_prev;
    else
        self->last = node_prev;

    assert( self->count );
    -- self->count;

    ccntr_spinlock_unlock(&self->lock);

    node->prev = NULL;
    node->next = NULL;
}
//------------------------------------------------------------------------------
