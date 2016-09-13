//
// Created by jack on 16-9-13.
//

#include "keynotfoundexception.h"

const char * xiamiapi::KeyNotFountException::what() const noexcept
{
    return "key not found";
}
