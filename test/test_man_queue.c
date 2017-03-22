#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "ccntr.h"
#include "test_man_queue.h"

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
CCNTR_DECLARE_QUEUE(queue, element_t*, (void(*)(void*))element_release)
//------------------------------------------------------------------------------
static
int man_queue_create(void **state)
{
    queue_t *queue = malloc(sizeof(queue_t));
    if( !queue ) return -1;

    queue_init(queue);

    *state = queue;
    return 0;
}
//------------------------------------------------------------------------------
static
int man_queue_release(void **state)
{
    queue_t *queue = *state;

    queue_destroy(queue);
    free(queue);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void queue_push_test(void **state)
{
    queue_t *queue = *state;

    assert_int_equal( queue_get_count(queue), 0 );

    queue_push(queue, element_create(11));
    assert_int_equal( queue_get_count(queue), 1 );

    queue_push(queue, element_create(33));
    assert_int_equal( queue_get_count(queue), 2 );

    queue_push(queue, element_create(55));
    assert_int_equal( queue_get_count(queue), 3 );
}
//------------------------------------------------------------------------------
static
void queue_pop_test(void **state)
{
    queue_t *queue = *state;

    assert_int_equal( queue_get_count(queue), 3 );

    element_t *ele;

    assert_non_null(( ele = queue_get_current(queue) ));
    assert_ptr_equal( queue_pop(queue), ele );
    assert_int_equal( queue_get_count(queue), 2 );
    assert_int_equal( ele->value, 11 );
    element_release(ele);

    assert_non_null(( ele = queue_get_current(queue) ));
    assert_ptr_equal( queue_pop(queue), ele );
    assert_int_equal( queue_get_count(queue), 1 );
    assert_int_equal( ele->value, 33 );
    element_release(ele);

    assert_non_null(( ele = queue_get_current(queue) ));
    assert_ptr_equal( queue_pop(queue), ele );
    assert_int_equal( queue_get_count(queue), 0 );
    assert_int_equal( ele->value, 55 );
    element_release(ele);

    assert_null( queue_get_current(queue) );
    assert_null( queue_pop(queue) );
    assert_int_equal( queue_get_count(queue), 0 );
}
//------------------------------------------------------------------------------
static
void queue_erase_test(void **state)
{
    queue_t *queue = *state;

    assert_int_equal( queue_get_count(queue), 3 );
    assert_non_null( queue_get_current(queue) );
    assert_int_equal( queue_get_current(queue)->value, 11 );

    queue_erase_current(queue);
    assert_int_equal( queue_get_count(queue), 2 );
    assert_non_null( queue_get_current(queue) );
    assert_int_equal( queue_get_current(queue)->value, 33 );

    queue_erase_current(queue);
    assert_int_equal( queue_get_count(queue), 1 );
    assert_non_null( queue_get_current(queue) );
    assert_int_equal( queue_get_current(queue)->value, 55 );

    queue_erase_current(queue);
    assert_int_equal( queue_get_count(queue), 0 );
    assert_null( queue_get_current(queue) );
}
//------------------------------------------------------------------------------
static
void queue_clear_test(void **state)
{
    queue_t *queue = *state;

    assert_int_equal( queue_get_count(queue), 3 );

    queue_clear(queue);
    assert_null( queue_get_current(queue) );
    assert_null( queue_pop(queue) );
    assert_int_equal( queue_get_count(queue), 0 );
}
//------------------------------------------------------------------------------
int test_man_queue(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(queue_push_test),
        cmocka_unit_test(queue_pop_test),

        cmocka_unit_test(queue_push_test),
        cmocka_unit_test(queue_erase_test),

        cmocka_unit_test(queue_push_test),
        cmocka_unit_test(queue_clear_test),
    };

    return cmocka_run_group_tests_name("managed queue test", tests, man_queue_create, man_queue_release);
}
//------------------------------------------------------------------------------
