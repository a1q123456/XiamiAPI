#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <string>

namespace xiamiapi
{
class NetworkException: public std::exception
{
public:
    NetworkException()=default;
    NetworkException(const int & code, const std::string & reason);
    const char * what() const noexcept;
private:
    std::string reason;
};
}



#endif // NETWORKEXCEPTION_H
