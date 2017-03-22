#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "ccntr.h"
#include "test_man_list.h"

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
CCNTR_DECLARE_LIST(list, element_t*, (void(*)(void*))element_release)
//------------------------------------------------------------------------------
#define compare_list(list, target) compare_list_and_array(list, target, sizeof(target)/sizeof(target[0]))
static
bool compare_list_and_array(const list_t *list, int array[], unsigned count)
{
    // Compare elements count.
    if( list_get_count(list) != count ) return false;

    // Access forward and compare elements.
    list_citer_t iter = list_get_first_c(list);
    for(int i = 0; i < (int)count; ++i, list_citer_move_next(&iter))
    {
        if( !list_citer_have_value(&iter) ) return false;

        const element_t *ele = list_citer_get_value(&iter);
        if( ele->value != array[i] ) return false;
    }
    if( list_citer_have_value(&iter) ) return false;

    // Access backward and compare elements.
    iter = list_get_last_c(list);
    for(int i = (int)count - 1; i >= 0; --i, list_citer_move_prev(&iter))
    {
        if( !list_citer_have_value(&iter) ) return false;

        const element_t *ele = list_citer_get_value(&iter);
        if( ele->value != array[i] ) return false;
    }
    if( list_citer_have_value(&iter) ) return false;

    return true;
}
//------------------------------------------------------------------------------
static
bool is_list_empty(const list_t *list)
{
    return !list_get_count(list);
}
//------------------------------------------------------------------------------
static
int man_list_create(void **state)
{
    list_t *list = malloc(sizeof(*list));
    if( !list ) return -1;

    list_init(list);

    *state = list;
    return 0;
}
//------------------------------------------------------------------------------
static
int man_list_release(void **state)
{
    list_t *list = *state;

    list_destroy(list);
    free(list);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void man_list_insert_test(void **state)
{
    list_t *list = *state;

    assert_true( is_list_empty(list) );

    element_t *ele_1 = element_create(1);
    element_t *ele_2 = element_create(2);
    element_t *ele_3 = element_create(3);
    element_t *ele_4 = element_create(4);
    element_t *ele_5 = element_create(5);
    element_t *ele_6 = element_create(6);

    {
        list_insert_last(list, ele_5);

        int target[] = { 5 };
        assert_true( compare_list(list, target) );
    }

    {
        list_insert_last(list, ele_6);

        int target[] = { 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_insert_first(list, ele_2);

        int target[] = { 2, 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_insert_first(list, ele_1);

        int target[] = { 1, 2, 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_iter_t pos = list_get_last(list);
        list_iter_move_prev(&pos);
        assert_true( list_iter_have_value(&pos) );

        list_insert(list, &pos, ele_3);

        int target[] = { 1, 2, 3, 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_iter_t pos = list_get_last(list);
        list_iter_move_prev(&pos);
        assert_true( list_iter_have_value(&pos) );

        list_insert(list, &pos, ele_4);

        int target[] = { 1, 2, 3, 4, 5, 6 };
        assert_true( compare_list(list, target) );
    }
}
//------------------------------------------------------------------------------
static
void man_list_erase_test(void **state)
{
    list_t *list = *state;

    {
        int target[] = { 1, 2, 3, 4, 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_erase_first(list);

        int target[] = { 2, 3, 4, 5, 6 };
        assert_true( compare_list(list, target) );
    }

    {
        list_erase_last(list);

        int target[] = { 2, 3, 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        list_iter_t pos = list_get_first(list);
        list_iter_move_next(&pos);
        assert_true( list_iter_have_value(&pos) );

        list_erase(list, &pos);

        int target[] = { 2, 4, 5 };
        assert_true( compare_list(list, target) );
    }
}
//------------------------------------------------------------------------------
static
void man_list_pop_test(void **state)
{
    list_t *list = *state;

    {
        int target[] = { 2, 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        list_iter_t pos = list_get_first(list);
        list_iter_move_next(&pos);
        assert_true( list_iter_have_value(&pos) );

        element_t *ele = list_pop(list, &pos);
        assert_non_null( ele );

        int target[] = { 2, 5 };
        assert_true( compare_list(list, target) );

        element_release(ele);
    }
}
//------------------------------------------------------------------------------
static
void man_list_clear_test(void **state)
{
    list_t *list = *state;

    {
        int target[] = { 2, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        list_clear(list);
        assert_true( is_list_empty(list) );
    }
}
//------------------------------------------------------------------------------
int test_man_list(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(man_list_insert_test),
        cmocka_unit_test(man_list_erase_test),
        cmocka_unit_test(man_list_pop_test),
        cmocka_unit_test(man_list_clear_test),
    };

    return cmocka_run_group_tests_name("managed list test", tests, man_list_create, man_list_release);
}
//------------------------------------------------------------------------------
