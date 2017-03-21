#include "container_of.h"
#include "abort_message.h"
#include "ccntr_man_map.h"

#warning Temporary hide!
//#ifdef CCNTR_MAN_MAP_ENABLED

//------------------------------------------------------------------------------
//---- Iterator ----------------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_map_iter_init(ccntr_man_map_iter_t   *self,
                             struct ccntr_man_map_t *container,
                             ccntr_map_node_t       *node)
{
}
//------------------------------------------------------------------------------
bool ccntr_man_map_iter_have_value(const ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_iter_move_prev(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_iter_move_next(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_map_iter_get_key(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Get key.
     *
     * @param self Object instance.
     * @return The key be pointed by the iterator.
     *
     * @attention Do NOT modify the key directly, except
     *            the key (and value) has already be popped from the container.
     *            If you want to modify a key in the container,
     *            you will be need to erase or pop it, then modify it, then insert again.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_map_iter_get_value(ccntr_man_map_iter_t *self)
{
    /**
     * @memberof ccntr_man_map_iter_t
     * @brief Get value.
     *
     * @param self Object instance.
     * @return The value be pointed by the iterator.
     */
}
//------------------------------------------------------------------------------
//---- Constant Iterator -------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_map_citer_init(ccntr_man_map_citer_t        *self,
                              const struct ccntr_man_map_t *container,
                              const ccntr_map_node_t       *node)
{
}
//------------------------------------------------------------------------------
bool ccntr_man_map_citer_have_value(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Check if have a valid value.
     *
     * @param self Object instance.
     * @return TRUE if it have a value; and FALSE if not.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_citer_move_prev(ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Move iterator to the previous value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_citer_move_next(ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Move iterator to the next value.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_map_citer_get_key(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Get key.
     *
     * @param self Object instance.
     * @return The key be pointed by the iterator.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_map_citer_get_value(const ccntr_man_map_citer_t *self)
{
    /**
     * @memberof ccntr_man_map_citer_t
     * @brief Get value.
     *
     * @param self Object instance.
     * @return The value be pointed by the iterator.
     */
}
//------------------------------------------------------------------------------
//---- Key Map Container -------------------------------------------------------
//------------------------------------------------------------------------------
void ccntr_man_map_init(ccntr_man_map_t              *self,
                        ccntr_map_compare_keys_t      compare,
                        ccntr_man_map_release_key_t   release_key,
                        ccntr_man_map_release_value_t release_value)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Constructor.
     *
     * @param self          Object instance.
     * @param compare       A function to be used to compare keys.
     *                      If this parameter is NULL, then
     *                      all keys will be treated as integral values.
     * @param release_key   Callback to release contained keys,
     *                      and can be NULL to do nothing.
     * @param release_value Callback to release contained values,
     *                      and can be NULL to do nothing.
     *
     * @remarks Object must be initialised (and once only) before using.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_destroy(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Destructor.
     *
     * @param self Object instance.
     *
     * @remarks Object must be destructed to finish using,
     *          and must not make any operation to the object after it be destructed.
     */
}
//------------------------------------------------------------------------------
unsigned ccntr_man_map_get_count(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get count of values it contained.
     *
     * @param self Object instance.
     * @return The count of values.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_iter_t ccntr_man_map_get_first(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_citer_t ccntr_man_map_get_first_c(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the first value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the first value,
     *         or an empty iterator if no any values contained.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_iter_t ccntr_man_map_get_last(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_citer_t ccntr_man_map_get_last_c(const ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Get the last value.
     *
     * @param self Object instance.
     * @return An iterator be pointed to the last value,
     *         or an empty iterator if no any values contained.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_iter_t ccntr_man_map_find(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return An iterator be pointed to the value if found;
     *         or an empty iterator if not found.
     */
}
//------------------------------------------------------------------------------
ccntr_man_map_citer_t ccntr_man_map_find_c(const ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return An iterator be pointed to the value if found;
     *         or an empty iterator if not found.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_map_find_value(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return The value if found; or NULL if not found.
     */
}
//------------------------------------------------------------------------------
const void* ccntr_man_map_find_value_c(const ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Find value by key.
     *
     * @param self Object instance.
     * @param key  The key to be used to serch for the value.
     * @return The value if found; or NULL if not found.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_insert(ccntr_man_map_t *self, void *key, void *value)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Insert a value.
     *
     * @param self  Object instance.
     * @param key   Key of the value to be inserted.
     * @param value The value to be inserted.
     *
     * @remarks If the container already have a value with the same key, then
     *          the old value (and key) will be replaced by the new one.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_erase(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase value.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_erase_by_key(ccntr_man_map_t *self, const void *key)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase value.
     *
     * @param self Object instance.
     * @param key  Key of the value.
     */
}
//------------------------------------------------------------------------------
void ccntr_man_map_clear(ccntr_man_map_t *self)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Erase all values it contained.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void* ccntr_man_map_pop(ccntr_man_map_t *self, ccntr_man_map_iter_t *pos)
{
    /**
     * @memberof ccntr_man_map_t
     * @brief Pop value from container.
     * @details Similarly to ccntr_man_map_t::ccntr_man_map_erase,
     *          but just remove the value and key from the container,
     *          and will not release them.
     *
     * @param self Object instance.
     * @param pos  Position of the value.
     * @return The value be removed from container.
     */
}
//------------------------------------------------------------------------------

#warning Temporary hide!
//#endif  // CCNTR_MAN_MAP_ENABLED
