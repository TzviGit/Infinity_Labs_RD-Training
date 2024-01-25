/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Version       1.0.0                                           ~
 *                                                               *
 ~ Author        OL110 Students                                  ~
 * Reviewer      Yinon Yishay                                    *
 ~ Description   Calculator - Header_File   				 ~
 * Group         OL110                                           *
 ~ Company       ILRD Ramat Gan                                  ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */


#ifndef __CALCULATOR_OL_110_H__
#define __CALCULATOR_OL_110_H__
#include "stack.h"

typedef struct calculator calc_t;

typedef enum
{
    CALC_SUCCESS = 0,
    CALC_MATH_ERROR = 1,
    CALC_ALLOC_ERROR = 2,
    CALC_SYNTAX_ERROR = 3,
    CALC_DONE
} calc_status_t;



calc_t *CalcCreate(void);

void CalcDestroy(calc_t *calc);

calc_status_t Calculate(const char *expression, double *result, calc_t *calc);

#endif /*__CALCULATOR_OL_110_H__*/
