//
// Created by jack on 16-8-15.
//

#include "GenericArray.h"

IUnknown * xiamiapi::GenericArray::get_element(unsigned long index)
{
    return reinterpret_cast<IUnknown *>(data + (sizeof_element * index));
}

unsigned long xiamiapi::GenericArray::length()
{
    return m_length;
}

void xiamiapi::GenericArray::Release()
{
    for (unsigned long i = 0; i < length(); i++)
    {
        auto tmp = get_element(i);
        tmp->~IUnknown();
    }
    delete[] data;
    delete this;
}