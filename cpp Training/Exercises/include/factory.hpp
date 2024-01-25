/*  ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~
~ Version       0.0.1                                           ~
* Date          18/01/2022                                      *
~ Author        RD110                                           ~
* Reviewer      Binyamin                                        *
~ Description   Factory - API                                   ~
* Group         R&D 110                                         *
~ Company       ILRD Ramat Gan                                  ~
* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#ifndef __ILRD_RD110_FACTORY_HPP__
#define __ILRD_RD110_FACTORY_HPP__


#include <boost/unordered_map.hpp> // boost::unordered_map
#include "singleton_strange.hpp" //singleton API , includes <boost/core/noncopyable.hpp> // boost::noncopyable
#include "shared_ptr.hpp"

namespace ilrd
{

class InvalidKey_Exception : std::exception
{
public:
    const char * what() const throw ()
    {
        return "Invalid Key";
    }
};

// the right CREATOR create the right object and return pointer to base, that is PRODUCT
template <typename PRODUCT, typename KEY, typename PARAM, typename CREATOR = PRODUCT(*)(PARAM)> 

class Factory: private boost::noncopyable
{
public:
    void Add(KEY key_, CREATOR func_) throw(std::bad_alloc, InvalidKey_Exception); // map entry can throw
    PRODUCT Create(KEY Key_, PARAM param_) const throw(std::bad_alloc, std::out_of_range); // PRODUCT can be a pointer
    void Remove(KEY key_) throw(std::bad_alloc);
private:
    boost::unordered_map<KEY, CREATOR> m_map;
    Factory() throw(std::bad_alloc);


    friend Factory* Singleton<Factory>::GetInstance() throw(std::bad_alloc);
};

template <typename PRODUCT, typename KEY, typename PARAM, typename CREATOR>
Factory<PRODUCT, KEY,PARAM,CREATOR>::Factory() throw(std::bad_alloc)
{

}

template <typename PRODUCT, typename KEY, typename PARAM, typename CREATOR>
void Factory<PRODUCT, KEY,PARAM,CREATOR>::Add(KEY key_, CREATOR func_) throw(std::bad_alloc,InvalidKey_Exception)
{
    if (m_map.count(key_) > 0)
    {
        throw InvalidKey_Exception();
    }
    m_map[key_]=  func_;
}

template <typename PRODUCT, typename KEY, typename PARAM, typename CREATOR>
void Factory<PRODUCT, KEY,PARAM,CREATOR>::Remove(KEY key_) throw(std::bad_alloc)
{
    m_map.erase(key_);
}

template <typename PRODUCT, typename KEY, typename PARAM, typename CREATOR>
PRODUCT Factory<PRODUCT, KEY,PARAM,CREATOR>::Create(KEY key_, PARAM param_) const
                                         throw(std::bad_alloc, std::out_of_range)
{
     const CREATOR &creator = (m_map.at(key_));
   return (creator(param_));
}


} // ILRD

#endif // __ILRD_RD110_FACTORY_HPP__
