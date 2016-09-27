//
// Created by jack on 16-8-5.
//

#ifndef XIAMIAPI_IUNKNOWN_H
#define XIAMIAPI_IUNKNOWN_H

#include <cstdint>

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
    virtual uint64_t Release() = 0;
    virtual HRESULT QueryInterface(RIID riid, void ** ppv) = 0;
    virtual uint64_t AddRef() = 0;
    virtual ~IUnknown() = 0;
};



#endif //XIAMIAPI_IUNKNOWN_H
