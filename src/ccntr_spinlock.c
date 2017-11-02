#include "ccntr_spinlock.h"

// This must come after the configure header to get the correct MACRO.
#ifdef CCNTR_THREAD_SAFE

#include <assert.h>
#include <stdatomic.h>
#include <string.h>

void ccntr_spinlock_init(ccntr_spinlock_t *self)
{
    static const atomic_flag flag0 = ATOMIC_FLAG_INIT;

    assert( sizeof(self->data) >= sizeof(flag0) );
    memcpy(self->data, &flag0, sizeof(flag0));
}

void ccntr_spinlock_lock(ccntr_spinlock_t *self)
{
    while( atomic_flag_test_and_set((atomic_flag*)self->data) )
    {}
}

void ccntr_spinlock_unlock(ccntr_spinlock_t *self)
{
    atomic_flag_clear((atomic_flag*)self->data);
}

#endif
