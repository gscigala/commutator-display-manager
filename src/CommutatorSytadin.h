#ifndef COMMUTATORSYTADIN_H
#define COMMUTATORSYTADIN_H

#include "Commutator.h"

#include <sdbus-c++/sdbus-c++.h>
#include <boost/signals2.hpp>

class CommutatorSytadin: public Commutator {
public:

	CommutatorSytadin();

	std::string getTrafficLevel() const {
		return m_trafficLevel;
	}

	std::string getTrafficTendency() const {
		return m_trafficTendency;
	}

	int getTrafficValue() const {
		return m_trafficValue;
	}

	boost::signals2::signal<void()> newData;

private:
	void onPropertiesChanged(sdbus::Signal& signal);

	std::shared_ptr<sdbus::IProxy> m_proxy;

	std::string m_trafficLevel;
	std::string m_trafficTendency;
	int m_trafficValue;
};

#endif // COMMUTATORSYTADIN_H
