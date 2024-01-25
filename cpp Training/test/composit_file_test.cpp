
/********************************************************
 * 
 * Tester for the Tree direstory printer
 * 
 * Author : Tzvi Moskowitz
 * 
 * version: 1.0.0
 * 
 * Reviewed and approved by : TODO
 * 
 * 28-12-21
 * 
 * OLRD 110 Ramat Gan
 * 
 * *********************************************************/
#include <cstdio>
#include "file_component.hpp"
#include "regular_file.hpp"
#include "directory.hpp"

using namespace ilrd;
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Directory dir(".");
        dir.Print();
    }
    else
    {
        Directory dir(argv[1]);
        dir.Print();
    }

    return 0;
}
