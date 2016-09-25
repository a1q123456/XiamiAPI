//
// Created by jack on 16-8-15.
//

#ifndef XIAMIAPI_STR_H
#define XIAMIAPI_STR_H
#include "xiamiapi.h"
#include <string>

namespace xiamiapi
{
class Str: public IStr
{
private:
    char * data;
    uint64_t m_length;

    int64_t m_Ref = 1;
public:
    Str(const std::string &other);

    virtual uint64_t length();
    virtual const char * c_str();

    Str(const Str &other) = delete;

    Str& operator=(Str arg) = delete;

    Str& operator=(const Str& other) = delete;

    Str& operator=(Str&& other) = delete;

    virtual uint64_t AddRef();

    virtual uint64_t Release();

    virtual HRESULT QueryInterface(RIID riid, void** ppv);


};
}

#endif //XIAMIAPI_STR_H
