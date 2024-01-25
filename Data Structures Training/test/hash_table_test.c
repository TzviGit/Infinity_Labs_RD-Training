/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz			                         ~
 * Reviewer    ------------ 									 ~
 ~ Description   Hash Table	  Test Module		 			   	 ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 ~ Date          October 25, 2021                                ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
#include <time.h> /*clock_t*/
#include <string.h> /*string funcs like strlen strcmp*/
#include <stdlib.h> /*malloc , free*/
#include "test_util.h"  /* API with util funcs for thistest module*/
#include "hash_table.h" /* API for the hash)map module that we are testing*/


#define BUF_SIZE 30


/***************************************************************************
~
~         ```````````Genaeral Util funcs
~
~*************************************************************************/


static void StripEndLineChar(char *buffer);




size_t ProccessKey(const void *data)
{
    size_t result = 0;

    char *str = (char *)data;
    while ('\0' != *str)
    {
        result += (size_t)*str++;
    }

    return result + (497) +(1000 * (*(char *)data));
}

int MyCompare(const void *d1, const void *d2)
{
    return strcmp((char *)d1, (char *)d2) == 0;;
}


int FreeAllData(void *data, void *param)
{
    (void)param;
    free((char *)data);
    return 0;
}


/***************************************************************************
~
~         Dictionary Tester -- Load the linux dictionary into a hashmap
~
~*************************************************************************/


int CopyFileToHashTable(h_table_t *h_table, char *pool_ptr, char *file_name)
{
    char buffer[BUF_SIZE] = {0};
    FILE *file_ptr = fopen(file_name, "r");

    if (NULL == file_ptr)
    {
        printf("ERROR : could not open file\n\n");
        return 1;
    }

    while (NULL != fgets(buffer, BUF_SIZE, file_ptr))
    {
        StripEndLineChar(buffer);

        HTInsert(h_table, (void *)strcpy(pool_ptr, buffer));

        pool_ptr += strlen(pool_ptr) + 1;

    }


    return fclose(file_ptr);

}




static void StripEndLineChar(char *buffer)
{
    char *ptr = buffer;
    while ('\0' != *ptr)
    {
        if ('\n' == *ptr)
        {
            *ptr = '\0';
            return;
        }
        ++ptr;
    }
}


void TestDictionary(void)
{
    clock_t before = {0};
    clock_t after = {0};

    char word_to_find[BUF_SIZE] = {0};



    h_table_t *dictionary  = HTCreate(1000, ProccessKey, MyCompare);
    char *mem_pool = (char *)malloc(2000000);

        before = clock();
    if (NULL == dictionary || NULL == mem_pool)
    {
        if (mem_pool)
        {
            free(mem_pool); mem_pool = NULL;
        }

        if (dictionary)
        {
            free(dictionary); dictionary = NULL;
        }

        return;
    }

    CopyFileToHashTable(dictionary, mem_pool,  "/usr/share/dict/words");
    printf(_YELLOW_ "size of dict is: %lu  words!!\n\n"_RESET_, HTSize(dictionary));

    after = clock();

    printf("it took %lu  seconds \n\n", after - before);

    scanf("%s", word_to_find);
    while (27 != *word_to_find)
    {
        printf(_CYAN_"Enter a word and press "_PURPLE_ "Enter." \
            "or "_RED_"ESC to exit\n\n"_RESET_);



        if (NULL == HTFind(dictionary, word_to_find))
        {
            printf(""_RED_ "Looks like "_BLUE_ " %s " _RED_
             "doesnt exist in the dictionary."_CYAN_
              "Try Re-spelling it!\n\n\n"_RESET_, word_to_find);
        }
        else
        {
            printf(""_GREEN_  "NICE GEUSS!!" _CYAN_ " %s" _GREEN_
             " Is in the Dictionary....\n\n"_RESET_, word_to_find);
        }

        scanf("%s", word_to_find);
    }

    free(mem_pool); mem_pool = NULL;
    HTDestroy(dictionary);
    dictionary = NULL;

}


/***************************************************************************
~
~                           GENERAL TESTER
~
~*************************************************************************/

int PrintString(void *data, void *param)
{
    (void)param;
    printf("%s -> ",(char *)data );
    return 0;
}


void TestHashMap(void)
{
    RESET_FLAG;
    char data1[] = "Hello World!";
    char data2[] = "Tzvi Moskowitz";
    char data3[] = "BABA";
    char data4[] = "AABB";
    char data5[] = "BBAA";
    char data6[] = "ABAB";

    h_table_t *hash_map = HTCreate(500, ProccessKey,MyCompare);
    VERIFY(HTIsEmpty(hash_map));
    VERIFY(HTSize(hash_map) == 0);
    HTInsert(hash_map, (void *)data1);

    HTInsert(hash_map, (void *)data2);

    HTInsert(hash_map, (void *)data3);


    HTInsert(hash_map, (void *)data4);

    HTInsert(hash_map, (void *)data5);
    VERIFY(data4 == HTFind(hash_map, data4));

    HTInsert(hash_map, (void *)data6);

    VERIFY(!HTIsEmpty(hash_map));
    VERIFY(HTSize(hash_map) == 6);


    HTForEach(hash_map, PrintString, NULL);
    VERIFY(HTRemove(hash_map, data3) == data3);

    VERIFY(HTSize(hash_map) == 5);

    VERIFY(NULL == HTFind(hash_map, data3));
    VERIFY(data4 == HTFind(hash_map, data4));
    HTRemove(hash_map, data4);

    HTRemove(hash_map, data6);

    HTRemove(hash_map, (void *)data5);




    VERIFY(HTSize(hash_map) == 2);

    VERIFY(NULL == HTFind(hash_map, data5));

    VERIFY(data2 == HTFind(hash_map, data2));




    HTDestroy(hash_map);

    CHECK_SUCCESS(HTCreate);

}


int main(void)
{
    TestHashMap();

    TestDictionary();
    return 0;
}
