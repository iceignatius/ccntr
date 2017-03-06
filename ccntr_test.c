#include "test_list.h"
#include "test_man_list.h"

int main(void)
{
    int ret;

    if(( ret = test_list() )) return ret;
    if(( ret = test_man_list() )) return ret;

    return 0;
}
