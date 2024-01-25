
/**********************************************************
 * 
 * dll_loader.cpp - dll_loader implementation
 * 
 * Author : Tzvi Moskowitz
 *
 * Reviewed and approved by : ???
 * 
 * Date: February 4, 2022
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/


#include <unistd.h> // read
#include <dlfcn.h> //dlsym
#include "dllLoader.hpp" // API for this module
#include <iostream>

namespace ilrd
{

   


    DirMonitor::DirMonitor(const std::string& path_)
    :Dispatcher(),
    m_dir(path_),
    m_listnerThread(boost::thread(boost::bind(&DirMonitor::DirListen, this)))
    {
        m_listnerThread.detach();
        // m_listnerThread = boost::thread(boost::bind(&DirMonitor::DirListen, this));
    }
    
    DirMonitor::~DirMonitor() throw()
    {

    }

    void DllLoader::ReactToEvent(struct inotify_event& event_)
    {
        
        
        if (event_.mask & (IN_CREATE | IN_MOVED_TO) )
        {
            //sleep to give time for event to be finalizes in directory
            usleep(100000);
            typedef char *(*getName_t)(void);
            std::string fileName(event_.name);
            std::string loadPath = m_dirPath + "/" +fileName;
           
            void *handle = dlopen(loadPath.c_str(), RTLD_LAZY );

            getName_t getName = reinterpret_cast<getName_t>(dlsym(handle, "GetFuncName"));
            
            std::string funcName = std::string(getName());

            LoadInfo_t info = {fileName, funcName, handle};
            m_availableFuncs.push_back(info);
        }
        else if (event_.mask & IN_DELETE)
        {
            std::string file(event_.name);
            typename std::vector<LoadInfo_t>::iterator iter = m_availableFuncs.begin();
            for (; iter != m_availableFuncs.end(); ++iter)
            {
                if (iter->fileName == file)
                {
                    m_availableFuncs.erase(iter);
                }
            }
        }
        
    }


///////////////////////////////////////////////////////////////////////////////////////////////////
//          DLL_LOADER IMPLE -----------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

    DllLoader::DllLoader(DirMonitor &dirMonitor_) throw()
    :CallBack(dirMonitor_, boost::bind(&DllLoader::ReactToEvent,this,_1) , nullptr),
    m_availableFuncs(),
    m_dirPath(dirMonitor_.m_dir)
    {
        // empty
    }

    void *DllLoader::LoadFunction(const std::string& funcName_) throw(KeyNotFound)
    {
        typename std::vector<LoadInfo_t>::iterator iter = m_availableFuncs.begin();
        for (; iter != m_availableFuncs.end(); ++iter)
        {
            if (iter->funcName == funcName_)
            {
                return dlsym(iter->handle, funcName_.c_str());
            }
        }
        
        throw KeyNotFound();

        return nullptr;
    }

    void DirMonitor::DirListen()
    {
        const size_t EVENT_SIZE = sizeof(struct inotify_event);
        const size_t EVENT_BUF_LEN = 1024 * (EVENT_SIZE + 16);

        char buffer[EVENT_BUF_LEN] = {0};
        try
        {
            while(true)
            {
                int fd = inotify_init(); //TODO: error_check

                int wd = inotify_add_watch(fd, m_dir.c_str(), IN_ALL_EVENTS|IN_CREATE|IN_DELETE|IN_MODIFY);//TODO: error_check

                ssize_t length = read(fd, buffer, EVENT_BUF_LEN);//TODO: error_check

                ssize_t i = 0;
                while(i < length)
                {
                    struct inotify_event *event = (struct inotify_event *)buffer;

                    Notify(*event);

                    i += EVENT_SIZE + event->len;
                }
            }
        }
        catch(...){}
    }
}