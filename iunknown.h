//
// Created by jack on 16-8-5.
//

#ifndef XIAMIAPI_IUNKNOWN_H
#define XIAMIAPI_IUNKNOWN_H

struct IUnknown
{
    virtual void Release() = 0;
    virtual ~IUnknown() = 0;
};

typedef unsigned long HRESULT;
enum {S_OK = 0, FAIL = 2001};

#endif //XIAMIAPI_IUNKNOWN_H
