#ifndef COMMUTATORIDFMLINEREPORTS_H
#define COMMUTATORIDFMLINEREPORTS_H

#include "Commutator.h"
#include "IdfmLineStatus.h"
#include "IdfmTransportMode.h"

#include <sdbus-c++/sdbus-c++.h>
#include <boost/signals2.hpp>

class CommutatorIdfmLineReports: public Commutator {
public:
	CommutatorIdfmLineReports();

	std::string getLineSeverityEffect(IdfmTransportMode mode, std::string name) const;

	boost::signals2::signal<void(IdfmTransportMode, std::string)> newData;

private:
        IdfmTransportMode getModeFromString(std::string str);
	std::string getLineFromString(IdfmTransportMode mode, std::string interfaceName);
	void onPropertiesChanged(sdbus::Signal& signal);

	std::map<std::unique_ptr<IdfmLineStatus>,
		 std::unique_ptr<sdbus::IProxy>> m_linesMap;
};

#endif // COMMUTATORIDFMLINEREPORTS_H
