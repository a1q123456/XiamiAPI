//
// Created by jack on 16-9-13.
//

#ifndef XIAMIAPI_KEYNOTFOUNDEXCEPTION_H
#define XIAMIAPI_KEYNOTFOUNDEXCEPTION_H


#include <exception>

namespace xiamiapi
{
    class KeyNotFountException : public std::exception
    {
        virtual const char *what() const noexcept;
    };
}

#endif //XIAMIAPI_KEYNOTFOUNDEXCEPTION_H
