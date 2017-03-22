#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "container_of.h"
#include "ccntr.h"
#include "test_queue.h"

typedef ccntr_queue_node_t node_t;

typedef struct element_t
{
    node_t node;
    int value;
} element_t;

//------------------------------------------------------------------------------
static
element_t* element_create(int value)
{
    element_t *ele = malloc(sizeof(element_t));
    ele->value = value;

    return ele;
}
//------------------------------------------------------------------------------
static
void element_release(element_t *ele)
{
    free(ele);
}
//------------------------------------------------------------------------------
static
int queue_create(void **state)
{
    ccntr_queue_t *queue = malloc(sizeof(ccntr_queue_t));
    if( !queue ) return -1;

    ccntr_queue_init(queue);

    *state = queue;
    return 0;
}
//------------------------------------------------------------------------------
static
int queue_release(void **state)
{
    ccntr_queue_t *queue = *state;

    free(queue);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void queue_push_test(void **state)
{
    ccntr_queue_t *queue = *state;

    assert_int_equal( ccntr_queue_get_count(queue), 0 );

    element_t *ele_11 = element_create(11);
    element_t *ele_33 = element_create(33);
    element_t *ele_55 = element_create(55);

    ccntr_queue_link(queue, &ele_11->node);
    assert_int_equal( ccntr_queue_get_count(queue), 1 );

    ccntr_queue_link(queue, &ele_33->node);
    assert_int_equal( ccntr_queue_get_count(queue), 2 );

    ccntr_queue_link(queue, &ele_55->node);
    assert_int_equal( ccntr_queue_get_count(queue), 3 );
}
//------------------------------------------------------------------------------
static
void queue_pop_test(void **state)
{
    ccntr_queue_t *queue = *state;

    assert_int_equal( ccntr_queue_get_count(queue), 3 );

    node_t *node;
    element_t *ele;

    // Pop element 1.

    node = ccntr_queue_get_current(queue);
    assert_non_null( node );
    assert_ptr_equal( ccntr_queue_unlink(queue), node );
    assert_int_equal( ccntr_queue_get_count(queue), 2 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 11 );
    element_release(ele);

    // Pop element 2.

    node = ccntr_queue_get_current(queue);
    assert_non_null( node );
    assert_ptr_equal( ccntr_queue_unlink(queue), node );
    assert_int_equal( ccntr_queue_get_count(queue), 1 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 33 );
    element_release(ele);

    // Pop element 3.

    node = ccntr_queue_get_current(queue);
    assert_non_null( node );
    assert_ptr_equal( ccntr_queue_unlink(queue), node );
    assert_int_equal( ccntr_queue_get_count(queue), 0 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 55 );
    element_release(ele);

    // Pop empty.

    assert_null( ccntr_queue_get_current(queue) );
    assert_null( ccntr_queue_unlink(queue) );
    assert_int_equal( ccntr_queue_get_count(queue), 0 );
}
//------------------------------------------------------------------------------
int test_queue(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(queue_push_test),
        cmocka_unit_test(queue_pop_test),
    };

    return cmocka_run_group_tests_name("queue test", tests, queue_create, queue_release);
}
//------------------------------------------------------------------------------
