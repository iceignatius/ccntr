General Container Library for C Language
========================================

Introduction
------------

This is a C library to offer a set of easy use general containers.

Features
--------

* This library support the following types of container:

    * Array.
    * Linked list.
    * Queue (first in, first out list).
    * Stack (last in, first out list).
    * Key map.

* Suppot multiple sub types of container:

    * Container templates (implemented by macros) that can be used easily.
    * Pure container without memory management and iterator
        that can be used on embedd environment.

    Please see the following sections for more information in details.

* Optional thread safe behaviour.

    We build a compile-time option to enable or disable the thread safe behaviour,
    so that users can operate container in multiple threads without external locks.
    (This option is disabled by default)

    The following are examples to enable and disable thread safe behaviour:

        cmake -DCCNTR_THREAD_SAFE=ON /path/to/source

        cmake -DCCNTR_THREAD_SAFE=OFF /path/to/source

    (Please notice that the thread safe behaviour is design to
    protect the container it self without iterators!)

Sub Types
---------

For each major type of container,
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
    by `CCNTR_DECLARE_<type>` macro.

    The container be built by template will be named as `<user_defined_name>_*`.

Documentation
-------------

*Doxygen* can be used to generate the full document,
and please refer that to use library in details:

    doxygen

By the other way, Examples in the next session can be used to quick start.

Examples
--------

The following codes are two examples to show how to use this library.
We only use linked list container as examples,
and the others are similarly.

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
        // Create and initialise our element object.
        element_t *ele = malloc(sizeof(element_t));
        ele->value = value;
        return ele;
    }

    void element_release(element_t *ele)
    {
        // Destroy our element object.
        free(ele);
    }

    // Use CCNTR_DECLARE_LIST macro to build our list class with
    // class name "mylist", and element type "element_t*".
    // Note that we assign the function "element_release" to the container,
    // then we will not need to release each element when we erase it
    // from container.
    CCNTR_DECLARE_LIST(mylist, element_t*, (void(*)(void*))element_release)

    int main(int argc, char *argv[])
    {
        // Initialise list container.

        mylist_t list;
        mylist_init(&list);

        // Insert elements.

        element_t *ele = element_create(4);
        mylist_insert_last(&list, ele);

        ele = element_create(6);
        mylist_insert_last(&list, ele);

        ele = element_create(2);
        mylist_insert_first(&list, ele);

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
        // User can use something like "container_of" macro
        // to get element by node.
        ccntr_list_node_t node;

        // The following variables are user defined member(s).
        int value;

    } element_t;

    element_t* element_create(int value)
    {
        // Create and initialise our element object.
        element_t *ele = malloc(sizeof(element_t));
        ele->value = value;
        return ele;
    }

    void element_release(element_t *ele)
    {
        // Destroy our element object.
        free(ele);
    }

    int main(int argc, char *argv[])
    {
        // Initialise list container.

        ccntr_list_t list;
        ccntr_list_init(&list);

        // Insert elements.

        element_t *ele = element_create(4);
        ccntr_list_link_last(&list, &ele->node);

        ele = element_create(6);
        ccntr_list_link_last(&list, &ele->node);

        ele = element_create(2);
        ccntr_list_link_first(&list, &ele->node);

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
        // If we are going to erase all elements,
        // then unlink each nodes (1) are not necessary.
        // We can just go through all nodes and free it,
        // then use discard function (2) to discard all nodes in the container.
        //
        // If you are finished using the container,
        // then you can even free all elements and just leave the container there.
        // The key point is that
        // pure containers do nothing about memory management.

        return 0;
    }
