/**
 * @file
 * @brief     Container: linked list (memory managed).
 * @author    王文佑
 * @date      2017/08/06
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_ARRAY_H_
#define _CCNTR_MAN_ARRAY_H_

#include "ccntr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CCNTR_MAN_ARRAY_ENABLED

/**
 * @brief Release value.
 * @details Callback that will be called when container want release a value.
 *
 * @param value The value to be released.
 */
typedef void(*ccntr_man_array_release_value_t)(void *value);

/**
 * @brief Compare values.
 * @details Callback that will be called when container want to compare values.
 *
 * @param val1 The first value.
 * @param val2 The second value.
 * @retval <0 Value @a val1 goes before value @a val2.
 * @retval 0  Values are equivalents.
 * @retval >0 Value @a val1 goes after value @a val2.
 */
typedef int(*ccntr_man_array_compare_values_t)(const void *val1, const void *val2);

/**
 * @class ccntr_man_array_t
 * @brief Array container.
 */
typedef struct ccntr_man_array_t
{
} ccntr_man_array_t;

void ccntr_man_array_init(ccntr_man_array_t               *self,
                          ccntr_man_array_compare_values_t compare,
                          ccntr_man_array_release_value_t  release_value);
void ccntr_man_array_destroy(ccntr_man_array_t *self);

unsigned ccntr_man_array_get_count(const ccntr_man_array_t *self);

void* ccntr_man_array_baseptr(ccntr_man_array_t *self);
const void* ccntr_man_array_baseptr_c(const ccntr_man_array_t *self);

void* ccntr_man_array_get_first(ccntr_man_array_t *self);
const void* ccntr_man_array_get_first_c(const ccntr_man_array_t *self);
void* ccntr_man_array_get_last(ccntr_man_array_t *self);
const void* ccntr_man_array_get_last_c(const ccntr_man_array_t *self);
void* ccntr_man_array_get(ccntr_man_array_t *self, unsigned index);
const void* ccntr_man_array_get_c(const ccntr_man_array_t *self, unsigned index);

void ccntr_man_array_insert(ccntr_man_array_t *self, unsigned index, void *value);
void ccntr_man_array_insert_first(ccntr_man_array_t *self, void *value);
void ccntr_man_array_insert_last(ccntr_man_array_t *self, void *value);

void ccntr_man_array_replace(ccntr_man_array_t *self, unsigned index, void *value);

void ccntr_man_array_erase(ccntr_man_array_t *self, unsigned index);
void ccntr_man_array_erase_first(ccntr_man_array_t *self);
void ccntr_man_array_erase_last(ccntr_man_array_t *self);

void ccntr_man_array_clear(ccntr_man_array_t *self);

void ccntr_man_array_sort(ccntr_man_array_t *self);

#endif  // CCNTR_MAN_ARRAY_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
