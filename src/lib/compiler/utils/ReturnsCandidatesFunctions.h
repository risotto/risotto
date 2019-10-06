//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_RETURNSCANDIDATESFUNCTIONS_H
#define RISOTTOV2_RETURNSCANDIDATESFUNCTIONS_H

#include <stdexcept>
#include "lib/compiler/FunctionsTable.h"

class ReturnsCandidatesFunctions {
public:
    bool forceReturnFunctionReference = false;

    virtual std::string getCandidatesFunctionsFor();
};


#endif //RISOTTOV2_RETURNSCANDIDATESFUNCTIONS_H
