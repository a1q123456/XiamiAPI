#include "networkexception.h"

namespace xiamiapi
{
NetworkException::NetworkException(const int & code, const std::string & reason)
{
    this->reason = "network exception code: " + std::to_string(code) + ", reason: " + reason;
}

const char * NetworkException::what() const noexcept
{
    return reason.c_str();
}

}
