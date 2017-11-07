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
    assert( from );

    node_t *parent = from->parent;
    if( parent )
    {
        if( from == parent->left )
            parent->left = to;
        else
            parent->right = to;

        if( to ) to->parent = parent;
    }
    else
    {
        assert( root == from );
        root = to;
        if( to ) to->parent = NULL;
    }

    from->parent = NULL;

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_replace_node(node_t *root, node_t *node_old, node_t *node_new)
{
    assert( root && node_old && node_new );

    node_new->is_red = node_old->is_red;

    node_t *left = node_old->left;
    node_unlink_left(node_old);
    node_link_left(node_new, left);

    node_t *right = node_old->right;
    node_unlink_right(node_old);
    node_link_right(node_new, right);

    return tree_move_node_parent(root, node_old, node_new);
}
//------------------------------------------------------------------------------
static
node_t* tree_swap_node(node_t *root, node_t *node1, node_t *node2)
{
    assert( root && node1 && node2 );

    node_t temp = {0};
    root = tree_replace_node(root, node1, &temp);
    root = tree_replace_node(root, node2, node1);
    root = tree_replace_node(root, &temp, node2);

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_rotate_node_left(node_t *root, node_t *node)
{
    assert( node && node->right );

    node_t *left   = node;
    node_t *right  = node->right;
    node_t *middle = right->left;

    node_unlink_left(right);
    node_unlink_right(left);
    root = tree_move_node_parent(root, left, right);
    node_link_left(right, left);
    node_link_right(left, middle);

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_rotate_node_right(node_t *root, node_t *node)
{
    assert( node && node->left );

    node_t *left   = node->left;
    node_t *right  = node;
    node_t *middle = left->right;

    node_unlink_right(left);
    node_unlink_left(right);
    root = tree_move_node_parent(root, right, left);
    node_link_right(left, right);
    node_link_left(right, middle);

    return root;
}
//------------------------------------------------------------------------------
//---- Tree Adjust -------------------------------------------------------------
//------------------------------------------------------------------------------
static
node_t* tree_insert_adjust_case_parent_red_uncle_black_and_node_inner(node_t *root, node_t **node)
{
    node_t *parent = (*node)->parent;
    node_t *grand  = node_get_grand_parent(*node);
    assert( parent && grand );

    if( (*node) == parent->right && parent == grand->left )
    {
        root = tree_rotate_node_left(root, parent);
        (*node) = (*node)->left;
    }
    else if( (*node) == parent->left && parent == grand->right )
    {
        root = tree_rotate_node_right(root, parent);
        (*node) = (*node)->right;
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_insert_adjust_case_parent_red_uncle_black_and_node_outer(node_t *root, node_t *node)
{
    node_t *parent = node->parent;
    node_t *grand  = node_get_grand_parent(node);

    parent->is_red = false;
    grand ->is_red = true;

    if( node == parent->left && parent == grand->left )
    {
        root = tree_rotate_node_right(root, grand);
    }
    else if( node == parent->right && parent == grand->right )
    {
        root = tree_rotate_node_left(root, grand);
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_insert_adjust(node_t *root, node_t *node)
{
    assert( root && node );

    node_t *parent = node->parent;
    node_t *grand  = node_get_grand_parent(node);
    node_t *uncle  = node_get_uncle(node);

    if( !parent )
    {
        node->is_red = false;
    }
    else if( !parent->is_red )
    {
        // Nothing to do.
    }
    else if( uncle && uncle->is_red )
    {
        parent->is_red = false;
        uncle ->is_red = false;
        grand ->is_red = true;
        root = tree_insert_adjust(root, grand);
    }
    else
    {
        root = tree_insert_adjust_case_parent_red_uncle_black_and_node_inner(root, &node);
        root = tree_insert_adjust_case_parent_red_uncle_black_and_node_outer(root, node);
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_erase_adjust_case_brother_child_red_inner(node_t *root, node_t *parent, node_t *node)
{
    node_t *brother = node_get_brother(node, parent);
    assert( brother );

    node_t *broleft  = brother->left;
    node_t *broright = brother->right;

    if( node == parent->left &&
        node_is_black(brother) &&
        node_is_red(broleft) &&
        node_is_black(broright) )
    {
        brother->is_red = true;
        broleft->is_red = false;
        root = tree_rotate_node_right(root, brother);
    }
    else if( node == parent->right &&
             node_is_black(brother) &&
             node_is_black(broleft) &&
             node_is_red(broright) )
    {
        brother ->is_red = true;
        broright->is_red = false;
        root = tree_rotate_node_left(root, brother);
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_erase_adjust_case_brother_child_red_outer(node_t *root, node_t *parent, node_t *node)
{
    node_t *brother = node_get_brother(node, parent);
    assert( brother );

    node_t *broleft  = brother->left;
    node_t *broright = brother->right;

    brother->is_red = parent->is_red;
    parent ->is_red = false;

    if( node == parent->left )
    {
        assert( broright );
        broright->is_red = false;
        root = tree_rotate_node_left(root, parent);
    }
    else
    {
        assert( broleft );
        broleft->is_red = false;
        root = tree_rotate_node_right(root, parent);
    }

    return root;
}
//------------------------------------------------------------------------------
static
node_t* tree_erase_adjust(node_t *root, node_t *parent, node_t *node)
{
    if( !parent ) return root;

    node_t *brother = node_get_brother(node, parent);

    if( node_is_red(brother) )
    {
        parent ->is_red = true;
        brother->is_red = false;
        if( node == parent->left )
            root = tree_rotate_node_left(root, parent);
        else
            root = tree_rotate_node_right(root, parent);
    }

    brother = node_get_brother(node, parent);
    node_t *broleft  = brother ? brother->left  : NULL;
    node_t *broright = brother ? brother->right : NULL;
    assert( brother );

    if( node_is_black(parent) &&
        node_is_black(brother) &&
        node_is_black(broleft) &&
        node_is_black(broright) )
    {
        brother->is_red = true;
        root = tree_erase_adjust(root, parent->parent, parent);
    }
    else if( node_is_red(parent) &&
             node_is_black(brother) &&
             node_is_black(broleft) &&
             node_is_black(broright) )
    {
        brother->is_red = true;
        parent ->is_red = false;;
    }
    else
    {
        root = tree_erase_adjust_case_brother_child_red_inner(root, parent, node);
        root = tree_erase_adjust_case_brother_child_red_outer(root, parent, node);
    }

    return root;
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
static
node_t* tree_find_nearest_less(node_t *root, const void *key, ccntr_map_compare_keys_t compare)
{
    node_t *node = tree_find_closest(root, key, compare);
    if( !node ) return NULL;

    int comp_res = compare(node->key, key);
    if( comp_res > 0 ) node = node_get_prev_inorder(node);

    return node;
}
//------------------------------------------------------------------------------
static
node_t* tree_find_nearest_great(node_t *root, const void *key, ccntr_map_compare_keys_t compare)
{
    node_t *node = tree_find_closest(root, key, compare);
    if( !node ) return NULL;

    int comp_res = compare(node->key, key);
    if( comp_res < 0 ) node = node_get_next_inorder(node);

    return node;
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
    return node_get_next_inorder(self);
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
    return node_get_prev_inorder(self);
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
    return node_get_next_postorder(self);
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

    ccntr_spinlock_init(&self->lock);
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
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_get_first_inorder(self->root);
    ccntr_spinlock_unlock(&self->lock);

    return node;
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
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_get_last_inorder(self->root);
    ccntr_spinlock_unlock(&self->lock);

    return node;
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
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_get_first_postorder(self->root);
    ccntr_spinlock_unlock(&self->lock);

    return node;
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
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_find_match(self->root, key, self->compare);
    ccntr_spinlock_unlock(&self->lock);

    return node;
}
//------------------------------------------------------------------------------
ccntr_map_node_t* ccntr_map_find_nearest_less(ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find nearest node which is less or equal than the specified key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_find_nearest_less(self->root, key, self->compare);
    ccntr_spinlock_unlock(&self->lock);

    return node;
}
//------------------------------------------------------------------------------
ccntr_map_node_t* ccntr_map_find_nearest_great(ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Find nearest node which is greater or equal than the specified key.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node if found; and NULL if not found.
     */
    ccntr_spinlock_lock(&self->lock);
    ccntr_map_node_t *node = tree_find_nearest_great(self->root, key, self->compare);
    ccntr_spinlock_unlock(&self->lock);

    return node;
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
     *
     * @attention The new node to be linked must be isolated (not linked in any container),
     *            or the bahaviour is undefuned!
     */
    if( !node ) return NULL;

    node_reset(node);

    ccntr_spinlock_lock(&self->lock);

    node_t *duplicated = NULL;
    node_t *closest = tree_find_closest(self->root, node->key, self->compare);
    if( closest )
    {
        int comp_res = self->compare(closest->key, node->key);
        if( comp_res < 0 )
        {
            node_link_right(closest, node);
            self->root = tree_insert_adjust(self->root, node);
            ++ self->count;
        }
        else if( comp_res > 0 )
        {
            node_link_left(closest, node);
            self->root = tree_insert_adjust(self->root, node);
            ++ self->count;
        }
        else
        {
            self->root = tree_replace_node(self->root, closest, node);
            duplicated = closest;
        }
    }
    else
    {
        self->root = node;
        self->root = tree_insert_adjust(self->root, node);
        ++ self->count;
    }

    ccntr_spinlock_unlock(&self->lock);

    return duplicated;
}
//------------------------------------------------------------------------------
static
void ccntr_map_unlink_without_lock(ccntr_map_t *self, node_t *node)
{
    assert( node );

    // Exchange node position with the nearest single/no child node.
    if( node_have_full_child(node) )
    {
        node_t *nearest = node_get_rightmost_child(node->left);
        self->root = tree_swap_node(self->root, node, nearest);
    }

    // Unlink the node, and use its child to replace the position.
    node_t *parent = node->parent;
    node_t *child  = node->left ? node->left : node->right;
    self->root = tree_move_node_parent(self->root, node, child);

    // Nodes adjust.
    if( node_is_red(node) )
    {
        // Nothing to do.
    }
    else if( node_is_red(child) )
    {
        child->is_red = false;
    }
    else
    {
        self->root = tree_erase_adjust(self->root, parent, child);
    }

    assert( self->count );
    -- self->count;
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
     *
     * @attention The node to be unlinkd must be a member of this container,
     *            or the behaviour is undefuned!
     */
    if( !node ) return;

    ccntr_spinlock_lock(&self->lock);
    ccntr_map_unlink_without_lock(self, node);
    ccntr_spinlock_unlock(&self->lock);

    node_reset(node);
}
//------------------------------------------------------------------------------
node_t* ccntr_map_unlink_by_key(ccntr_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_map_t
     * @brief Search and unlink a node from the container.
     *
     * @param self Object instance.
     * @param key  Key of the node.
     * @return The node which just be found and unlinked;
     *         or NULL if there does not have a node with the key.
     */
    ccntr_spinlock_lock(&self->lock);

    node_t *node = tree_find_match(self->root, key, self->compare);
    if( node ) ccntr_map_unlink_without_lock(self, node);

    ccntr_spinlock_unlock(&self->lock);

    return node;
}
//------------------------------------------------------------------------------
