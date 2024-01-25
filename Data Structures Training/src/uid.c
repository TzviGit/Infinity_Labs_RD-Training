/******************************************************************************
* Author        Tzvi Moskowitz
* Reviewer      ---
* Description   UID module
* Group         OL110
* Company       ILRD Ramat Gan
* ****************************************************************************/

#include <stdatomic.h> /* atomic variables */
#include "uid.h" /* API for this module */


const uuid_t BAD_UID = {0};


uuid_t GetUID()
{
    static atomic_size_t counter = 0;
    uuid_t uid = BAD_UID;
    time_t seconds = time(NULL);

    if ((time_t)-1 != seconds)
    {
        uid.__counter__ = ++counter;
        uid.__process_id__ = getpid();
        uid.__time_ind__ = seconds;
    }

    return uid;
}


int IsEqualUID(uuid_t uid1, uuid_t uid2)
{
    return (uid1.__time_ind__ == uid2.__time_ind__ &&
     uid1.__counter__ == uid2.__counter__   &&
     uid1.__process_id__ == uid2.__process_id__);
}
