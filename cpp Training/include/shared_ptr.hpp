/**********************************************************
 * 
 * shared_ptr.hpp - shared_ptr API
 * 
 * Author : OLRD 110
 * 
 * version : 0.1.0
 *
 * Reviewed and approved by : Yinon
 * 
 * 12-01-22
 * 
 * OLRD 110 Ramat Gan
 * 
 * *******************************************************/

#ifndef __ILRD_RD110_SHARED_POINTER_HPP__
#define __ILRD_RD110_SHARED_POINTER_HPP__

#include <cstddef>      // size_t
#include <stdexcept>    // std::bad_alloc
#include <iostream>

#include "util.hpp"     // nullptr macro

namespace ilrd
{


    
template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *ptr_ = nullptr) throw(std::bad_alloc);
    ~SharedPtr() throw();

    SharedPtr(const SharedPtr& other_) throw();
    
    // template <typename D>
    // SharedPtr(const SharedPtr<D>& other_) throw();

    template <typename D>
    operator SharedPtr<D>() const throw(); //DYNAMIC_CAST<>

    SharedPtr& operator=(const SharedPtr& rhs_) throw();
    
    // template <typename D>
    // SharedPtr<T>& operator=(const SharedPtr<D>& other_) throw();
    
    T& operator*() const throw();
    T* operator->() const throw();
    

private:
    struct SharedPtrData
    {
        SharedPtrData(T *ptr)
        : m_ptr(ptr), m_counter(1)
        {
            // empty
        }


        T *m_ptr;
        size_t m_counter;
    };


    SharedPtrData *m_data;


    void ProperlyDetachData()
    {
        size_t count = --(m_data->m_counter);
        if (0 == count)
        {
            delete (m_data->m_ptr);
            delete(m_data);
        }

        m_data = nullptr;
    }

    //this is to prevent creation on heap
    void *operator new(size_t);
    void *operator new[](size_t);
};


template<typename T>
SharedPtr<T>::SharedPtr(T *ptr) throw(std::bad_alloc)
:m_data(new SharedPtr::SharedPtrData(ptr))
{
   // std::cout << "NOW IN CONSTRUCTOR\n";
   // std::cout << "reference count is now :" << m_data->m_counter << " \n";
    //empty
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other_)throw()
:m_data(other_.m_data)
{
    ++(m_data->m_counter);
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other_)throw()
{
    // if (m_data != other_.m_data)
    // {
    //     ProperlyDetachData();

    //     m_data = other_.m_data;
    //     ++(m_data->m_counter);
    // }

    SharedPtrData *temp = other_.m_data;
    ++other_.m_data->m_counter;
    ProperlyDetachData();
    m_data = temp;

    return *this;
}

template<typename T>
SharedPtr<T>::~SharedPtr()throw()
{
   // std::cout << "NOW IN CONSTRUCTOR\n";
    ProperlyDetachData();
}

template <typename T>
template <typename D>
SharedPtr<T>::operator SharedPtr<D>() const throw()
{
    //this method will force compilaton error on 
    // any non-valid cast
    D *D_Pointer = (m_data->m_ptr);
    (void)D_Pointer;

    return *reinterpret_cast<const SharedPtr<D> *>(this);
}

template<typename T>
T& SharedPtr<T>::operator*() const throw()
{
    return *(m_data->m_ptr);
}

template<typename T>
T* SharedPtr<T>::operator->() const throw()
{
    return (m_data->m_ptr);
}

} // namespace ilrd;

#endif // __ILRD_RD110_SHARED_POINTER_HPP__

