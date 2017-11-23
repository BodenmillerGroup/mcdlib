#ifndef MCD_MCDFILEMALFORMEDEXCEPTION_H
#define MCD_MCDFILEMALFORMEDEXCEPTION_H

#include <stdexcept>

namespace mcd {

    class MCDFileMalformedException : public std::runtime_error {

        using std::runtime_error::runtime_error;

    };

}

#endif //MCD_MCDFILEMALFORMEDEXCEPTION_H
