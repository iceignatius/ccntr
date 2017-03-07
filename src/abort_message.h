#ifndef _ABORT_MESSAGE_H_
#define _ABORT_MESSAGE_H_

#include <stdlib.h>
#include <stdio.h>

static inline
void abort_message(const char *msg)
{
    fputs(msg, stderr);
    abort();
}

#endif
