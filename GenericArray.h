//
// Created by jack on 16-8-15.
//

#ifndef XIAMIAPI_GENERICARRAY_H
#define XIAMIAPI_GENERICARRAY_H
#include "xiamiapi.h"
#include <vector>
#include <iostream>
#include "iunknown.h"

namespace xiamiapi
{
    class GenericArray : public IGenericArray
    {
    private:
        char* data;
        unsigned long m_length;
        unsigned long sizeof_element;
    public:
        template <class T>
        GenericArray(const std::vector<T> &other)
        {

            sizeof_element = sizeof(T);
            data = new char[sizeof_element * other.size()];

            for (int i = 0; i < other.size(); i++)
            {
                new (data + (i * sizeof_element)) T(other[i]);
            }
            m_length = other.size();
        }

        GenericArray(const GenericArray &other) = delete;

        GenericArray& operator=(GenericArray arg) = delete;

        GenericArray& operator=(const GenericArray& other) = delete;

        GenericArray& operator=(GenericArray&& other) = delete;

        virtual IUnknown * get_element(unsigned long index);
        virtual unsigned long length();
        virtual void Release();
    };
}

#endif //XIAMIAPI_GENERICARRAY_H
