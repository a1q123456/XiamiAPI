//
// Created by jack on 16-8-15.
//

#include <cstring>
#include "str.h"

xiamiapi::Str::Str(const std::string &other)
{
    data = new char[other.size()];
    memcpy(data, other.c_str(), other.size());
}

uint64_t xiamiapi::Str::length()
{
    return m_length;
}

const char *xiamiapi::Str::c_str()
{
    return data;
}

uint64_t xiamiapi::Str::Release()
{
    if (--m_Ref == 0)
    {
        delete[] data;
        delete this;
        return 0;
    }
    return 1;
}

HRESULT xiamiapi::Str::QueryInterface(RIID riid, void** ppv)
{
    if (ppv == nullptr)
    {
        return E_INVALIDARG;
    }
    if (riid == IID_IUnknown || riid == __uuidof(Str))
    {
        *ppv = (IUnknown *) this;
        AddRef();
        return S_OK;
    }
    *ppv = nullptr;
    return E_NOINTERFACE;
}

