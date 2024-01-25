#include "shared_mem.hpp" //api for the SHaredMem class

int main()
{
    SharedMem reader("/tmp/ipc_ping_pong_exe5_ftok", 200);

    reader.ShrMemReader();

    reader.ShrMemRemove();

    return 0;
}