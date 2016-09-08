//
// Created by jack on 16-9-8.
//

#include "iunknown.h"

const char * __uuidof_impl(const char * classname)
{
    return classname;
}

HRESULT IUnknown::QueryInterface(RIID riid, void **ppv)
{
    if (ppv == nullptr)
    {
        return E_INVALIDARG;
    }
    if (riid == IID_IUnknown)
    {
        *ppv = (IUnknown *) this;
        AddRef();
        return S_OK;
    }
    *ppv = nullptr;
    return E_NOINTERFACE;
}

unsigned long IUnknown::AddRef()
{
    ++m_Ref;
    return 0;
}

unsigned long IUnknown::Release()
{
    if (--m_Ref == 0)
    {
        delete this;
        return 0;
    }
    return 1;
}

const char *IUnknown::IID()
{
    return "00000-0000-0000-0000";
}
