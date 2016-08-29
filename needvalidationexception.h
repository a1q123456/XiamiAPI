#ifndef NEEDVALIDATIONEXCEPTION_H
#define NEEDVALIDATIONEXCEPTION_H

#include <exception>
#include <string>

namespace xiamiapi
{

    class NeedValidationException : public std::exception
    {
    public:
        const char * what() const noexcept;
        NeedValidationException() = default;
    };


}


#endif // NEEDVALIDATIONEXCEPTION_H
