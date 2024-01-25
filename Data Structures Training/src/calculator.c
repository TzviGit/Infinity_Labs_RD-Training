/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      -----------
* Description   Calculator built on a state machine design.
* Date          Oct 17, 2021
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdlib.h> /*malloc,  strtod*/
#include <assert.h> /*asserrt*/
#include <string.h> /*stlen*/
#include <math.h> /*power*/
#include "calculator.h" /*API FOR THIS C FILE*/
#include "stack.h" /*API for stack*/

#define DUMMY (double)492349324
#define LUT_ROW_LENGTH 256
typedef void (*transition_func_t)(calc_t *calc);

typedef enum
{
    WATING_FOR_OPERAND,
    WAITING_FOR_OPERATOR,
    NUM_OF_STATES
} state_t;

typedef enum
{
    OPEN_PAREN = 0,
    CLOSE_PAREN,
    MULTIPLY,
    ADD,
    SUBTRACT = 5,
    DIVIDE = 7,
    NULL_TERMINATOR = 18,
    EXPONENT = 54,
    OPR_TABLE_LEN
}operate_t;

struct calculator
{
    char *expression_ptr;
    stack_t *operators;
    stack_t *operands;
    state_t state;
    calc_status_t status;
    transition_func_t operations_LUT[OPR_TABLE_LEN][OPR_TABLE_LEN];
    transition_func_t transition_LUT[NUM_OF_STATES][256];

};


static void StackDoublePop(stack_t *stack);
static void StackDoublePush(stack_t *stack, double d);
static void StackCharPush(stack_t *stack, char ch);
static double StackDoublePeekAndPop(stack_t *stack);
static char StackCharPeek(stack_t *stack);

static void FillTransitionLUT(transition_func_t transition_LUT[][256]);
static void FillOperationsLUT(transition_func_t operation_LUT[][OPR_TABLE_LEN]);

static void FinishParenthesis(calc_t *calc);
static void PushParenthesis(calc_t *calc);
static void ProccessOperator(calc_t *calc);
static void UpdateSyntaxError(calc_t *calc);
static void ProccessOperand(calc_t *calc);

static void PushOperator(calc_t *calc);
static void IncrementExpressionPointer(calc_t *calc);
static void UpdateDone(calc_t *calc);
static void AddTop(calc_t *calc);
static void SubtractTop(calc_t *calc);
static void MultiplyTop(calc_t *calc);
static void DivideTop(calc_t *calc);
static void ExponentiateTop(calc_t *calc);
static void ProccessNullTerminator(calc_t *calc);


calc_t *CalcCreate(void)
{
    calc_t *calc = (calc_t *)malloc(sizeof(*calc));
    if (!calc)
    {
        return NULL;
    }

    calc->state = WATING_FOR_OPERAND;
    calc->status = CALC_SUCCESS;
    calc->expression_ptr = NULL;
    calc->operators = NULL;
    calc->operands = NULL;

    FillTransitionLUT(calc->transition_LUT);
    FillOperationsLUT(calc->operations_LUT);

    return calc;
}


static void FillTransitionLUT(transition_func_t function_LUT[][256])
{
    size_t i = 0, j = 0;

    for (i = 0; i < NUM_OF_STATES; ++i)
    {
        for (j = 0; j < LUT_ROW_LENGTH; ++j)
        {
            function_LUT[i][j] = UpdateSyntaxError;
        }
    }

    function_LUT[WATING_FOR_OPERAND][' '] = IncrementExpressionPointer;
    function_LUT[WATING_FOR_OPERAND]['('] = PushParenthesis;
    function_LUT[WATING_FOR_OPERAND][')'] = ProccessOperator;


    function_LUT[WATING_FOR_OPERAND]['-'] = ProccessOperand;
    function_LUT[WATING_FOR_OPERAND]['+'] = ProccessOperand;

    for (i = '0'; i <= '9'; ++i)
    {
        function_LUT[WATING_FOR_OPERAND][i] = ProccessOperand;
    }

    function_LUT[WAITING_FOR_OPERATOR][' '] = IncrementExpressionPointer;
    function_LUT[WAITING_FOR_OPERATOR][0] = ProccessNullTerminator;
    function_LUT[WAITING_FOR_OPERATOR]['-'] = ProccessOperator;
    function_LUT[WAITING_FOR_OPERATOR]['+'] = ProccessOperator;
    function_LUT[WAITING_FOR_OPERATOR]['*'] = ProccessOperator;
    function_LUT[WAITING_FOR_OPERATOR]['/'] = ProccessOperator;

    function_LUT[WAITING_FOR_OPERATOR]['^'] = ProccessOperator;
    function_LUT[WAITING_FOR_OPERATOR][')'] = ProccessOperator;

}


