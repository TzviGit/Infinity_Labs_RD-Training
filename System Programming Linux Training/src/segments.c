static int FuncOne(int *a)
{
    static double One_d1 = 3.3;
    static double One_d2;
    double One_d3 =3.22, One_d4;

    char *FuncOne_cp1 = "TZVI";
    char *FuncOne_cp2 = "MOSKOWITZ";
    return *(int *)&One_d1;
}

 int FuncTwo(int *a)
{
    const static double Two_d1 = 3.3;
    static double Two_d2;
    double Two_d3 =3.22, Two_d4;

    char *FuncTwo_cp1 = "HELLO";
    char *FuncTwo_cp2 = "WORLD";
    return *(int *)&Two_d1;
}

static char static_ch1 = 'a', static_ch2;

char global_ch1 = 'A', global_ch2;

char *cp1 = "INFINITY";
char *cp2 = "LABS";
