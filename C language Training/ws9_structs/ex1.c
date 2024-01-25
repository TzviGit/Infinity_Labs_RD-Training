#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strlen strdup etc. */
#include <assert.h>/*assert*/

/*#define APPROVED_BY_FANIA(this) (printf("%s\n", #this will work really well!))*/

#define MAX2(a,b) (((a) > (b)) ? (a) : (b))
#define MAX3(a, b, c) MAX2(MAX2(a, b), (c))
#define SIZE_OF_VAR(var) ((char *)(&var + 1) - (char *)(&var))
#define SIZE_OF_TYPE(type) ((char *)(((type *)1000) + 1) - (char *)(type *)1000)

void AddToStr(void **, int );
void Nothing(void **);
void FreeAllocation(void **);
int CountDigits(int );
void AddToInt(void **, int );
void PrintStr(void *);
void PrintFloat(void *);
void AddToFloat(void **, int );
void PrintInt(void *);
void IntToPointer(void **, int );
void FloatToPointer(void **, float );
char *StrDup(const char *);

 typedef union
{
  int i;
  float f;
  char string[100];
}data_type;

 typedef union
{
    void (*p1)();
    void (*p2)(int);
    void (*p3)(float);
    void (*p4)(char *);
}funcPointer_type;

 typedef struct
{
    data_type var;
    funcPointer_type PrintElem;
    funcPointer_type AddToElem;
}union_element;

typedef struct
{
    void *var;
    void (*Print)(void *);
    void (*Add)(void **, int);
    void (*FreeRes)(void **);
}element;

void FloatToPointer(void **p, float f)
{
    *p = (void *)*((size_t *)&f);
}
void IntToPointer(void **p, int i)
{
    *p = (void *)(long)i;
}
void PrintInt(void *p)
{
    printf("%ld\n",(long)p);
}

void PrintFloat(void *p)
{
    printf("%f\n",*(float *)&p);
}

void PrintStr(void *p)
{
  assert(p);

  printf("%s\n", (char *)p);
}

void AddToFloat(void **p, int a)
{
    float new_f = (*(float *)p + (float)a);
    FloatToPointer(p, new_f);
}

void AddToInt(void **p, int i)
{
    long new_i = (long)*p + i;
    IntToPointer(p, new_i);
}

int CountDigits(int a)
{
    int sign = 1, digits = 0, base = 1;

    if (a <= 0)
    {
        ++digits;
        sign = -1;
        a *= sign;
    }

    while (a / base)
    {
      ++digits;
      base *= 10;
    }
    return digits;
}

void AddToStr(void **p, int num)
{
    char *new_string = NULL;
    char *src = NULL;
    int alloc_length = 0;

    assert(*p);

    alloc_length += strlen(*p) + 1 + CountDigits(num);

    new_string = (char *)malloc(sizeof(*new_string) * alloc_length);
    if (!new_string)
    {
        return;
    }

    src = (char *)*p;
    strcpy(new_string, src);

    sprintf((new_string + strlen(new_string)),"%d",num);

    free(*p);
    *p = new_string;
}
void Nothing(void **p)
{
    (void)p;
}
void FreeAllocation(void **p)
{
    free(*p);
    *p = NULL;
}
int main()
{

    element elem1 = {0} ,elem2 = {0},elem3 = {0};
    element elem_arr[3] = {0};

    int a = 0;
    int i = 0, int_to_add = -206;
    float f = 0;
    void *Ip = (void *)&a, *fp = (void *) &a;
    char *string = StrDup("chapter");

    printf("%ld\n",SIZE_OF_VAR(a));
    printf("%ld\n",SIZE_OF_TYPE(element));

    if (!string)
    {
        return 0;
    }

    IntToPointer(&Ip, a);
    FloatToPointer(&fp, f);


    elem1.var = Ip;
    elem1.Print = PrintInt;
    elem1.Add = AddToInt;
    elem1.FreeRes = Nothing;

    elem2.var = fp;
    elem2.Print = PrintFloat;
    elem2.Add = AddToFloat;
    elem2.FreeRes = Nothing;

    elem3.var = (void *)string;
    elem3.Print = PrintStr;
    elem3.Add = AddToStr;
    elem3.FreeRes = FreeAllocation;

    elem_arr[0] = elem1;
    elem_arr[1] = elem2;
    elem_arr[2] = elem3;

    for (i = 0; i < 3; ++i)
    {
        elem_arr[i].Print(elem_arr[i].var);
        elem_arr[i].Add(&elem_arr[i].var,int_to_add);
        elem_arr[i].Print(elem_arr[i].var);
        elem_arr[i].FreeRes(&elem_arr[i].var);
    }

    return 0;
}

char *StrDup(const char *s)
{
	char *copy = (char *)malloc(sizeof(char) * (strlen(s) + 1));

	return copy ? strcpy(copy, s) : NULL;
}
