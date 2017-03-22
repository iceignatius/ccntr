/**
 * @file
 * @brief     Container: queue (memory managed).
 * @author    王文佑
 * @date      2017/03/22
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_QUEUE_H_
#define _CCNTR_MAN_QUEUE_H_

#include "ccntr_config.h"
#include "ccntr_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_MAN_QUEUE_ENABLED

/**
 * @brief Release value.
 * @details Callback that will be called when container want release a value.
 *
 * @param value The value to be released.
 */
typedef void(*ccntr_man_queue_release_value_t)(void *value);

/**
 * @class ccntr_man_queue_t
 * @brief Linked list container.
 */
typedef struct ccntr_man_queue_t
{
    ccntr_queue_t super;

    ccntr_man_queue_release_value_t release_value;

} ccntr_man_queue_t;

void ccntr_man_queue_init(ccntr_man_queue_t *self, ccntr_man_queue_release_value_t release_value);
void ccntr_man_queue_destroy(ccntr_man_queue_t *self);

unsigned ccntr_man_queue_get_count(const ccntr_man_queue_t *self);

void* ccntr_man_queue_get_current(ccntr_man_queue_t *self);
const void* ccntr_man_queue_get_current_c(const ccntr_man_queue_t *self);

void ccntr_man_queue_push(ccntr_man_queue_t *self, void *value);
void* ccntr_man_queue_pop(ccntr_man_queue_t *self);
void ccntr_man_queue_erase_current(ccntr_man_queue_t *self);

void ccntr_man_queue_clear(ccntr_man_queue_t *self);

#endif  // CCNTR_MAN_QUEUE_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
