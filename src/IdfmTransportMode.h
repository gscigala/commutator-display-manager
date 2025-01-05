#ifndef IDFMTRANSPORTMODE_H
#define IDFMTRANSPORTMODE_H

#include <string>

enum class IdfmTransportMode {
    METRO,
    RAPID_TRANSIT,
    LOCAL_TRAIN,
    UNKNOWN
};

std::string idfmTransportModeToString(IdfmTransportMode mode);

#endif // IDFMTRANSPORTMODE_H
