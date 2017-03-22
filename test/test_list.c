#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "container_of.h"
#include "ccntr.h"
#include "test_list.h"

typedef struct element_t
{
    ccntr_list_node_t node;
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
#define compare_list(list, target) compare_list_and_array(list, target, sizeof(target)/sizeof(target[0]))
static
bool compare_list_and_array(const ccntr_list_t *list, int array[], unsigned count)
{
    // Compare elements count.
    if( ccntr_list_get_count(list) != count ) return false;

    // Access forward and compare elements.
    const ccntr_list_node_t *node = ccntr_list_get_first_c(list);
    for(int i = 0; i < (int)count; ++i, node = node->next)
    {
        if( !node ) return false;

        const element_t *ele = container_of(node, element_t, node);
        if( ele->value != array[i] ) return false;
    }

    // Access backward and compare elements.
    node = ccntr_list_get_last_c(list);
    for(int i = (int)count - 1; i >= 0; --i, node = node->prev)
    {
        if( !node ) return false;

        const element_t *ele = container_of(node, element_t, node);
        if( ele->value != array[i] ) return false;
    }

    return true;
}
//------------------------------------------------------------------------------
static
bool is_list_empty(const ccntr_list_t *list)
{
    return !ccntr_list_get_count(list) &&
           !ccntr_list_get_first_c(list) &&
           !ccntr_list_get_last_c(list);
}
//------------------------------------------------------------------------------
static
int list_create(void **state)
{
    ccntr_list_t *list = malloc(sizeof(*list));
    if( !list ) return -1;

    ccntr_list_init(list);

    *state = list;
    return 0;
}
//------------------------------------------------------------------------------
static
int list_release(void **state)
{
    ccntr_list_t *list = *state;

    free(list);

    *state = NULL;
    return 0;
}
//------------------------------------------------------------------------------
static
void list_insert_test(void **state)
{
    ccntr_list_t *list = *state;

    assert_true( is_list_empty(list) );

    element_t *ele_1 = element_create(1);
    element_t *ele_2 = element_create(2);
    element_t *ele_3 = element_create(3);
    element_t *ele_4 = element_create(4);
    element_t *ele_5 = element_create(5);

    {
        ccntr_list_link_last(list, &ele_4->node);

        int target[] = { 4 };
        assert_true( compare_list(list, target) );
    }

    {
        ccntr_list_link_last(list, &ele_5->node);

        int target[] = { 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        ccntr_list_link_first(list, &ele_2->node);

        int target[] = { 2, 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        ccntr_list_link_first(list, &ele_1->node);

        int target[] = { 1, 2, 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        ccntr_list_link(list, &ele_4->node, &ele_3->node);

        int target[] = { 1, 2, 3, 4, 5 };
        assert_true( compare_list(list, target) );
    }
}
//------------------------------------------------------------------------------
static
void list_erase_test(void **state)
{
    ccntr_list_t *list = *state;

    {
        int target[] = { 1, 2, 3, 4, 5 };
        assert_true( compare_list(list, target) );
    }

    {
        ccntr_list_node_t *node = ccntr_list_get_first(list);
        assert_non_null(node);

        ccntr_list_unlink(list, node);

        int target[] = { 2, 3, 4, 5 };
        assert_true( compare_list(list, target) );

        element_t *ele = container_of(node, element_t, node);
        element_release(ele);
    }

    {
        ccntr_list_node_t *node = ccntr_list_get_last(list);
        assert_non_null(node);

        ccntr_list_unlink(list, node);

        int target[] = { 2, 3, 4 };
        assert_true( compare_list(list, target) );

        element_t *ele = container_of(node, element_t, node);
        element_release(ele);
    }

    {
        ccntr_list_node_t *node = ccntr_list_get_first(list);
        assert_non_null(node);
        assert_non_null( node = node->next );

        ccntr_list_unlink(list, node);

        int target[] = { 2, 4 };
        assert_true( compare_list(list, target) );

        element_t *ele = container_of(node, element_t, node);
        element_release(ele);
    }

    {
        ccntr_list_node_t *node = ccntr_list_get_first(list);
        while( node )
        {
            element_t *ele = container_of(node, element_t, node);
            node = node->next;

            element_release(ele);
        }

        ccntr_list_discard_all(list);
        assert_true( is_list_empty(list) );
    }
}
//------------------------------------------------------------------------------
int test_list(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(list_insert_test),
        cmocka_unit_test(list_erase_test),
    };

    return cmocka_run_group_tests_name("list test", tests, list_create, list_release);
}
//------------------------------------------------------------------------------