static void FillOperationsLUT(transition_func_t operation_LUT[][OPR_TABLE_LEN])
{
    size_t i = 0;

    for (i = 0; i < OPR_TABLE_LEN; ++i)
    {
        operation_LUT[NULL_TERMINATOR][i] = PushOperator;
        operation_LUT[i][OPEN_PAREN] = PushOperator;
        operation_LUT[OPEN_PAREN][i] = PushOperator;
        operation_LUT[i][EXPONENT]= PushOperator;
    }



    operation_LUT[NULL_TERMINATOR][NULL_TERMINATOR] = UpdateDone;
    operation_LUT[CLOSE_PAREN][NULL_TERMINATOR] = UpdateSyntaxError;
    operation_LUT[CLOSE_PAREN][CLOSE_PAREN] = UpdateSyntaxError;
    operation_LUT[OPEN_PAREN][CLOSE_PAREN] = FinishParenthesis;


    /****NOW FOR MULTIPLY ON STACK*****/


    for (i = 1; i <= NULL_TERMINATOR; ++i)
    {
        operation_LUT[MULTIPLY][i] = MultiplyTop;
    }

    /****NOW FOR ADDITION ON STACK*****/


    operation_LUT[ADD][MULTIPLY] = PushOperator;
    operation_LUT[ADD][DIVIDE] = PushOperator;

    operation_LUT[ADD][NULL_TERMINATOR] = AddTop;
    operation_LUT[ADD][CLOSE_PAREN] = AddTop;
    operation_LUT[ADD][ADD] = AddTop;
    operation_LUT[ADD][SUBTRACT] = AddTop;

    /****NOW FOR SUBTRACTION ON STACK*****/

    operation_LUT[SUBTRACT][MULTIPLY] = PushOperator;
    operation_LUT[SUBTRACT][DIVIDE] = PushOperator;

    operation_LUT[SUBTRACT][NULL_TERMINATOR] = SubtractTop;
    operation_LUT[SUBTRACT][CLOSE_PAREN] = SubtractTop;
    operation_LUT[SUBTRACT][ADD] = SubtractTop;
    operation_LUT[SUBTRACT][SUBTRACT] = SubtractTop;


    /****NOW FOR DIVISION*****/


    for (i = 1; i <= NULL_TERMINATOR; ++i)
    {
        operation_LUT[DIVIDE][i] = DivideTop;
    }


    /****NOW FOR EXPONENTIATION ON STACK*****/

    for (i = 1; i <= NULL_TERMINATOR; ++i)
    {
        operation_LUT[EXPONENT][i] = ExponentiateTop;
    }




}


void CalcDestroy(calc_t *calc)
{
    if (calc->operands)
    {
        StackDestroy(calc->operands);
        calc->operands = NULL;
    }

    if (calc->operators)
    {
        StackDestroy(calc->operators);
        calc->operators = NULL;
    }

    calc->expression_ptr = NULL;

    free(calc);
    calc = NULL;
}


calc_status_t Calculate(const char *expression, double *result, calc_t *calc)
{
    size_t expr_len = 0;
    assert(calc);
    assert(expression);
    assert(result);

    expr_len = strlen(expression);

    calc->expression_ptr = (char *)expression;

    calc->operands = StackCreate(1 + expr_len);
    if (!calc->operands)
    {
        return CALC_ALLOC_ERROR;
    }

    calc->operators = StackCreate(1 + expr_len);
    if (!calc->operators)
    {
        StackDestroy(calc->operands); calc->operands = NULL;
        return CALC_ALLOC_ERROR;
    }

    /*push DUMMY on operands stack to avoid undefined behavior*/
    StackDoublePush(calc->operands, DUMMY);
    StackCharPush(calc->operators, ':');

    calc->status = CALC_SUCCESS;
    calc->state = WATING_FOR_OPERAND;

    while (CALC_SUCCESS == calc->status)
    {
        calc->transition_LUT[calc->state][(int)*calc->expression_ptr](calc);
    }

    calc->status = calc->status * (CALC_DONE != calc->status);

    *result = StackDoublePeekAndPop(calc->operands);

    StackDestroy(calc->operands); calc->operands = NULL;
    StackDestroy(calc->operators); calc->operators = NULL;

    return calc->status;



}


static void ProccessOperator(calc_t *calc)
{
    size_t opr_on_stack_ind = StackCharPeek(calc->operators) - '(';
    size_t opr_recieved_ind = *calc->expression_ptr - '(';

    calc->operations_LUT[opr_on_stack_ind][opr_recieved_ind](calc);
}

static void UpdateDone(calc_t *calc)
{
    calc->status = CALC_DONE;
}


static void UpdateSyntaxError(calc_t *calc)
{
    calc->status = CALC_SYNTAX_ERROR;
}

static void ProccessOperand(calc_t *calc)
{
    char *temp = calc->expression_ptr;
    double operand = strtod(calc->expression_ptr, (char **)&calc->expression_ptr);

    calc->status = (CALC_SYNTAX_ERROR * (temp == calc->expression_ptr));


    StackDoublePush(calc->operands, operand);


    calc->state = WAITING_FOR_OPERATOR;

}


static void PushOperator(calc_t *calc)
{
    StackCharPush(calc->operators, *calc->expression_ptr);

    ++calc->expression_ptr;
    calc->state = WATING_FOR_OPERAND;
}


