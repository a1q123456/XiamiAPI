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
        uint64_t  m_length;
        uint64_t  sizeof_element;
        RIID riid;
    public:
        template <class T>
        GenericArray(const std::vector<T> &other, RIID riid): riid(riid)
        {

            sizeof_element = sizeof(T);
            data = new char[sizeof_element * other.size()];

            for (int64_t i = 0; i < other.size(); i++)
            {
                new (data + (i * sizeof_element)) T(other[i]);
            }
            m_length = other.size();
        }

        GenericArray(const GenericArray &other) = delete;

        GenericArray& operator=(GenericArray arg) = delete;

        GenericArray& operator=(const GenericArray& other) = delete;

        GenericArray& operator=(GenericArray&& other) = delete;

        virtual IUnknown * get_element(uint64_t  index);
        virtual uint64_t length();
        virtual uint64_t Release();
        virtual RIID ElementIID();

        virtual HRESULT QueryInterface(RIID riid, void** ppv);
    };
}

#endif //XIAMIAPI_GENERICARRAY_H
