#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "ccntr.h"
#include "test_man_map.h"

typedef struct testkey_t
{
    int value;
} testkey_t;

typedef struct element_t
{
    // For test purpose, we defined that:
    // element_value = 2 * key_value
    int value;
} element_t;

//------------------------------------------------------------------------------
static
testkey_t* testkey_create(int value)
{
    testkey_t *key = malloc(sizeof(testkey_t));
    key->value = value;

    return key;
}
//------------------------------------------------------------------------------
static
void testkey_release(testkey_t *key)
{
    free(key);
}
//------------------------------------------------------------------------------
static
int testkey_compare(const testkey_t *key1, const testkey_t *key2)
{
    return key1->value - key2->value;
}
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
CCNTR_DECLARE_MAP(map,
                  testkey_t*,
                  element_t*,
                  (int(*)(const void*,const void*)) testkey_compare,
                  (void(*)(void*)) testkey_release,
                  (void(*)(void*)) element_release)
//------------------------------------------------------------------------------
static
int man_map_create(void **state)
{
    map_t *map = malloc(sizeof(map_t));
    if( !map ) return -1;

    map_init(map);

    *state = map;
    return 0;
}
//------------------------------------------------------------------------------
static
int man_map_release(void **state)
{
    map_t *map = *state;

    map_destroy(map);
    free(map);

    *state = 0;
    return 0;
}
//------------------------------------------------------------------------------
static
void man_map_insert_test(void **state)
{
    map_t *map = *state;

    assert_int_equal( map_get_count(map), 0 );

    map_insert(map, testkey_create(1), element_create(2*1));
    assert_int_equal( map_get_count(map), 1 );

    map_insert(map, testkey_create(3), element_create(2*3));
    assert_int_equal( map_get_count(map), 2 );

    map_insert(map, testkey_create(5), element_create(2*5));
    assert_int_equal( map_get_count(map), 3 );

    map_insert(map, testkey_create(7), element_create(2*7));
    assert_int_equal( map_get_count(map), 4 );

    map_insert(map, testkey_create(9), element_create(2*9));
    assert_int_equal( map_get_count(map), 5 );
}
//------------------------------------------------------------------------------
static
void man_map_duplicated_insert_test(void **state)
{
    map_t *map = *state;

    assert_int_equal( map_get_count(map), 5 );

    map_insert(map, testkey_create(1), element_create(2*1));
    assert_int_equal( map_get_count(map), 5 );

    map_insert(map, testkey_create(5), element_create(2*5));
    assert_int_equal( map_get_count(map), 5 );

    map_insert(map, testkey_create(9), element_create(2*9));
    assert_int_equal( map_get_count(map), 5 );
}
//------------------------------------------------------------------------------
static
void man_map_iterate_test(void **state)
{
    map_t *map = *state;

    // Iterate next.

    map_citer_t iter = map_get_first_c(map);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 1 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*1 );

    map_citer_move_next(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 3 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*3 );

    map_citer_move_next(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 5 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*5 );

    map_citer_move_next(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 7 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*7 );

    map_citer_move_next(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 9 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*9 );

    map_citer_move_next(&iter);
    assert_false( map_citer_have_value(&iter) );
    assert_null( map_citer_get_key(&iter) );
    assert_null( map_citer_get_value(&iter) );

    // Iterate previous.

    iter = map_get_last_c(map);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 9 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*9 );

    map_citer_move_prev(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 7 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*7 );

    map_citer_move_prev(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 5 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*5 );

    map_citer_move_prev(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 3 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*3 );

    map_citer_move_prev(&iter);
    assert_true( map_citer_have_value(&iter) );
    assert_non_null( map_citer_get_key(&iter) );
    assert_non_null( map_citer_get_value(&iter) );
    assert_int_equal( map_citer_get_key(&iter)->value, 1 );
    assert_int_equal( map_citer_get_value(&iter)->value, 2*1 );

    map_citer_move_prev(&iter);
    assert_false( map_citer_have_value(&iter) );
    assert_null( map_citer_get_key(&iter) );
    assert_null( map_citer_get_value(&iter) );
}
//------------------------------------------------------------------------------
static
void man_map_find_test(void **state)
{
    map_t *map = *state;

    testkey_t key;
    const element_t *ele;

    key.value = 0;
    ele = map_find_value_c(map, &key);
    assert_null( ele );

    key.value = 1;
    ele = map_find_value_c(map, &key);
    assert_non_null( ele );
    assert_int_equal( ele->value, 2*key.value);

    key.value = 2;
    ele = map_find_value_c(map, &key);
    assert_null( ele );

    key.value = 3;
    ele = map_find_value_c(map, &key);
    assert_non_null( ele );
    assert_int_equal( ele->value, 2*key.value);

    key.value = 4;
    ele = map_find_value_c(map, &key);
    assert_null( ele );

    key.value = 5;
    ele = map_find_value_c(map, &key);
    assert_non_null( ele );
    assert_int_equal( ele->value, 2*key.value);

    key.value = 6;
    ele = map_find_value_c(map, &key);
    assert_null( ele );

    key.value = 7;
    ele = map_find_value_c(map, &key);
    assert_non_null( ele );
    assert_int_equal( ele->value, 2*key.value);

    key.value = 8;
    ele = map_find_value_c(map, &key);
    assert_null( ele );

    key.value = 9;
    ele = map_find_value_c(map, &key);
    assert_non_null( ele );
    assert_int_equal( ele->value, 2*key.value);
}
//------------------------------------------------------------------------------
static
void man_map_erase_test(void **state)
{
    map_t *map = *state;

    assert_int_equal( map_get_count(map), 5 );

    testkey_t key = {3};
    map_erase_by_key(map, &key);
    assert_null( map_find_value_c(map, &key) );
    assert_int_equal( map_get_count(map), 4 );
}
//------------------------------------------------------------------------------
static
void man_map_pop_test(void **state)
{
    map_t *map = *state;

    assert_int_equal( map_get_count(map), 4 );

    testkey_t target = {7};
    map_iter_t pos = map_find(map, &target);
    assert_true( map_iter_have_value(&pos) );

    testkey_t *key = map_iter_get_key(&pos);
    element_t *ele = map_iter_get_value(&pos);
    assert_non_null( key );
    assert_non_null( ele );

    assert_ptr_equal( map_pop(map, &pos), ele );
    assert_null( map_find_value(map, &target) );
    assert_int_equal( map_get_count(map), 3 );

    testkey_release(key);
    element_release(ele);
}
//------------------------------------------------------------------------------
static
void man_map_clear_test(void **state)
{
    map_t *map = *state;

    assert_int_equal( map_get_count(map), 3 );

    map_clear(map);
    assert_int_equal( map_get_count(map), 0 );
}
//------------------------------------------------------------------------------
int test_man_map(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(man_map_insert_test),
        cmocka_unit_test(man_map_duplicated_insert_test),
        cmocka_unit_test(man_map_iterate_test),
        cmocka_unit_test(man_map_find_test),
        cmocka_unit_test(man_map_erase_test),
        cmocka_unit_test(man_map_pop_test),
        cmocka_unit_test(man_map_clear_test),
    };

    return cmocka_run_group_tests_name("managed map test", tests, man_map_create, man_map_release);
}
//------------------------------------------------------------------------------
