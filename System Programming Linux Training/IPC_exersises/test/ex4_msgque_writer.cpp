/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz                                      ~
 * Reviewer      --------                                            *
 ~ Description   Using Message Queue             ~
 * Group         OL110                                               *
 ~ Company       ILRD Ramat Gan                                      ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <sys/types.h> // key_t
#include <sys/ipc.h> // ftok()
#include <sys/msg.h> // msgget(), msgsnd()
#include <fcntl.h> // S_IRWXU
#include <stdio.h> // sprintf();
#include <cstdlib> //system 



static const size_t MAX_MSG_SIZE = 19;
static const size_t PROJECT_ID = 99;
static void WriteToMsgQueue(int queueId);

struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAX_MSG_SIZE];
};


int main()
{
    system("touch ipc_exercise");
    key_t uniqueKey = ftok("ipc_exercise", PROJECT_ID);

    int queueId = msgget(uniqueKey, S_IRWXU | IPC_CREAT);

    WriteToMsgQueue(queueId);
    return 0;
}

static void WriteToMsgQueue(int queueId)
{
   
    mesg_buffer msg = {0, 0};
    for (int i = 1; i <= 3; ++i)
    {
        msg.mesg_type = 1;
        sprintf(msg.mesg_text, "message number - %d", i);

        msgsnd(queueId, &msg, MAX_MSG_SIZE, 0);
    }

    msg.mesg_type = 1;
    sprintf(msg.mesg_text, "end");

    msgsnd(queueId, &msg, 4, 0);
}
