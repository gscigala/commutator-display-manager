#include "CommutatorSytadin.h"

#include <boost/log/trivial.hpp>

void CommutatorSytadin::onPropertiesChanged(sdbus::Signal& signal)
{
	std::string interfaceName;
	std::map<std::string, sdbus::Variant> changedProperties;
	std::vector<std::string> invalidatedProperties;

	signal >> interfaceName >> changedProperties >> invalidatedProperties;

	if (!changedProperties.empty()) {
		for (const auto& [propertyName, propertyValue] : changedProperties) {
			std::string valueStr = propertyValue.get<std::string>();
			BOOST_LOG_TRIVIAL(info) << "CommutatorSytadin: " << "New " << propertyName << " value: " << valueStr;

			if (propertyName == "traffic_level") {
				m_trafficLevel = valueStr;
			}
			if (propertyName == "traffic_tendency") {
				m_trafficTendency = valueStr;
			}
			if (propertyName == "traffic_value") {
				m_trafficValue = std::stoi(valueStr);
			}
		}

		newData();
	}
}

CommutatorSytadin::CommutatorSytadin():
	Commutator("Sytadin") {
	const char* destinationName = "com.commutator.Sytadin";
	const char* objectPath = "/com/commutator/Sytadin";
	m_proxy = sdbus::createProxy(destinationName, objectPath);

	const char* interfaceName = "org.freedesktop.DBus.Properties";
	m_proxy->registerSignalHandler(interfaceName, "PropertiesChanged",
				       [this](sdbus::Signal& signal) {
					       this->onPropertiesChanged(signal);
				       });
	m_proxy->finishRegistration();

	auto method = m_proxy->createMethodCall(interfaceName, "GetAll");
	method << "";
	try {
		auto reply = m_proxy->callMethod(method);
		std::map<std::string, sdbus::Variant> properties;
		reply >> properties;
		for (const auto& [key, value] : properties) {
			std::string valueStr;
			valueStr = value.get<std::string>();

			if (key == "traffic_level") {
				m_trafficLevel = valueStr;
				BOOST_LOG_TRIVIAL(info) << "CommutatorSytadin: " << "Got trafficLevel : " << m_trafficLevel;
			}
			if (key == "traffic_tendency") {
				m_trafficTendency = valueStr;
				BOOST_LOG_TRIVIAL(info) << "CommutatorSytadin: " << "Got trafficTendency : " << m_trafficTendency;
			}
			if (key == "traffic_value") {
				m_trafficValue = std::stoi(valueStr);
				BOOST_LOG_TRIVIAL(info) << "CommutatorSytadin: " << "Got trafficValue : " << m_trafficValue;
			}
		}
	}
	catch(const sdbus::Error& e) {
		BOOST_LOG_TRIVIAL(error) << "CommutatorSytadin: " << "Got  error " << e.getName() << " with message " << e.getMessage();
	}

	BOOST_LOG_TRIVIAL(info) << "CommutatorSytadin: " << "CommutatorSytadin created.";
}
