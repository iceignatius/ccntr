#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "ccntr.h"
#include "test_man_stack.h"

typedef struct element_t
{
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
CCNTR_DECLARE_STACK(stack, element_t*, (void(*)(void*))element_release)
//------------------------------------------------------------------------------
static
int man_stack_create(void **state)
{
    stack_t *stack = malloc(sizeof(stack_t));
    if( !stack ) return -1;

    stack_init(stack);

    *state = stack;
    return 0;
}
//------------------------------------------------------------------------------
static
int man_stack_release(void **state)
{
    stack_t *stack = *state;

    stack_destroy(stack);
    free(stack);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void stack_push_test(void **state)
{
    stack_t *stack = *state;

    assert_int_equal( stack_get_count(stack), 0 );

    stack_push(stack, element_create(11));
    assert_int_equal( stack_get_count(stack), 1 );

    stack_push(stack, element_create(33));
    assert_int_equal( stack_get_count(stack), 2 );

    stack_push(stack, element_create(55));
    assert_int_equal( stack_get_count(stack), 3 );
}
//------------------------------------------------------------------------------
static
void stack_pop_test(void **state)
{
    stack_t *stack = *state;

    assert_int_equal( stack_get_count(stack), 3 );

    element_t *ele;

    assert_non_null(( ele = stack_get_current(stack) ));
    assert_ptr_equal( stack_pop(stack), ele );
    assert_int_equal( stack_get_count(stack), 2 );
    assert_int_equal( ele->value, 55 );
    element_release(ele);

    assert_non_null(( ele = stack_get_current(stack) ));
    assert_ptr_equal( stack_pop(stack), ele );
    assert_int_equal( stack_get_count(stack), 1 );
    assert_int_equal( ele->value, 33 );
    element_release(ele);

    assert_non_null(( ele = stack_get_current(stack) ));
    assert_ptr_equal( stack_pop(stack), ele );
    assert_int_equal( stack_get_count(stack), 0 );
    assert_int_equal( ele->value, 11 );
    element_release(ele);

    assert_null( stack_get_current(stack) );
    assert_null( stack_pop(stack) );
    assert_int_equal( stack_get_count(stack), 0 );
}
//------------------------------------------------------------------------------
static
void stack_erase_test(void **state)
{
    stack_t *stack = *state;

    assert_int_equal( stack_get_count(stack), 3 );
    assert_non_null( stack_get_current(stack) );
    assert_int_equal( stack_get_current(stack)->value, 55 );

    stack_erase_current(stack);
    assert_int_equal( stack_get_count(stack), 2 );
    assert_non_null( stack_get_current(stack) );
    assert_int_equal( stack_get_current(stack)->value, 33 );

    stack_erase_current(stack);
    assert_int_equal( stack_get_count(stack), 1 );
    assert_non_null( stack_get_current(stack) );
    assert_int_equal( stack_get_current(stack)->value, 11 );

    stack_erase_current(stack);
    assert_int_equal( stack_get_count(stack), 0 );
    assert_null( stack_get_current(stack) );
}
//------------------------------------------------------------------------------
static
void stack_clear_test(void **state)
{
    stack_t *stack = *state;

    assert_int_equal( stack_get_count(stack), 3 );

    stack_clear(stack);
    assert_null( stack_get_current(stack) );
    assert_null( stack_pop(stack) );
    assert_int_equal( stack_get_count(stack), 0 );
}
//------------------------------------------------------------------------------
int test_man_stack(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(stack_push_test),
        cmocka_unit_test(stack_pop_test),

        cmocka_unit_test(stack_push_test),
        cmocka_unit_test(stack_erase_test),

        cmocka_unit_test(stack_push_test),
        cmocka_unit_test(stack_clear_test),
    };

    return cmocka_run_group_tests_name("managed stack test", tests, man_stack_create, man_stack_release);
}
//------------------------------------------------------------------------------
