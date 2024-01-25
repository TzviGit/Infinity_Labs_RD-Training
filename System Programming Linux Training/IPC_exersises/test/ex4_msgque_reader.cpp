/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Tzvi Moskowitz                                      ~
 * Reviewer      --------                                            *
 ~ Description   Using Message Queue             ~
 * Group         OL110                                               *
 ~ Company       ILRD Ramat Gan                                      ~
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <sys/types.h> // key_t
#include <sys/ipc.h> // ftok()
#include <sys/msg.h> // msgget(), msgrcv()
#include <fcntl.h> // S_IRWXU  permission modes
#include <iostream> // std::cout, std::endl
#include <cstring> // strcmp



static const size_t MAX_MSG_SIZE = 19;
static const size_t PROJECT_ID = 99;
static void ReadFromMsgQueue(int queueId);

struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAX_MSG_SIZE];
};

int main()
{
    key_t uniqueKey = ftok("ipc_exercise", PROJECT_ID);

    int queueId = msgget(uniqueKey, S_IRWXU | IPC_CREAT);
    ReadFromMsgQueue(queueId);

    return 0;
}

static void ReadFromMsgQueue(int queueId)
{
    
    for (int i = 1; i <= 10; ++i)
    {
        mesg_buffer msg = {0, 0}; 
    
        msgrcv(queueId, &msg, MAX_MSG_SIZE, 1, MSG_NOERROR);

        std::cout << msg.mesg_text << std::endl;

        if (!strcmp(msg.mesg_text, "end"))
        {
        msgctl(queueId, IPC_RMID, NULL);
        }
    }
}
