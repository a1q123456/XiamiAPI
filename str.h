//
// Created by jack on 16-8-15.
//

#ifndef XIAMIAPI_STR_H
#define XIAMIAPI_STR_H
#include "xiamiapi.h"

namespace xiamiapi
{
class Str: public IStr
{
private:
    char * data;
    unsigned long m_length;
public:
    Str(const std::string &other);

    virtual unsigned long length();
    virtual const char * c_str();

    Str(const Str &other) = delete;

    Str& operator=(Str arg) = delete;

    Str& operator=(const Str& other) = delete;

    Str& operator=(Str&& other) = delete;

    virtual void Release();
};
}

#endif //XIAMIAPI_STR_H