//
// Created by jack on 16-8-13.
//

#include "xiamiapi.h"
#include "xiamiapi_impl.h"

using std::string;

namespace xiamiapi
{
    IXiamiAPI * xiamiapi::IXiamiAPI::CreateInstance()
    {
        return new xiamiapi::XiamiAPI();
    }
}

