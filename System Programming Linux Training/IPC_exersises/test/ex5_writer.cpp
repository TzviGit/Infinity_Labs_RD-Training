#include "shared_mem.hpp" //api for the SHaredMem class

int main()
{
    SharedMem writer("/tmp/ipc_ping_pong_exe5_ftok", 200);

    writer.ShrMemWriter("ping");

    return 0;
}