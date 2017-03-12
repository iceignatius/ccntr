General Container Library for C Language
========================================

Introduction
------------

This is a C library to offer a set of easy use general containers.

For each type of container,
we implement 3 sub types for different occasion and preference:

1. Pure container.

    The pure container concentrate on container operations,
    and do not involved in memory allocation and user data.
    That means user will be needs to handle menory allocation
    and data convertion by their self,
    but will have a pure and light container,
    or extend to their own container based on it.

    The pure container (class and functions)
    will be named as `ccntr_<type>_*`.

2. Memory managed container.

    The managed container handles and hide things about element nodes and memory,
    so that user can operator with user defined values
    without element management ant convertion.

    The managed container will be named as `ccntr_man_<type>_*`.

3. Container template.

    The container templates are similarly to managed container,
    but handles user data type convertion.
    user can build their container class with specific data type and class name
    by `CCNTR_<type>_TEMPLATE` macro.

    The container be built by template will be named as `<user_defined_name>_*`.

Types of Container
------------------

This library support the following types of container:

* Linked list.

Examples
--------

The following codes are two examples to show how to use this library.
We only use linked list type of container as example,
beacuse the others are similarly.

### List Template

    #include <stdlib.h>  // For memory allocation.
    #include <stdio.h>   // For message print.
    #include <ccntr.h>   // To use our containers.

    // We define our element data type here.
    typedef struct element_t
    {
        int value;
    } element_t;

    element_t* element_create(int value)
    {
        // Nothing but create and initialise our element object.
        element_t *ele = malloc(sizeof(element_t));
        ele->value = value;
        return ele;
    }

    void element_release(element_t *ele)
    {
        // Nothing but destroy our element object.
        free(ele);
    }

    // Use CCNTR_LIST_TEMPLATE macro to build or list class with
    // class name "mylist", and element type "element_t*".
    // Note that we assign the function "element_release" to the container,
    // then we will not need to release each element when we pop or erase it
    // from container.
    CCNTR_LIST_TEMPLATE(mylist, element_t*, (void(*)(void*))element_release)

    int main(int argc, char *argv[])
    {
        // Initialise list container.

        mylist_t list;
        mylist_init(&list);

        // Insert elements.

        element_t *ele = element_create(4);
        mylist_push_back(&list, ele);

        ele = element_create(6);
        mylist_push_back(&list, ele);

        ele = element_create(2);
        mylist_push_back(&list, ele);

        // Get container elements.

        printf("List dump (count=%u):", mylist_get_count(&list));
        for(mylist_citer_t iter = mylist_get_first_c(&list);
            mylist_citer_have_value(&iter);
            mylist_citer_move_next(&iter))
        {
            const element_t *ele = mylist_citer_get_value(&iter);
            printf(" %d", ele->value);
        }
        printf("\n");
        // We will got output for above codes:
        // "List dump (count=3): 2 4 6"

        // Finally, destroy the container when we do not use it any more.
        mylist_destroy(&list);

        return 0;
    }

### Pure List

    #include <stdlib.h>  // For memory allocation.
    #include <stdio.h>   // For message print.
    #include <ccntr.h>   // To use our containers.

    // We define our element data type here.
    typedef struct element_t
    {
        // There must have a container node as a member.
        //
        // And we chose to set the node as the first member of element,
        // so that the address of node and element will be the same.
        // But it is not necessary to set the node as the first member,
        // User can use something like "container_of macro"
        // to get element by node.
        ccntr_list_node_t node;

        // The following variables are user defined members.
        int value;

    } element_t;

    element_t* element_create(int value)
    {
        // Nothing but create and initialise our element object.
        element_t *ele = malloc(sizeof(element_t));
        ele->value = value;
        return ele;
    }

    void element_release(element_t *ele)
    {
        // Nothing but destroy our element object.
        free(ele);
    }

    int main(int argc, char *argv[])
    {
        // Initialise list container.

        ccntr_list_t list;
        ccntr_list_init(&list);

        // Insert elements.

        element_t *ele = element_create(4);
        ccntr_list_link_back(&list, &ele->node);

        ele = element_create(6);
        ccntr_list_link_back(&list, &ele->node);

        ele = element_create(2);
        ccntr_list_link_front(&list, &ele->node);

        // Get container elements.

        printf("List dump (count=%u):", ccntr_list_get_count(&list));
        for(const ccntr_list_node_t *node = ccntr_list_get_first_c(&list);
            node;
            node = node->next)
        {
            const element_t *ele = (const element_t*) node;
            printf(" %d", ele->value);
        }
        printf("\n");
        // We will got output for above codes:
        // "List dump (count=3): 2 4 6"

        // Pop and release all elements.
        //
        // We need to get all nodes and free it by ourself,
        // because the container does not handle memory management.
        for(ccntr_list_node_t *node = ccntr_list_get_first(&list);
            node;
            node = node->next)
        {
            element_t *ele = (element_t*) node;
            ccntr_list_unlink(&list, &ele->node);  // Remark (1).
            element_release(ele);
        }

        ccntr_list_discard_all(&list);  // Remark (2).

        // Remarks:
        //
        // If we are finish using the container,
        // then unlink each nodes (1) are not necessary.
        // We can just go through all nodes and free it,
        // then use discard function (2)
        // to discard all nodes in the container.

        return 0;
    }
