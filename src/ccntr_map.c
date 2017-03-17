#include <assert.h>
#include <stdint.h>
#include "ccntr_map.h"

typedef ccntr_map_node_t node_t;

//------------------------------------------------------------------------------
//---- Node Initialise ---------------------------------------------------------
//------------------------------------------------------------------------------
static
void node_reset(node_t *node)
{
    node->parent = NULL;
    node->left   = NULL;
    node->right  = NULL;
    node->is_red = true;
}
//------------------------------------------------------------------------------
//---- Node Characteristics ----------------------------------------------------
//------------------------------------------------------------------------------
static
bool node_is_isolated(const node_t *node)
{
    assert( node );
    return !node->parent && !node->left && !node->right;
}
//------------------------------------------------------------------------------
static
bool node_is_black(node_t *node)
{
    return !node || !node->is_red;
}
//------------------------------------------------------------------------------
static
bool node_is_red(node_t *node)
{
    return node && node->is_red;
}
//------------------------------------------------------------------------------
static
bool node_have_child(node_t *node)
{
    return node && ( node->left || node->right );
}
//------------------------------------------------------------------------------
static
bool node_have_full_child(node_t *node)
{
    return node && node->left && node->right;
}
//------------------------------------------------------------------------------
//---- Node Family -------------------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* node_get_leftmost_child(node_t *node)
{
    while( node && node->left )
        node = node->left;

    return node;
}
//------------------------------------------------------------------------------
static
node_t* node_get_rightmost_child(node_t *node)
{
    while( node && node->right )
        node = node->right;

    return node;
}
//------------------------------------------------------------------------------
static
node_t* node_get_leftmost_parent(node_t *node)
{
    if( !node ) return NULL;

    for(node_t *parent = node->parent;
        parent && parent->right == node;)
    {
        node = parent;
        parent = node->parent;
    }

    return node;
}
//------------------------------------------------------------------------------
static
node_t* node_get_rightmost_parent(node_t *node)
{
    if( !node ) return NULL;

    for(node_t *parent = node->parent;
        parent && parent->left == node;)
    {
        node = parent;
        parent = node->parent;
    }

    return node;
}
//------------------------------------------------------------------------------
static
node_t* node_get_grand_parent(node_t *node)
{
    return ( node && node->parent )?( node->parent->parent ):( NULL );
}
//------------------------------------------------------------------------------
static
node_t* node_get_uncle(node_t *node)
{
    if( !node ) return NULL;

    node_t *parent = node->parent;
    if( !parent ) return NULL;

    node_t *grand_parent = parent->parent;
    if( !grand_parent ) return NULL;

    return ( parent == grand_parent->left )?( grand_parent->right ):( grand_parent->left );
}
//------------------------------------------------------------------------------
static
node_t* node_get_brother(node_t *node, node_t *parent)
{
    // The input parent is used for the case if node is NULL.
    if( !parent ) return NULL;
    return ( parent->left == node )?( parent->right ):( parent->left );
}
//------------------------------------------------------------------------------
//---- Node Search -------------------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* tree_find_closest(node_t *root, const void *key, ccntr_map_compare_keys_t compare)
{
    node_t *node = root;
    while( node )
    {
        int comp_res = compare(node->key, key);
        if( comp_res < 0 )
        {
            if( !node->right ) break;
            node = node->right;
        }
        else if( comp_res > 0 )
        {
            if( !node->left ) break;
            node = node->left;
        }
        else
        {
            break;
        }
    }

    return node;
}
//------------------------------------------------------------------------------
static
node_t* tree_find_match(node_t *root, const void *key, ccntr_map_compare_keys_t compare)
{
    node_t *node = root;
    while( node )
    {
        int comp_res = compare(node->key, key);
        if( comp_res < 0 )
            node = node->right;
        else if( comp_res > 0 )
            node = node->left;
        else
            break;
    }

    return node;
}
//------------------------------------------------------------------------------
//---- Node Link ---------------------------------------------------------------
//------------------------------------------------------------------------------
static
void node_link_left(node_t *main, node_t *node)
{
    assert( main );
    assert( !main->left );

    if( node )
    {
        main->left   = node;
        node->parent = main;
    }
}
//------------------------------------------------------------------------------
static
void node_link_right(node_t *main, node_t *node)
{
    assert( main );
    assert( !main->right );

    if( node )
    {
        main->right  = node;
        node->parent = main;
    }
}
//------------------------------------------------------------------------------
static
void node_unlink_left(node_t *node)
{
    assert( node );

    node_t *child = node->left;
    if( child )
    {
        node ->left   = NULL;
        child->parent = NULL;
    }
}
//------------------------------------------------------------------------------
static
void node_unlink_right(node_t *node)
{
    assert( node );

    node_t *child = node->right;
    if( child )
    {
        node ->right  = NULL;
        child->parent = NULL;
    }
}
//------------------------------------------------------------------------------
//---- Node Link - Advanced ----------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* tree_move_node_parent(node_t *root, node_t *from, node_t *to)
{
    assert( from && to );

    node_t *parent = from->parent;
    if( parent )
    {
        if( from == parent->left )
            parent->left = to;
        else
            parent->right = to;

        if( to ) to->parent = parent;
        from->parent = NULL;
    }
    else
    {
        assert( root == from );
        root = to;
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_replace_node(node_t *root, node_t *node_old, node_t *node_new)
{
    assert( root && node_old && node_new );

    node_t *left = node_old->left;
    node_unlink_left(node_old);
    node_link_left(node_new, left);

    node_t *right = node_old->right;
    node_unlink_right(node_old);
    node_link_right(node_new, right);

    return tree_move_node_parent(root, node_old, node_new);
}
//------------------------------------------------------------------------------
//---- Node Visit (In Order) ---------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* tree_get_first_inorder(node_t *root)
{
    return node_get_leftmost_child(root);
}
//------------------------------------------------------------------------------
static
node_t* tree_get_last_inorder(node_t *root)
{
    return node_get_rightmost_child(root);
}
//------------------------------------------------------------------------------
static
node_t* node_get_next_inorder(node_t *node)
{
    if( !node ) return NULL;

    return ( node->right )?
           ( node_get_leftmost_child(node->right) ):
           ( node_get_leftmost_parent(node)->parent );
}
//------------------------------------------------------------------------------
static
node_t* node_get_prev_inorder(node_t *node)
{
    if( !node ) return NULL;

    return ( node->left )?
           ( node_get_rightmost_child(node->left) ):
           ( node_get_rightmost_parent(node)->parent );
}
//------------------------------------------------------------------------------
//---- Node Visit (Post Order) -------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* tree_get_first_postorder(node_t *root)
{
    node_t *node = root;
    while( node_have_child(node) )
        node = node->left ? node->left : node->right;

    return node;
}
//------------------------------------------------------------------------------
static
node_t* node_get_next_postorder(node_t *node)
{
    if( !node || !node->parent ) return NULL;

    node_t *parent = node->parent;
    return ( parent->right && parent->right != node )?
           ( tree_get_first_postorder(parent->right) ):
           ( parent );
}
//------------------------------------------------------------------------------
//---- Container Iterator ------------------------------------------------------
//------------------------------------------------------------------------------
node_t* ccntr_map_node_get_next(node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node (in order).
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the smaller key to the larger.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_node_get_next_c(const node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node (in order).
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the smaller key to the larger.
     */
    return ccntr_map_node_get_next((node_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_node_get_prev(node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the previous node (in order).
     *
     * @param self Object instance.
     * @return The previous node; or NULL if there does not have the previous node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the larger key to the smaller.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_node_get_prev_c(const node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the previous node (in order).
     *
     * @param self Object instance.
     * @return The previous node; or NULL if there does not have the previous node.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and nodes will be visited from the larger key to the smaller.
     */
    return ccntr_map_node_get_prev((node_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_node_get_next_postorder(node_t *self)
{
    /**
     * @memberof ccntr_map_node_t
     * @brief Get the next node (post order).
     *
     * @param self Object instance.
     * @return The next node; or NULL if there does not have the next node.
     *
     * @remarks Visit nodes with post-order rule usually be suit for some usage
     *          like visit and release all nodes.
     */
}
//------------------------------------------------------------------------------
//---- Container ---------------------------------------------------------------
//------------------------------------------------------------------------------
static
int compare_default(const void *key1, const void *key2)
{
    return (intptr_t) key1 - (intptr_t) key2;
}
//------------------------------------------------------------------------------
void ccntr_map_init(ccntr_map_t *self, ccntr_map_compare_keys_t compare)
{
    /**
     * @memberof ccntr_map_t
     * @brief Constructor.
     *
     * @param self    Object instance.
     * @param compare A function to be used to compare keys.
     *                If this parameter is NULL, then
     *                all keys will be treated as integral values.
     */
    self->root    = NULL;
    self->count   = 0;
    self->compare = compare ? compare : compare_default;
}
//------------------------------------------------------------------------------
unsigned ccntr_map_get_count(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get nodes count.
     *
     * @param self Object instance.
     * @return The nodes count.
     */
    return self->count;
}
//------------------------------------------------------------------------------
node_t* ccntr_map_get_first(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node (in order).
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the first node will have the smallest key.
     */
    return tree_get_first_inorder(self->root);
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_get_first_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node (in order).
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the first node will have the smallest key.
     */
    return ccntr_map_get_first((ccntr_map_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_get_last(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the last node will have the largest key.
     */
    return tree_get_last_inorder(self->root);
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_get_last_c(const ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the last node.
     *
     * @param self Object instance.
     * @return The last node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with in-order rule usually be suit for most usage,
     *          and the last node will have the largest key.
     */
    return ccntr_map_get_last((ccntr_map_t*)self);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_get_first_postorder(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Get the first node (post order).
     *
     * @param self Object instance.
     * @return The first node; or NULL if no any nodes contained.
     *
     * @remarks Visit nodes with post-order rule usually be suit for some usage
     *          like visit and release all nodes.
     */
}
//------------------------------------------------------------------------------
node_t* ccntr_map_link(ccntr_map_t *self, node_t *node)
{
    /**
     * @memberof ccntr_map_t
     * @brief Link a node into the container.
     *
     * @param self Object instance.
     * @param node The new node to be linked.
     *             If the container already have a node with the same key,
     *             then the old node will be pop out,
     *             and the new one will be saved.
     * @return A node be pop out which have the same key with the new node;
     *         or NULL if there do not have node with duplicated keys.
     */
    if( !node ) return NULL;

    node_reset(node);

    node_t *duplicated = NULL;
    node_t *closest = tree_find_closest(self->root, node->key, self->compare);
    if( closest )
    {
        int comp_res = self->compare(closest->key, node->key);
        if( comp_res < 0 )
        {
            node_link_right(closest, node);
            ++ self->count;
        }
        else if( comp_res > 0 )
        {
            node_link_left(closest, node);
            ++ self->count;
        }
        else
        {
            self->root = tree_replace_node(self->root, closest, node);
        }
    }
    else
    {
        self->root = node;
        ++ self->count;
    }

    return duplicated;
}
//------------------------------------------------------------------------------
void ccntr_map_unlink(ccntr_map_t *self, node_t *node)
{
    /**
     * @memberof ccntr_map_t
     * @brief Unlink a node from the container.
     *
     * @param self Object instance.
     * @param node The node which is linked in the container.
     */
}
//------------------------------------------------------------------------------
node_t* ccntr_map_find(ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find node by key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
}
//------------------------------------------------------------------------------
const node_t* ccntr_map_find_c(const ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find node by key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    return ccntr_map_find((ccntr_map_t*)self, key);
}
//------------------------------------------------------------------------------
void ccntr_map_discard_all(ccntr_map_t *self)
{
    /**
     * @memberof ccntr_map_t
     * @brief Discard all linkage of nodes in the container.
     *
     * @param self Object instance.
     */
    self->root  = NULL;
    self->count = 0;
}
//------------------------------------------------------------------------------
