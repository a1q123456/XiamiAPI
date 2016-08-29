#include "needvalidationexception.h"
#include <cpr/cpr.h>

namespace xiamiapi
{

    const char * NeedValidationException::what() const noexcept
    {
        static const char * ret = "Need Validation";
        return ret;
    }
}
