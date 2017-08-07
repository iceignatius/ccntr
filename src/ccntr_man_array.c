#include "ccntr_man_array.h"

#ifdef CCNTR_MAN_ARRAY_ENABLED

//------------------------------------------------------------------------------
void ccntr_man_array_init(ccntr_man_array_t               *self,
                          ccntr_man_array_compare_values_t compare,
                          ccntr_man_array_release_value_t  release_value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Constructor.
     *
     * @param self          Object instance.
     * @param compare       Callback to compare values,
     *                      and can be NULL to not support value comparison.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @attention Object must be initialised (and once only) before using.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_destroy(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @attention Object must be destructed to finish using,
     *            and must not make any operation to the object after it be destructed.
     */
}
//------------------------------------------------------------------------------
unsigned ccntr_man_array_get_count(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get elements count.
     *
     * @param self Object instance.
     * @return The elements count.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_array_baseptr(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get a direct pointer to the data array.
     *
     * @param self Object instance.
     * @return The data array.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_array_baseptr_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get a direct pointer to the data array.
     *
     * @param self Object instance.
     * @return The data array.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_array_get_first(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the first element.
     *
     * @param self Object instance.
     * @return The first element; or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_array_get_first_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the first element.
     *
     * @param self Object instance.
     * @return The first element; or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_array_get_last(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the last element.
     *
     * @param self Object instance.
     * @return The last element; or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_array_get_last_c(const ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Get the last element.
     *
     * @param self Object instance.
     * @return The last element; or NULL if container is empty.
     */
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
void ccntr_man_array_insert(ccntr_man_array_t *self, unsigned index, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Insert a value to the specific position.
     *
     * @param self  Object instance.
     * @param index The value will be inserted to this position of array.
     *              If the index is out of range,
     *              the value will be appended to the end of array.
     * @param value The value to be inserted.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_insert_first(ccntr_man_array_t *self, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Insert a value to the first position.
     *
     * @param self  Object instance.
     * @param value The value to be inserted.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_insert_last(ccntr_man_array_t *self, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Insert a value to the last position.
     *
     * @param self  Object instance.
     * @param value The value to be inserted.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_replace(ccntr_man_array_t *self, unsigned index, void *value)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Place a value to the specific position and replace the old one.
     *
     * @param self  Object instance.
     * @param index The value will be inserted to this position of array.
     *              If the index is out of range,
     *              the value will be appended to the end of array.
     * @param value The value to be inserted.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_erase(ccntr_man_array_t *self, unsigned index)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase value at the specific position.
     *
     * @param self  Object instance.
     * @param index Index of the value.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_erase_first(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase the first value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_erase_last(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase the last value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_clear(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_array_sort(ccntr_man_array_t *self)
{
    /**
     * @memberof ccntr_man_array_t
     * @brief Sort elements.
     *
     * @param self Object instance.
     *
     * @remarks This function will have no effect if
     *          function compare_values is not set.
     */
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_ARRAY_ENABLED
