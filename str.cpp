//
// Created by jack on 16-8-15.
//

#include "str.h"

xiamiapi::Str::Str(const std::string &other)
{
    data = new char[other.size()];
    memcpy(data, other.c_str(), other.size());
}

unsigned long xiamiapi::Str::length()
{
    return m_length;
}

const char *xiamiapi::Str::c_str()
{
    return data;
}

void xiamiapi::Str::Release()
{
    delete[] data;
    delete this;
}

