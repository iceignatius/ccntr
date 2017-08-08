#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "ccntr.h"
#include "test_man_array.h"

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
static
int element_compare(const element_t **ele1, const element_t **ele2)
{
    return ele1[0]->value - ele2[0]->value;
}
//------------------------------------------------------------------------------
CCNTR_DECLARE_ARRAY(array,
                    element_t*,
                    (int(*)(const void**,const void**)) element_compare,
                    (void(*)(void*)) element_release)
//------------------------------------------------------------------------------
#define compare_array(array, target) compare_array_ptr(array, target, sizeof(target)/sizeof(target[0]))
static
bool compare_array_ptr(const array_t *array, int tararr[], unsigned count)
{
    if( array_get_count(array) != count ) return false;

    const element_t **elearr = array_baseptr_c(array);
    for(unsigned i=0; i<count; ++i)
    {
        if( elearr[i]->value != tararr[i] )
            return false;
    }

    return true;
}
//------------------------------------------------------------------------------
static
void man_array_insert_test(void **state)
{
    array_t array;
    array_init(&array);

    {
        array_insert_last(&array, element_create(5));

        int target[] = { 5 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_insert_last(&array, element_create(6));

        int target[] = { 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_insert_first(&array, element_create(2));

        int target[] = { 2, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_insert_first(&array, element_create(1));

        int target[] = { 1, 2, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_insert(&array, 2, element_create(4));

        int target[] = { 1, 2, 4, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_insert(&array, 2, element_create(3));

        int target[] = { 1, 2, 3, 4, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    array_destroy(&array);
}
//------------------------------------------------------------------------------
static
void man_array_replace_test(void **state)
{
    array_t array;
    array_init(&array);

    {
        array_insert_last(&array, element_create(1));
        array_insert_last(&array, element_create(3));
        array_insert_last(&array, element_create(5));

        int target[] = { 1, 3, 5 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_replace(&array, 0, element_create(2));

        int target[] = { 2, 3, 5 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_replace(&array, 1, element_create(4));

        int target[] = { 2, 4, 5 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_replace(&array, 2, element_create(6));

        int target[] = { 2, 4, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_replace(&array, 3, element_create(8));

        int target[] = { 2, 4, 6, 8 };
        assert_true( compare_array(&array, target) );
    }

    array_destroy(&array);
}
//------------------------------------------------------------------------------
static
void man_array_erase_test(void **state)
{
    array_t array;
    array_init(&array);

    {
        array_insert_last(&array, element_create(1));
        array_insert_last(&array, element_create(2));
        array_insert_last(&array, element_create(3));
        array_insert_last(&array, element_create(4));
        array_insert_last(&array, element_create(5));
        array_insert_last(&array, element_create(6));

        int target[] = { 1, 2, 3, 4, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase(&array, 2);

        int target[] = { 1, 2, 4, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase(&array, 2);

        int target[] = { 1, 2, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase_last(&array);

        int target[] = { 1, 2, 5 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase_last(&array);

        int target[] = { 1, 2 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase_first(&array);

        int target[] = { 2 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_erase_first(&array);
        assert_int_equal( array_get_count(&array), 0 );
    }

    array_destroy(&array);
}
//------------------------------------------------------------------------------
static
void man_array_clear_test(void **state)
{
    array_t array;
    array_init(&array);

    {
        array_insert_last(&array, element_create(1));
        array_insert_last(&array, element_create(2));
        array_insert_last(&array, element_create(3));
        array_insert_last(&array, element_create(4));
        array_insert_last(&array, element_create(5));
        array_insert_last(&array, element_create(6));

        int target[] = { 1, 2, 3, 4, 5, 6 };
        assert_true( compare_array(&array, target) );
    }

    array_clear(&array);
    assert_int_equal( array_get_count(&array), 0 );

    array_destroy(&array);
}
//------------------------------------------------------------------------------
static
void man_array_access_test(void **state)
{
    array_t array;
    array_init(&array);

    assert_null( array_get(&array, 0) );
    assert_null( array_get_first(&array) );
    assert_null( array_get_last(&array) );

    {
        array_insert_last(&array, element_create(1));
        array_insert_last(&array, element_create(3));
        array_insert_last(&array, element_create(5));

        int target[] = { 1, 3, 5 };
        assert_true( compare_array(&array, target) );
    }

    assert_int_equal( array_get(&array, 0)->value, 1 );
    assert_int_equal( array_get(&array, 1)->value, 3 );
    assert_int_equal( array_get(&array, 2)->value, 5 );
    assert_null( array_get(&array, 3) );

    assert_int_equal( array_get_first(&array)->value, 1 );
    assert_int_equal( array_get_last(&array)->value, 5 );

    array_destroy(&array);
}
//------------------------------------------------------------------------------
static
void man_array_sort_test(void **state)
{
    array_t array;
    array_init(&array);

    {
        array_insert_last(&array, element_create(2));
        array_insert_last(&array, element_create(5));
        array_insert_last(&array, element_create(1));
        array_insert_last(&array, element_create(3));
        array_insert_last(&array, element_create(8));
        array_insert_last(&array, element_create(4));
        array_insert_last(&array, element_create(6));
        array_insert_last(&array, element_create(7));

        int target[] = { 2, 5, 1, 3, 8, 4, 6, 7 };
        assert_true( compare_array(&array, target) );
    }

    {
        array_sort(&array);

        int target[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        assert_true( compare_array(&array, target) );
    }

    array_destroy(&array);
}
//------------------------------------------------------------------------------
int test_man_array(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(man_array_insert_test),
        cmocka_unit_test(man_array_replace_test),
        cmocka_unit_test(man_array_erase_test),
        cmocka_unit_test(man_array_clear_test),
        cmocka_unit_test(man_array_access_test),
        cmocka_unit_test(man_array_sort_test),
    };

    return cmocka_run_group_tests_name("managed array test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
