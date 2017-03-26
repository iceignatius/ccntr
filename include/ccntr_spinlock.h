#ifndef _CCNTR_SPINLOCK_H_
#define _CCNTR_SPINLOCK_H_

#include "ccntr_config.h"

#ifdef CCNTR_THREAD_SAFE
#include <stdatomic.h>  // Must include this after the configure header to get the correct configurations!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_THREAD_SAFE

typedef atomic_flag ccntr_spinlock_t;

#define CCNTR_DECLARE_SPINLOCK(name) ccntr_spinlock_t name

static inline
void ccntr_spinlock_init(ccntr_spinlock_t *self)
{
    static const atomic_flag flag0 = ATOMIC_FLAG_INIT;
    *self = flag0;
}

static inline
void ccntr_spinlock_lock(ccntr_spinlock_t *self)
{
    while( atomic_flag_test_and_set(self) )
    {}
}

static inline
void ccntr_spinlock_unlock(ccntr_spinlock_t *self)
{
    atomic_flag_clear(self);
}

#else  // CCNTR_THREAD_SAFE

#define CCNTR_DECLARE_SPINLOCK(name)

#define ccntr_spinlock_init(self)
#define ccntr_spinlock_lock(self)
#define ccntr_spinlock_unlock(self)

#endif  // CCNTR_THREAD_SAFE

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
