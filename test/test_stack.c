#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "container_of.h"
#include "ccntr.h"
#include "test_stack.h"

typedef ccntr_stack_node_t node_t;

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
int stack_create(void **state)
{
    ccntr_stack_t *stack = malloc(sizeof(ccntr_stack_t));
    if( !stack ) return -1;

    ccntr_stack_init(stack);

    *state = stack;
    return 0;
}
//------------------------------------------------------------------------------
static
int stack_release(void **state)
{
    ccntr_stack_t *stack = *state;

    free(stack);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void stack_push_test(void **state)
{
    ccntr_stack_t *stack = *state;

    assert_int_equal( ccntr_stack_get_count(stack), 0 );

    element_t *ele_11 = element_create(11);
    element_t *ele_33 = element_create(33);
    element_t *ele_55 = element_create(55);

    ccntr_stack_link(stack, &ele_11->node);
    assert_int_equal( ccntr_stack_get_count(stack), 1 );

    ccntr_stack_link(stack, &ele_33->node);
    assert_int_equal( ccntr_stack_get_count(stack), 2 );

    ccntr_stack_link(stack, &ele_55->node);
    assert_int_equal( ccntr_stack_get_count(stack), 3 );
}
//------------------------------------------------------------------------------
static
void stack_pop_test(void **state)
{
    ccntr_stack_t *stack = *state;

    assert_int_equal( ccntr_stack_get_count(stack), 3 );

    node_t *node;
    element_t *ele;

    // Pop element 3.

    node = ccntr_stack_get_node(stack);
    assert_non_null( node );
    assert_ptr_equal( ccntr_stack_unlink(stack), node );
    assert_int_equal( ccntr_stack_get_count(stack), 2 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 55 );
    element_release(ele);

    // Pop element 2.

    node = ccntr_stack_get_node(stack);
    assert_non_null( node );
    assert_ptr_equal( ccntr_stack_unlink(stack), node );
    assert_int_equal( ccntr_stack_get_count(stack), 1 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 33 );
    element_release(ele);

    // Pop element 1.

    node = ccntr_stack_get_node(stack);
    assert_non_null( node );
    assert_ptr_equal( ccntr_stack_unlink(stack), node );
    assert_int_equal( ccntr_stack_get_count(stack), 0 );

    ele = container_of(node, element_t, node);
    assert_int_equal( ele->value, 11 );
    element_release(ele);

    // Pop empty.

    assert_null( ccntr_stack_get_node(stack) );
    assert_null( ccntr_stack_unlink(stack) );
    assert_int_equal( ccntr_stack_get_count(stack), 0 );
}
//------------------------------------------------------------------------------
int test_stack(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(stack_push_test),
        cmocka_unit_test(stack_pop_test),
    };

    return cmocka_run_group_tests_name("stack test", tests, stack_create, stack_release);
}
//------------------------------------------------------------------------------
