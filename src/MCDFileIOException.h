#ifndef MCD_MCDFILENOTFOUNDEXCEPTION_H
#define MCD_MCDFILENOTFOUNDEXCEPTION_H

#include <stdexcept>

namespace mcd {

    class MCDFileIOException : public std::runtime_error {

        using std::runtime_error::runtime_error;

    };

}

#endif //MCD_MCDFILENOTFOUNDEXCEPTION_H
