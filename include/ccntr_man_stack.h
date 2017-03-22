/**
 * @file
 * @brief     Container: stack (memory managed).
 * @author    王文佑
 * @date      2017/03/22
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_STACK_H_
#define _CCNTR_MAN_STACK_H_

#include "ccntr_config.h"
#include "ccntr_stack.h"

#ifdef __cplusplus
extern "C" {
#endif

#warning Temporary hide!
//#ifdef CCNTR_MAN_STACK_ENABLED

/**
 * @brief Release value.
 * @details Callback that will be called when container want release a value.
 *
 * @param value The value to be released.
 */
typedef void(*ccntr_man_stack_release_value_t)(void *value);

/**
 * @class ccntr_man_stack_t
 * @brief Linked list container.
 */
typedef struct ccntr_man_stack_t
{
} ccntr_man_stack_t;

void ccntr_man_stack_init(ccntr_man_stack_t *self, ccntr_man_stack_release_value_t release_value);
void ccntr_man_stack_destroy(ccntr_man_stack_t *self);

unsigned ccntr_man_stack_get_count(const ccntr_man_stack_t *self);

void* ccntr_man_stack_get_current(ccntr_man_stack_t *self);
const void* ccntr_man_stack_get_current_c(const ccntr_man_stack_t *self);

void ccntr_man_stack_push(ccntr_man_stack_t *self, void *value);
void* ccntr_man_stack_pop(ccntr_man_stack_t *self);
void ccntr_man_stack_erase_current(ccntr_man_stack_t *self);

void ccntr_man_stack_clear(ccntr_man_stack_t *self);

#warning Temporary hide!
//#endif  // CCNTR_MAN_STACK_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
