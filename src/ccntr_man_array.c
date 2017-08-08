#include <assert.h>
#include "abort_message.h"
#include "ccntr_man_array.h"

#ifdef CCNTR_MAN_ARRAY_ENABLED

//------------------------------------------------------------------------------
static
void release_value_default(void *value)
{
    // Nothing to do.
}
//------------------------------------------------------------------------------
void ccntr_man_array_init(ccntr_man_array_t                *self,
                          ccntr_man_array_compare_values_t  compare,
                          ccntr_man_array_release_value_t   release_value)
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
    self->elements = malloc(1);
    if( !self->elements )
        abort_message("ERROR: Cannot allocate more memory!\n");

    self->capacity = 0;
    self->count    = 0;

    self->compare       = compare;
    self->release_value = release_value ? release_value : release_value_default;

    ccntr_spinlock_init(&self->lock);
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
    ccntr_man_array_clear(self);
    free(self->elements);
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
    ccntr_spinlock_lock( (ccntr_spinlock_t*) &self->lock );
    unsigned count = self->count;
    ccntr_spinlock_unlock( (ccntr_spinlock_t*) &self->lock );

    return count;
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
    ccntr_spinlock_lock(&self->lock);
    void *value = self->count ? self->elements[0] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
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
    return ccntr_man_array_get_first((ccntr_man_array_t*)self);
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
    ccntr_spinlock_lock(&self->lock);
    void *value = self->count ? self->elements[ self->count - 1 ] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
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
    return ccntr_man_array_get_last((ccntr_man_array_t*)self);
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
    ccntr_spinlock_lock(&self->lock);
    void *value = index < self->count ? self->elements[index] : NULL;
    ccntr_spinlock_unlock(&self->lock);

    return value;
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
    return ccntr_man_array_get((ccntr_man_array_t*)self, index);
}
//------------------------------------------------------------------------------
static
void extend_array_buffer(ccntr_man_array_t *self)
{
    unsigned newcap = self->capacity ? self->capacity << 1 : 1;
    assert( newcap > self->capacity );

    size_t newsize = newcap * sizeof(self->elements[0]);
    assert( newsize > newcap );

    void **newbuf = realloc(self->elements, newsize);
    if( !newbuf )
        abort_message("ERROR: Cannot allocate more memory!\n");

    self->elements = newbuf;
    self->capacity = newcap;
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
    ccntr_spinlock_lock(&self->lock);

    index = ( index <= self->count )?( index ):( self->count );

    if( self->count == self->capacity )
        extend_array_buffer(self);

    for(unsigned i = self->count; i > index; --i)
        self->elements[i] = self->elements[i-1];

    self->elements[index] = value;

    ++ self->count;

    ccntr_spinlock_unlock(&self->lock);
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
    ccntr_man_array_insert(self, 0, value);
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
    ccntr_man_array_insert(self, self->count, value);
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
    ccntr_spinlock_lock(&self->lock);

    if( index < self->count )
    {
        void *oldvalue = self->elements[index];
        self->elements[index] = value;

        self->release_value(oldvalue);
    }
    else
    {
        if( self->count == self->capacity )
            extend_array_buffer(self);

        self->elements[ self->count ++ ] = value;
    }

    ccntr_spinlock_unlock(&self->lock);
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
    ccntr_spinlock_lock(&self->lock);

    if( index < self->count )
    {
        self->release_value(self->elements[index]);

        for(unsigned i = index + 1; i < self->count; ++i)
            self->elements[i-1] = self->elements[i];

        -- self->count;
    }

    ccntr_spinlock_unlock(&self->lock);
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
    ccntr_man_array_erase(self, 0);
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
    if( self->count )
        ccntr_man_array_erase(self, self->count - 1);
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
    ccntr_spinlock_lock(&self->lock);

    for(unsigned i = 0; i < self->count; ++i)
    {
        void *value = self->elements[i];
        self->release_value(value);
    }

    self->count = 0;

    ccntr_spinlock_unlock(&self->lock);
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
    ccntr_spinlock_lock(&self->lock);

    if( self->compare )
    {
        qsort(self->elements,
              self->count,
              sizeof(self->elements[0]),
              (int(*)(const void*,const void*)) self->compare);
    }

    ccntr_spinlock_unlock(&self->lock);
}
//------------------------------------------------------------------------------

#endif  // CCNTR_MAN_ARRAY_ENABLED
