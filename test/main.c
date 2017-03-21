#include "test_list.h"
#include "test_man_list.h"
#include "test_map.h"
#include "test_man_map.h"

int main(void)
{
    int ret;

    if(( ret = test_list() )) return ret;
    if(( ret = test_man_list() )) return ret;
    if(( ret = test_map() )) return ret;
    if(( ret = test_man_map() )) return ret;

    return 0;
}
