/**
 * @file
 * @brief     Container: linked list (memory managed).
 * @author    王文佑
 * @date      2017/08/06
 * @copyright ZLib Licence
 */
#ifndef _CCNTR_MAN_ARRAY_H_
#define _CCNTR_MAN_ARRAY_H_

#include "ccntr_spinlock.h"

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
typedef int(*ccntr_man_array_compare_values_t)(const void **val1, const void **val2);

/**
 * @class ccntr_man_array_t
 * @brief Array container.
 */
typedef struct ccntr_man_array_t
{
    void     **elements;
    unsigned   capacity;
    unsigned   count;

    ccntr_man_array_compare_values_t compare;
    ccntr_man_array_release_value_t  release_value;

    CCNTR_DECLARE_SPINLOCK(lock);

} ccntr_man_array_t;

void ccntr_man_array_init(ccntr_man_array_t                *self,
                          ccntr_man_array_compare_values_t  compare,
                          ccntr_man_array_release_value_t   release_value);
void ccntr_man_array_destroy(ccntr_man_array_t *self);

static inline
unsigned ccntr_man_array_get_count(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get elements count.
     *
     * @param self Object instance.
     * @return The elements count.
     */
    ccntr_spinlock_lock( (ccntr_spinlock_t*) &self->lock );
    unsigned count = self->count;
    ccntr_spinlock_unlock( (ccntr_spinlock_t*) &self->lock );

    return count;
}

static inline
void** ccntr_man_array_baseptr(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get a direct pointer to the data array.
     *
     * @param self Object instance.
     * @return The data array.
     *
     * @note This function is not thread safe!
     */
    return self->elements;
}

static inline
void const* const* ccntr_man_array_baseptr_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get a direct pointer to the data array.
     *
     * @param self Object instance.
     * @return The data array.
     *
     * @note This function is not thread safe!
     */
    return (const void*) self->elements;
}

static inline
void* ccntr_man_array_get_first(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the first element.
     *
     * @param self Object instance.
     * @return The first element; or NULL if container is empty.
     */
    ccntr_spinlock_lock(&self->lock);
    void *value = self->count ? self->elements[0] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
}

static inline
const void* ccntr_man_array_get_first_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the first element.
     *
     * @param self Object instance.
     * @return The first element; or NULL if container is empty.
     */
    return ccntr_man_array_get_first((ccntr_man_array_t*)self);
}

static inline
void* ccntr_man_array_get_last(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the last element.
     *
     * @param self Object instance.
     * @return The last element; or NULL if container is empty.
     */
    ccntr_spinlock_lock(&self->lock);
    void *value = self->count ? self->elements[ self->count - 1 ] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
}

static inline
const void* ccntr_man_array_get_last_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the last element.
     *
     * @param self Object instance.
     * @return The last element; or NULL if container is empty.
     */
    return ccntr_man_array_get_last((ccntr_man_array_t*)self);
}

static inline
void* ccntr_man_array_get(ccntr_man_array_t *self, unsigned index)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get element by index.
     *
     * @param self  Object instance.
     * @param index Index of the element.
     * @return The element at the position; or NULL if the index is out of range.
     */
    ccntr_spinlock_lock(&self->lock);
    void *value = index < self->count ? self->elements[index] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
}

static inline
const void* ccntr_man_array_get_c(const ccntr_man_array_t *self, unsigned index)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get element by index.
     *
     * @param self  Object instance.
     * @param index Index of the element.
     * @return The element at the position; or NULL if the index is out of range.
     */
    return ccntr_man_array_get((ccntr_man_array_t*)self, index);
}

void ccntr_man_array_insert(ccntr_man_array_t *self, unsigned index, void *value);

static inline
void ccntr_man_array_insert_first(ccntr_man_array_t *self, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Insert a value to the first position.
     *
     * @param self  Object instance.
     * @param value The value to be inserted.
     */
    ccntr_man_array_insert(self, 0, value);
}

static inline
void ccntr_man_array_insert_last(ccntr_man_array_t *self, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Insert a value to the last position.
     *
     * @param self  Object instance.
     * @param value The value to be inserted.
     */
    ccntr_man_array_insert(self, self->count, value);
}

void ccntr_man_array_replace(ccntr_man_array_t *self, unsigned index, void *value);

void ccntr_man_array_erase(ccntr_man_array_t *self, unsigned index);

static inline
void ccntr_man_array_erase_first(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase the first value.
     *
     * @param self Object instance.
     */
    ccntr_man_array_erase(self, 0);
}

static inline
void ccntr_man_array_erase_last(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase the last value.
     *
     * @param self Object instance.
     */
    if( self->count )
        ccntr_man_array_erase(self, self->count - 1);
}

void ccntr_man_array_clear(ccntr_man_array_t *self);

void ccntr_man_array_sort(ccntr_man_array_t *self);

#endif  // CCNTR_MAN_ARRAY_ENABLED

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
