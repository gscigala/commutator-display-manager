#include "IdfmTransportMode.h"

std::string idfmTransportModeToString(IdfmTransportMode mode) {

	std::string str;

	switch(mode) {
	case (IdfmTransportMode::METRO): {
		str = "METRO";
		break;
	}
	case (IdfmTransportMode::RAPID_TRANSIT): {
		str= "RAPID_TRANSIT";
		break;
	}
	case (IdfmTransportMode::LOCAL_TRAIN): {
		str = "LOCAL_TRAIN";
		break;
	}
	default: {
		str = "UNKNOWN";
		break;
	}
	}

	return str;
}
