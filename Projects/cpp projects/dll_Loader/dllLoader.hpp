/**********************************************************
 * 
 * DllLoader.hpp - DllLoader API
 * 
 * Author : OLRD 110
 * 
 * version : 0.1.0
 *
 * Reviewed and approved by : Yinon
 * 
 * 03-02-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#ifndef __ILRD_RD110_DLLLOADER_HPP__
#define __ILRD_RD110_DLLLOADER_HPP__

#include <sys/inotify.h>  //for the inotify struct and funcs
#include <boost/function.hpp> //boost::function
#include <boost/thread.hpp> //thread
#include <vector> //vector

#include "dispatcher.hpp"   // Dispatcher & CallBack classes

namespace ilrd
{

typedef std::pair<std::string, void *> loadPair_t;

class DirMonitor : public Dispatcher<struct inotify_event&>
{
public:
    explicit DirMonitor(const std::string &path);
    virtual ~DirMonitor() throw();

private:
    boost::thread m_listnerThread;
    void DirListen();
    std::string m_dir;
    friend class DllLoader;
};

class DllLoader : public CallBack<struct inotify_event&>
{
public:
    
    DllLoader(DirMonitor& dirMonitor)throw();
    virtual ~DllLoader()throw(){} //?
    void *LoadFunction(const std::string &funcName_) throw(KeyNotFound);

private:
    typedef struct 
    {
        std::string fileName;
        std::string funcName;
        void *handle;
    }LoadInfo_t;

    std::vector<LoadInfo_t> m_availableFuncs;
    void ReactToEvent(struct inotify_event &event_);
    std::string m_dirPath;
};

} // namespace ilrd

#endif // __ILRD_RD110_DLLLOADER_HPP__

//user must apply function GetFunctionName()
//void *dlopen(const char *filename, int flags);


//   struct inotify_event {
//               int      wd;       /* Watch descriptor */
//               uint32_t mask;     /* Mask describing event */
//               uint32_t cookie;   /* Unique cookie associating related
//                                      events (for rename(2)) */
//               uint32_t len;      /* Size of name field */
//               char     name[];   /* Optional null-terminated name */
//           };