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
