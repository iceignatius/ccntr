#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "container_of.h"
#include "ccntr.h"
#include "test_list.h"

typedef ccntr_map_node_t node_t;

//------------------------------------------------------------------------------
static
node_t* node_create(int key)
{
    node_t *node = malloc(sizeof(node_t));
    node->key = (void*)(intptr_t) key;

    return node;
}
//------------------------------------------------------------------------------
static
void node_release(node_t *node)
{
    free(node);
}
//------------------------------------------------------------------------------
static
int compare_keys(const void *key1, const void *key2)
{
    return (intptr_t) key1 - (intptr_t) key2;
}
//------------------------------------------------------------------------------
static
void map_simple_link_test(void **state)
{
    // Initialise container.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    // Link test.

    assert_int_equal( ccntr_map_get_count(&map), 0 );

    assert_null( ccntr_map_link(&map, node_1) );
    assert_int_equal( ccntr_map_get_count(&map), 1 );

    assert_null( ccntr_map_link(&map, node_3) );
    assert_int_equal( ccntr_map_get_count(&map), 2 );

    assert_null( ccntr_map_link(&map, node_5) );
    assert_int_equal( ccntr_map_get_count(&map), 3 );

    assert_null( ccntr_map_link(&map, node_7) );
    assert_int_equal( ccntr_map_get_count(&map), 4 );

    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Clear.

    node_release(node_1);
    node_release(node_3);
    node_release(node_5);
    node_release(node_7);
    node_release(node_9);
    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void map_iterate_test(void **state)
{
    // Build container base.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    assert_null( ccntr_map_link(&map, node_1) );
    assert_null( ccntr_map_link(&map, node_3) );
    assert_null( ccntr_map_link(&map, node_5) );
    assert_null( ccntr_map_link(&map, node_7) );
    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Iterate test.

    node_t *node = ccntr_map_get_first(&map); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_3 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_7 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_next(node); assert_null( node );

    node = ccntr_map_get_last(&map); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_7 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_3 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_prev(node); assert_null( node );

    // Clear.

    for(ccntr_map_node_t *node = ccntr_map_get_first_postorder(&map); node;)
    {
        ccntr_map_node_t *node_del = node;
        node = ccntr_map_node_get_next_postorder(node);

        node_release(node_del);
    }

    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void map_unlink_test(void **state)
{
    // Build container base.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    assert_null( ccntr_map_link(&map, node_1) );
    assert_null( ccntr_map_link(&map, node_3) );
    assert_null( ccntr_map_link(&map, node_5) );
    assert_null( ccntr_map_link(&map, node_7) );
    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Unlink test.

    ccntr_map_unlink(&map, node_3);
    node_release(node_3);
    assert_int_equal( ccntr_map_get_count(&map), 4 );

    ccntr_map_unlink(&map, node_7);
    node_release(node_7);
    assert_int_equal( ccntr_map_get_count(&map), 3 );

    // Iterate test.

    node_t *node = ccntr_map_get_first(&map); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_next(node); assert_null( node );

    node = ccntr_map_get_last(&map); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_prev(node); assert_null( node );

    // Clear.

    for(ccntr_map_node_t *node = ccntr_map_get_first_postorder(&map); node;)
    {
        ccntr_map_node_t *node_del = node;
        node = ccntr_map_node_get_next_postorder(node);

        node_release(node_del);
    }

    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void map_unlink_by_key_test(void **state)
{
    // Build container base.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    assert_null( ccntr_map_link(&map, node_1) );
    assert_null( ccntr_map_link(&map, node_3) );
    assert_null( ccntr_map_link(&map, node_5) );
    assert_null( ccntr_map_link(&map, node_7) );
    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Unlink test.

    assert_ptr_equal( ccntr_map_unlink_by_key(&map, (void*)(intptr_t) 3), node_3 );
    node_release(node_3);
    assert_int_equal( ccntr_map_get_count(&map), 4 );

    assert_ptr_equal( ccntr_map_unlink_by_key(&map, (void*)(intptr_t) 7), node_7 );
    node_release(node_7);
    assert_int_equal( ccntr_map_get_count(&map), 3 );

    assert_null( ccntr_map_unlink_by_key(&map, (void*)(intptr_t) 250) );
    assert_int_equal( ccntr_map_get_count(&map), 3 );

    // Iterate test.

    node_t *node = ccntr_map_get_first(&map); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_next(node); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_next(node); assert_null( node );

    node = ccntr_map_get_last(&map); assert_ptr_equal( node, node_9 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_5 );
    node = ccntr_map_node_get_prev(node); assert_ptr_equal( node, node_1 );
    node = ccntr_map_node_get_prev(node); assert_null( node );

    // Clear.

    for(ccntr_map_node_t *node = ccntr_map_get_first_postorder(&map); node;)
    {
        ccntr_map_node_t *node_del = node;
        node = ccntr_map_node_get_next_postorder(node);

        node_release(node_del);
    }

    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void map_search_test(void **state)
{
    // Build container base.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    assert_null( ccntr_map_link(&map, node_1) );
    assert_null( ccntr_map_link(&map, node_3) );
    assert_null( ccntr_map_link(&map, node_5) );
    assert_null( ccntr_map_link(&map, node_7) );
    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Search test.

    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  0), NULL );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  1), node_1 );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  2), NULL );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  3), node_3 );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  4), NULL );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  5), node_5 );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  6), NULL );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  7), node_7 );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  8), NULL );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t)  9), node_9 );
    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t) 10), NULL );

    // Clear.

    for(ccntr_map_node_t *node = ccntr_map_get_first_postorder(&map); node;)
    {
        ccntr_map_node_t *node_del = node;
        node = ccntr_map_node_get_next_postorder(node);

        node_release(node_del);
    }

    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void map_duplicated_link_test(void **state)
{
    // Build container base.

    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);

    node_t *node_1 = node_create(1);
    node_t *node_3 = node_create(3);
    node_t *node_5 = node_create(5);
    node_t *node_7 = node_create(7);
    node_t *node_9 = node_create(9);

    assert_null( ccntr_map_link(&map, node_1) );
    assert_null( ccntr_map_link(&map, node_3) );
    assert_null( ccntr_map_link(&map, node_5) );
    assert_null( ccntr_map_link(&map, node_7) );
    assert_null( ccntr_map_link(&map, node_9) );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    // Duplicated link test.

    node_t *node_7_new = node_create(7);
    node_t *node_7_old = ccntr_map_link(&map, node_7_new);
    assert_ptr_equal( node_7_old, node_7 );
    assert_int_equal( ccntr_map_get_count(&map), 5 );

    assert_ptr_equal( ccntr_map_find(&map, (void*)(intptr_t) 7), node_7_new );
    node_release(node_7_old);

    // Clear.

    for(ccntr_map_node_t *node = ccntr_map_get_first_postorder(&map); node;)
    {
        ccntr_map_node_t *node_del = node;
        node = ccntr_map_node_get_next_postorder(node);

        node_release(node_del);
    }

    ccntr_map_discard_all(&map);
}
//------------------------------------------------------------------------------
static
void rbtree_check_link(node_t *node)
{
    if( node && node->left )
    {
        assert_ptr_equal( node->left->parent, node );
        rbtree_check_link(node->left);
    }

    if( node && node->right )
    {
        assert_ptr_equal( node->right->parent, node );
        rbtree_check_link(node->right);
    }
}
//------------------------------------------------------------------------------
static
void rbtree_check_tags_order(node_t *node)
{
    if( node && node->left )
    {
        assert_true( 0 > compare_keys(node->left->key, node->key) );
        rbtree_check_tags_order(node->left);
    }

    if( node && node->right )
    {
        assert_true( 0 < compare_keys(node->right->key, node->key) );
        rbtree_check_tags_order(node->right);
    }
}
//------------------------------------------------------------------------------
static
void rbtree_check_root(node_t *root)
{
    assert_true( !root || !root->is_red );
}
//------------------------------------------------------------------------------
static
void rbtree_check_reds(node_t *node)
{
    if( !node ) return;

    if( node->is_red )
    {
        assert_true( !node->left || !node->left->is_red );
        assert_true( !node->right || !node->right->is_red );
    }

    rbtree_check_reds(node->left);
    rbtree_check_reds(node->right);
}
//------------------------------------------------------------------------------
static
unsigned rbtree_check_and_count_black_level(node_t *node)
{
    if( !node ) return 1;

    size_t count_left  = rbtree_check_and_count_black_level(node->left);
    size_t count_right = rbtree_check_and_count_black_level(node->right);
    assert_int_equal( count_left, count_right );

    return count_left + ( node->is_red ? 0 : 1 );
}
//------------------------------------------------------------------------------
static
void rbtree_condition_check(node_t *root)
{
    rbtree_check_link(root);
    rbtree_check_tags_order(root);
    rbtree_check_root(root);
    rbtree_check_reds(root);
    rbtree_check_and_count_black_level(root);
}
//------------------------------------------------------------------------------
static
unsigned rbtree_count_nodes(node_t *node)
{
    return ( node )?
           ( 1 + rbtree_count_nodes(node->left) + rbtree_count_nodes(node->right) ):
           ( 0 );
}
//------------------------------------------------------------------------------
static
void rbtree_count_check(node_t *root, unsigned count)
{
    assert_int_equal( rbtree_count_nodes(root), count );
}
//------------------------------------------------------------------------------
static
void rbtree_total_condition_check(ccntr_map_t *map)
{
    rbtree_condition_check(map->root);
    rbtree_count_check(map->root, map->count);
}
//------------------------------------------------------------------------------
static
void map_rbtree_condition_test_in_single_case(int tags_add_queue[],
                                              int tags_remove_queue[],
                                              unsigned queue_size)
{
    ccntr_map_t map;
    ccntr_map_init(&map, compare_keys);
    rbtree_total_condition_check(&map);

    for(unsigned i=0; i<queue_size; ++i)
    {
        assert_null( ccntr_map_link(&map, node_create(tags_add_queue[i])) );
        rbtree_total_condition_check(&map);
    }

    for(unsigned i=0; i<queue_size; ++i)
    {
        node_t *node = ccntr_map_find(&map, (void*)(intptr_t) tags_remove_queue[i]);
        assert_non_null( node );

        ccntr_map_unlink(&map, node);
        rbtree_total_condition_check(&map);

        node_release(node);
    }
}
//------------------------------------------------------------------------------
static
void map_rbtree_condition_test(void **state)
{
    {
        int add_queue   [] = {31,32,21,12,25,16,9,13,11,27,26,7,5,6,28,18,17,22,19,29,30,2,23,15,20,3,1,10,14,8,24,4};
        int remove_queue[] = {27,32,15,19,26,1,9,22,3,21,29,28,13,23,7,11,17,31,14,5,12,4,6,20,30,10,16,18,8,25,24,2};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }

    {
        int add_queue   [] = {27,32,25,17,21,7,10,12,22,14,16,3,1,15,29,5,2,6,4,9,24,26,8,30,31,18,11,19,20,28,13,23};
        int remove_queue[] = {31,10,6,19,27,1,5,15,2,28,3,20,30,24,18,16,25,17,26,29,32,7,11,4,23,9,8,14,22,13,21,12};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }

    {
        int add_queue   [] = {2,21,1,28,24,22,13,5,10,30,29,8,4,11,9,19,3,14,6,7,15,25,23,26,17,16,32,18,31,27,20,12};
        int remove_queue[] = {26,17,2,14,28,18,3,12,30,21,13,22,32,4,5,1,24,19,25,16,23,9,10,11,29,15,27,7,20,31,8,6};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }

    {
        int add_queue   [] = {7,17,29,2,22,30,24,19,4,26,31,27,16,32,21,11,3,12,9,10,23,5,20,25,1,28,8,18,14,13,15,6};
        int remove_queue[] = {20,1,29,21,31,32,24,27,22,3,4,9,25,6,13,30,17,5,28,8,15,18,2,11,16,10,12,19,26,23,7,14};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }

    {
        int add_queue   [] = {15,1,10,20,2,21,3,13,9,30,26,17,5,7,29,27,24,4,31,11,8,16,28,19,32,14,12,18,23,22,6,25};
        int remove_queue[] = {26,14,15,12,29,13,1,20,19,22,9,6,24,7,5,2,4,10,16,21,23,28,30,17,27,18,31,8,25,3,32,11};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }

    {
        int add_queue   [] = {15,26,6,23,27,18,32,31,11,3,8,24,17,13,16,25,4,19,28,1,10,9,5,30,14,29,2,22,12,20,7,21};
        int remove_queue[] = {31,13,29,24,22,20,4,25,11,16,5,9,19,8,18,23,7,12,32,26,27,14,6,3,17,30,2,10,15,21,28,1};
        assert_int_equal( sizeof(add_queue), sizeof(remove_queue) );
        int queue_size = sizeof(add_queue)/sizeof(add_queue[0]);

        map_rbtree_condition_test_in_single_case(add_queue, remove_queue, queue_size);
    }
}
//------------------------------------------------------------------------------
int test_map(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(map_simple_link_test),
        cmocka_unit_test(map_iterate_test),
        cmocka_unit_test(map_unlink_test),
        cmocka_unit_test(map_unlink_by_key_test),
        cmocka_unit_test(map_search_test),
        cmocka_unit_test(map_duplicated_link_test),
        cmocka_unit_test(map_rbtree_condition_test),
    };

    return cmocka_run_group_tests_name("map test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
