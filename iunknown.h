//
// Created by jack on 16-8-5.
//

#ifndef XIAMIAPI_IUNKNOWN_H
#define XIAMIAPI_IUNKNOWN_H

typedef unsigned long HRESULT;
typedef const char * RIID;
enum {S_OK = 0, FAIL = 2001, E_INVALIDARG = 1001, E_NOINTERFACE = 1002};
template<class T>
const char * __uuidof_impl()
{
    return T::IID();
}

#define __uuidof(classname) __uuidof_impl<classname>()

#define IID_IUnknown __uuidof(IUnknown)

struct IUnknown
{
    static const char * IID();
    int64_t m_Ref = 1;
    virtual unsigned long Release() = 0;
    virtual HRESULT QueryInterface(RIID riid, void ** ppv) = 0;
    virtual unsigned long AddRef();
};



#endif //XIAMIAPI_IUNKNOWN_H
