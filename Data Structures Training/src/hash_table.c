/********************************************************
 *
 * hash_t.c - hash table implementation
 *
 * Author : Tzvi Moskowitz
 *
 * Reviewed and approved by : ~~~~~~~~
 *
 * Date:  October 25, 2021
 *
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/


#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc,free*/
#include "hash_table.h" /* API for this module*/
#include "s_list.h" /*api for singly linked list we use in our Implementation*/




/*typedef struct hashable_object
{
    char *key;
    void *data;
}hashable_t;*/

struct h_table
{
    size_t hash_entries;
    slist_t **table;
    hash_function_t hash_function;
    find_func_t compare_func;
    size_t current_size;
};


static void DestroyAllEntrieLists(h_table_t *h_table);
static void *RemoveAndGetData(slist_t *list, const void *data, find_func_t compare);
static void InitializeTableEntries(h_table_t **h_table);
static void FreeHashMapStruct(h_table_t **h_table);
/*static void InitializeHashmapStructFields(h_table_t *h_table,
    size_t hash_entries,slist_t **table,hash_function_t hash_function,
                                                    find_func_t compare);*/


#define FOR_ALL_ENTRIES_IN_TABLE(h_table) for (i = 0; i < (h_table)->hash_entries; ++i)

#define INITIALIZE_HASHMAP_STRUCT_FIELDS(h_table) {h_table->hash_entries = hash_entries;\
h_table->table = table_ptr;\
h_table->hash_function = hash_function;\
h_table->compare_func = compare;\
h_table->current_size = 0;}

 /*size_t ProccessKey(const hashable_t *data,size_t map_size)
{
    size_t result = 0;
    char *key_ptr  = data->key;
    while(NULL != key_ptr)
    {
        result += (int)*key_ptr;
    }

    return result % map_size;
}
*/

h_table_t *HTCreate(size_t hash_entries, hash_function_t hash_function,
                                                        find_func_t compare)
{
    h_table_t *h_table = NULL;
    slist_t **table_ptr = NULL;

    assert (hash_function);
    assert (compare);

    h_table = (h_table_t *)malloc(sizeof(*h_table)
                                + (sizeof(slist_t *) * hash_entries));
    if (NULL == h_table)
    {
        return NULL;
    }

    table_ptr = (slist_t **)((h_table_t *)h_table + 1);

    INITIALIZE_HASHMAP_STRUCT_FIELDS(h_table);
    /*InitializeHashmapStructFields(h_table, hash_entries,  table_ptr,
                                                hash_function,  compare);
*/
    InitializeTableEntries(&h_table);

    return h_table;
}

static void InitializeTableEntries(h_table_t **h_table)
{
    size_t i = 0;

    for (i = 0; i < (*h_table)->hash_entries; ++i)
    {
        slist_t *temp = SLCreate((*h_table)->compare_func);

        if (NULL == temp)
        {
            size_t j = 0;
            for (j =0 ;j < i; ++j)
            {
                SLDestroy((*h_table)->table[j]);
                (*h_table)->table[j] = NULL;
            }

            FreeHashMapStruct(h_table);

            return;
        }

        (*h_table)->table[i] = temp;
    }

}

/*static void InitializeHashmapStructFields(h_table_t *h_table,
    size_t hash_entries,slist_t **table,hash_function_t hash_function,
    find_func_t compare)
{
    h_table->hash_entries = hash_entries;
    h_table->table = table;
    h_table->hash_function = hash_function;
    h_table->compare_func = compare;
    h_table->current_size = 0;
}*/

void HTDestroy(h_table_t *h_table)
{
    DestroyAllEntrieLists(h_table);

    FreeHashMapStruct(&h_table);
    free(h_table);  h_table = NULL;
}

static void DestroyAllEntrieLists(h_table_t *h_table)
{
    size_t i = 0;

    for (i = 0; i < h_table->hash_entries; ++i)
    {
        SLDestroy(h_table->table[i]);
    }
}


static void FreeHashMapStruct(h_table_t **h_table)
{
    (*h_table)->table = NULL;
    (*h_table)->hash_function = NULL;
    (*h_table)->compare_func = NULL;

    free(*h_table); *h_table = NULL;
}


int HTInsert(h_table_t *h_table, void *data)
{
    size_t index = 0;
    slist_t *list = NULL;

    assert (h_table);
    assert (data);

    index = h_table->hash_function(data) % h_table->hash_entries;
    list = h_table->table[index];

    if (SLIterIsEqual(SLEnd(list), SLAddBefore(SLEnd(list), data)))
    {
        return 1;
    }

    ++(h_table->current_size);

    return 0;
}

void *HTRemove(h_table_t *h_table, const void *data)
{
    size_t index = 0;
    void *taken_data = NULL;

     assert (h_table);

     index = h_table->hash_function(data) % h_table->hash_entries;
     taken_data = RemoveAndGetData(h_table->table[index], data, h_table->compare_func);

     h_table->current_size -= (NULL == taken_data) ? 0 : 1;

    return taken_data;
}


static void *RemoveAndGetData(slist_t *list, const void *data, find_func_t compare)
{
    sl_iter_t iter = 0;
    void *taken_data = NULL;
    int flag = 0;

    assert (list);
    assert (data);

    iter = SLFind(list, data, compare);
    flag = SLIterIsEqual(iter, SLEnd(list));
    taken_data = flag ? NULL : SLGetValue(iter);

    if (!flag)
    {
        SLRemove( iter);
    }

    return taken_data;
}


void *HTFind(const h_table_t *h_table, const void *data)
{
    size_t index = 0;
    void *found_data = NULL;

    assert (h_table);
    assert (data);

    index = h_table->hash_function(data) % h_table->hash_entries;
    found_data = RemoveAndGetData(h_table->table[index], data, h_table->compare_func);

    if (NULL != found_data)
    {
        SLAddBefore(SLBegin(h_table->table[index]), (void *)data);
    }

    return found_data;
}


int HTIsEmpty(const h_table_t *h_table)
{
    assert (h_table);

    return 0 == h_table->current_size;
}


size_t HTSize(const h_table_t *h_table)
{
    assert (h_table);

    return h_table->current_size;
}


int HTForEach(h_table_t *h_table, action_func_t action, void *param)
{
    size_t i  = 0;
    int flag = 0;

    assert (h_table);
    assert (action);

    for (i = 0; i < (h_table)->hash_entries; ++i)
    {
        slist_t *slist = h_table->table[i];

        flag = SLForEach(SLBegin(slist), SLEnd(slist), action, param);

        if (0 != flag)
        {
            break;
        }
    }

    return flag;
}
