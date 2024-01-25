/*---------------------------------c file--------------*/
/********************************************************
 * uid.c - UID implementation
 *
 * Author :
 * Reviewed and approved by :
 *
 *
 * 14/09/2021
 * OLRD 110 Ramat Gan
 *
 * *********************************************************/



/*=================================h file-------------*/
/********************************************************
 *
 * uid.h - UID API
 *
 * Author : OLRD 110 Narnia (johnny ? fani : shahar)    XDDDDD
 * Reviewed and approved by : Binyamin Haymann
 *
 *
 * 14/09/2021
 * OLRD 110 Ramat Gan
 *
 *
 * *********************************************************/
#ifndef __UID_OL_110_H__
#define __UID_OL_110_H__

#include <unistd.h> /* pid_t, getpid */
#include <sys/types.h>
#include <time.h> /* time_t, time */


typedef struct uid
{
    size_t __counter__;
    pid_t __process_id__;
    time_t __time_ind__;
}uuid_t;

extern const uuid_t BAD_UID;

uuid_t GetUID(void);

int IsEqualUID(uuid_t uid1, uuid_t uid2);


#endif /* __UID_OL_110_H__ */
