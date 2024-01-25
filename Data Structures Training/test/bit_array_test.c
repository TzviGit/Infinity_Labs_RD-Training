#include "bit_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST2(function,buf1,buf2,arr, choice,result) (TESTFunc2Params((function),(buf1),(buf2),(arr),(choice),(result), #function))
#define TEST1(function,buf1, buf2, arr, result) (TESTFunc1Params((function),(buf1),(buf2),(arr),(result), #function))


void TESTFunc1Params(bit_arr_t (*fp)(bit_arr_t),char *,char *,  bit_arr_t , bit_arr_t , char *);
void TESTFunc2Params(bit_arr_t (*fp)(bit_arr_t, size_t),char *, char *, bit_arr_t ,size_t , bit_arr_t , char *);
void TestToString();
void TestSetVal(bit_arr_t, size_t, bit_arr_t);
void TestMirror(bit_arr_t  );
void TestGetBit(bit_arr_t , size_t );
void TestSetBit(bit_arr_t , size_t , size_t );

int Atoi( const char *, unsigned int );
char *Itoa(size_t , char *, int  );

void TESTFunc2Params(bit_arr_t (*fp)(bit_arr_t, size_t),char *buf1,char *buf2, bit_arr_t arr,size_t choice, bit_arr_t result, char *fun_name)
{
    if (strcmp(ToString(fp(arr, choice),buf1),ToString(result,buf2)))
    {
        printf("%s  FAILED!! \n\n", fun_name );
    }
    else
    {

        printf("%s  PASSED!! \n\n", fun_name );
    }

}


void TESTFunc1Params(bit_arr_t (*fp)(bit_arr_t), char *buf1, char *buf2, bit_arr_t arr, bit_arr_t result, char *fun_name)
{
    if (0 != strcmp(ToString(fp(arr),buf1),ToString(result, buf2)))
    {
        printf("%s  FAILED!! \n\n", fun_name );
    }
    else
    {
        printf("%s  PASSED!! \n\n", fun_name );
    }

}

void TestSetBit(bit_arr_t arr, size_t index, size_t val)
{
    if ((val << index) == (SetBit(arr, index, val) & ((size_t)(1 << index))))
    {
        printf("SETBIT  PASSED !!\n\n");
    }
    else
    {
        printf("SETBIT  FAILED !!!\n\n");
    }
}

void TestGetBit(bit_arr_t arr, size_t index)
{
    if ((size_t)(GetBit(arr, index) << index) == ((1 << index) & arr))
    {
        printf("GetBit PASSED !!\n\n");
    }
    else
    {
        printf("GetBit  FAILED ! TEST\n\n");
    }

}

void TestMirror(bit_arr_t arr)
{
    size_t maskR1 = 0x55,maskL1 = 0xAA00000000000000, maskR2 = 0xff000000, maskL2 = 0x000000ff00000000;
    if (((arr & maskR1) == (Mirror(arr) & maskL1)) && ((arr & maskR2) == (Mirror(arr) & maskL2)) && (Mirror(Mirror(arr)) == arr))
    {
        printf("MIRROR PASSED!!\n\n");
    }
    else
    {
        printf("MIRROR FAILED!!\n\n");

    }
}


void TestToString()
{
    bit_arr_t arr = (bit_arr_t)~12345678;
    int bool1 = 0, bool2 = 0, bool3 = 0;
    char *buf1 = (char *)malloc(73);
    char *buf2 = (char *)malloc(73);
    if (!buf1){return;}
    if (!buf2){return;}
    buf2= Itoa(arr,buf2, 2);
    bool1 = strcmp(ToString(arr,buf1), ToString(Atoi(buf2,2),buf2));
    arr = (bit_arr_t)0;
    buf2 = Itoa(arr,buf2, 2);
    bool2 = strcmp(ToString(arr,buf1), ToString(Atoi(buf2,2),buf2));
    arr = (bit_arr_t)~45345345;
    buf2 = Itoa(arr,buf2, 2);
    bool3 = strcmp(ToString(arr,buf1), ToString(Atoi(buf2,2),buf2));

    if (bool1 || bool2 ||bool3)
    {
        printf("ToString Failed\n\n");

    }
    else
    {
        printf("ToString WORKS!!\n\n");
    }
    free(buf1); free(buf2);
    buf1 = NULL;
    buf2 = NULL;
}

int main()
{
    char *buf1 = calloc(1,200);
    char *buf2 = calloc(1,200);


    TestToString();

    TEST2(SetOn,buf1,buf2,16,2,20);
    TEST2(SetOn,buf1,buf2,0,6,64);
    TEST2(FlipBit,buf1,buf2,65,1,67);
    TEST2(SetOff,buf1,buf2,18,1,16);
    TEST2(RotL,buf1,buf2,16,1,8);
    TEST2(RotR,buf1,buf2,15,67,120);
    TEST1(CountOn,buf1,buf2,32,1);
    TEST1(CountOff,buf1,buf2,15,64 -4);
    TestMirror(0xcc0000 );
    TEST1(SetAll,buf1,buf2,453454554,~0);
    TEST1(ResetAll,buf1,buf2,44554,0);
    TestSetBit(128, 2,1);
    TestGetBit(31,7);

    return 0;
}