static void PushParenthesis(calc_t *calc)
{
    StackCharPush(calc->operators, '(');

    ++calc->expression_ptr;
}

static void ProccessNullTerminator(calc_t *calc)
{
    size_t opr_on_stack_ind = StackCharPeek(calc->operators) - '(';

    calc->operations_LUT[opr_on_stack_ind][NULL_TERMINATOR](calc);
}

static void FinishParenthesis(calc_t *calc)
{
    StackPop(calc->operators);

    ++calc->expression_ptr;
}

static void AddTop(calc_t *calc)
{
    double operand2 = StackDoublePeekAndPop(calc->operands);
    double operand1 = StackDoublePeekAndPop(calc->operands);

    StackPop(calc->operators);

    StackDoublePush(calc->operands, operand1 + operand2);

}

static void SubtractTop(calc_t *calc)
{
    double operand2 = StackDoublePeekAndPop(calc->operands);
    double operand1 = StackDoublePeekAndPop(calc->operands);

    StackPop(calc->operators);

    StackDoublePush(calc->operands, operand1 - operand2);

}

static void MultiplyTop(calc_t *calc)
{
    double operand2 = StackDoublePeekAndPop(calc->operands);
    double operand1 = StackDoublePeekAndPop(calc->operands);

    StackPop(calc->operators);

    StackDoublePush(calc->operands, operand1 * operand2);

}

static void DivideTop(calc_t *calc)
{
    double operand2 = StackDoublePeekAndPop(calc->operands);
    double operand1 = StackDoublePeekAndPop(calc->operands);

    StackPop(calc->operators);

    calc->status = (CALC_MATH_ERROR * (0 == operand2));

    operand2 += (0 == operand2)? 1 : 0;

    StackDoublePush(calc->operands, operand1 / operand2);

}

static void ExponentiateTop(calc_t *calc)
{
    double operand2 = StackDoublePeekAndPop(calc->operands);
    double operand1 = StackDoublePeekAndPop(calc->operands);

    StackPop(calc->operators);

    StackDoublePush(calc->operands, pow(operand1, operand2));

}

static void IncrementExpressionPointer(calc_t *calc)
{
    ++calc->expression_ptr;
}








static void StackDoublePush(stack_t *stack, double d)
{
    int type = sizeof(double) / sizeof(void *);

    switch(type)
    {
        case 1 :
                StackPush(stack, *(void **)&d);
                break;

        case 2 :
            {
                int arr[2] = {0};
                size_t i = 0;
                *(double *)arr = d;

                for (i = 0; i < 2; ++i)
                {
                    StackPush(stack, (void *)(long)arr[i]);
                }

                break;
            }
    }

}

static double StackDoublePeek(stack_t *stack)
{
    int type = sizeof(double) / sizeof(void *);

    switch(type)
    {
        case 1 :
                {
                    void *temp = StackPeek(stack);
                    return *(double *)&temp;
                }
        case 2 :
                {
                    int arr[2]= {0};
                    long i = 0;

                    for (i = 1; i >= 0; --i)
                    {
                        void *temp = StackPeek(stack);
                        arr[i] = *(int *)&temp;
                        StackPop(stack);
                    }

                    StackDoublePush(stack, *(double *)arr);

                    return *(double *)arr;
                }
        default :
            return 0;
    }
}

static double StackDoublePeekAndPop(stack_t *stack)
{
    double d = StackDoublePeek(stack);

    StackDoublePop(stack);

    return d;
}

static void StackDoublePop(stack_t *stack)
{
    int type = sizeof(double) / sizeof(void *);
    size_t i = 0;

    for (i = 0; i < (size_t)type; ++i)
    {
        StackPop(stack);
    }
}


static void StackCharPush(stack_t *stack, char ch)
{
    StackPush(stack, *(void **)&ch);
}

/*static double StackDoublePeekAndPop(stack_t *stack)
{
    void *temp = StackPeek(stack);
    double d = *(double *)&temp;
    StackPop(stack);
    return d;
}
*/
static char StackCharPeek(stack_t *stack)
{
    void *temp = StackPeek(stack);
    char ch = *(char *)&temp;

    return ch;
}
/*
static void Stack32BitDoublePush(stack_t *stack, double d)
{
    int rounds = sizeof(double) / sizeof(void *);
    size_t i = 0;

    char arr[sizeof(double)] = {0};
    *(double *)arr = d;

    for (i = 0; i < rounds; ++i)
    {
        StackPush(stack, *(int *)arr);
        ++(*(int**)&arr);
    }
}

static double Stack32BitPeekAndPop(stack_t *stack)
{
    int rounds = sizeof(double) / sizeof(void *);
    size_t i = 0;

    char arr[sizeof(double)] = {0};
    int *ptr = arr + sizeof(double);

    for (i = 0; i < rounds; ++i)
    {
        --ptr;
        void *temp = StackPeek(stack);
        *ptr = *(int *)&temp;
        StackPop(stack);
    }

    return *(double *)ptr;
}
*/
