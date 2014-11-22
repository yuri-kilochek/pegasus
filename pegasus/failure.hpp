#ifndef PEGASUS_HEADER_FAILURE
#define PEGASUS_HEADER_FAILURE

#include <exception>

namespace pegasus {
    struct failure
        : std::exception
    {
        char const* what() const noexcept override {
            return "::pegasus::failure";
        }
    };
}

#endif
