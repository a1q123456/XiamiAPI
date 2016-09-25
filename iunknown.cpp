//
// Created by jack on 16-9-8.
//

#include <cstdint>
#include "iunknown.h"

const char * __uuidof_impl(const char * classname)
{
    return classname;
}

const char *IUnknown::IID()
{
    return "00000-0000-0000-0000";
}

IUnknown::~IUnknown()
{

}