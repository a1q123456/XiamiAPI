//
// Created by jack on 16-8-15.
//

#include "GenericArray.h"

IUnknown * xiamiapi::GenericArray::get_element(uint64_t index)
{
    return reinterpret_cast<IUnknown *>(data + (sizeof_element * index));
}

uint64_t xiamiapi::GenericArray::length()
{
    return m_length;
}

uint64_t xiamiapi::GenericArray::Release()
{
    if (--m_Ref == 0)
    {
        for (uint64_t i = 0; i < length(); i++)
        {
            auto tmp = get_element(i);
            tmp->~IUnknown();
        }
        delete[] data;
        delete this;
        return 0;
    }
    return 1;
}

HRESULT xiamiapi::GenericArray::QueryInterface(RIID riid, void **ppv)
{
    if (ppv == nullptr)
    {
        return E_INVALIDARG;
    }
    if (riid == IID_IUnknown || riid == __uuidof(IGenericArray))
    {
        *ppv = (IUnknown *) this;
        AddRef();
        return S_OK;
    }
    *ppv = nullptr;
    return E_NOINTERFACE;
}

RIID xiamiapi::GenericArray::ElementIID()
{
    return riid;
}

uint64_t xiamiapi::GenericArray::AddRef()
{
    ++m_Ref;
    return 0;
}
