#ifndef _CCNTR_SPINLOCK_H_
#define _CCNTR_SPINLOCK_H_

#include "ccntr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_THREAD_SAFE

typedef struct ccntr_spinlock
{
    unsigned char data[8];
} ccntr_spinlock_t;

#define CCNTR_DECLARE_SPINLOCK(name) ccntr_spinlock_t name

void ccntr_spinlock_init(ccntr_spinlock_t *self);
void ccntr_spinlock_lock(ccntr_spinlock_t *self);
void ccntr_spinlock_unlock(ccntr_spinlock_t *self);

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
