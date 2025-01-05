#include "CommutatorVigicrues.h"

#include <boost/log/trivial.hpp>

void CommutatorVigicrues::onPropertiesChanged(sdbus::Signal& signal)
{
	std::string interfaceName;
	std::map<std::string, sdbus::Variant> changedProperties;
	std::vector<std::string> invalidatedProperties;

	signal >> interfaceName >> changedProperties >> invalidatedProperties;

	if (!changedProperties.empty()) {
		for (const auto& [propertyName, propertyValue] : changedProperties) {
			std::string valueStr = propertyValue.get<std::string>();
			BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues: " << "New " << propertyName << " value: " << valueStr;

			if (propertyName == "flow_tendency") {
				m_flowTendency = valueStr;
			}
			if (propertyName == "flow_value") {
				m_flowValue = std::stof(valueStr);
			}
			if (propertyName == "water_level_tendency") {
				m_waterLevelTendency = valueStr;
			}
			if (propertyName == "water_level_value") {
				m_waterLevelValue = std::stof(valueStr);
			}
		}

		newData();
	}
}

CommutatorVigicrues::CommutatorVigicrues():
	Commutator("Vigicrues") {
	const char* destinationName = "com.commutator.Vigicrues";
	const char* objectPath = "/com/commutator/Vigicrues";
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

			if (key == "flow_tendency") {
				m_flowTendency = valueStr;
				BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues: " << "Got flow_tendency : " << m_flowTendency;
			}
			if (key == "flow_value") {
				m_flowValue = std::stof(valueStr);
				BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues: " << "Got flow_value : " << m_flowValue;
			}
			if (key == "water_level_tendency") {
				m_waterLevelTendency = valueStr;
				BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues: " << "Got water_level_tendency : " << m_waterLevelTendency;
			}
			if (key == "water_level_value") {
				m_waterLevelValue = std::stof(valueStr);
				BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues: " << "Got water_level_value : " << m_waterLevelValue;
			}
		}
	}
	catch(const sdbus::Error& e) {
		BOOST_LOG_TRIVIAL(error) << "CommutatorVigicrues: " << "Got  error " << e.getName() << " with message " << e.getMessage();
	}

	BOOST_LOG_TRIVIAL(info) << "CommutatorVigicrues created.";
}
