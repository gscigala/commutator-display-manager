#include "CommutatorIdfmLineReports.h"

#include <algorithm>

#include <pugixml.hpp>
#include <boost/log/trivial.hpp>

IdfmTransportMode CommutatorIdfmLineReports::getModeFromString(std::string str)
{
	if (str.find("Metro") != std::string::npos) {
		return IdfmTransportMode::METRO;
	} else if (str.find("LocalTrain") != std::string::npos) {
		return IdfmTransportMode::LOCAL_TRAIN;
	} else if (
		str.find("RapidTransit") != std::string::npos) {
		return IdfmTransportMode::RAPID_TRANSIT;
	} else {
		BOOST_LOG_TRIVIAL(error) << "CommutatorIdfmLineReports: Wrong transport type!";
		return IdfmTransportMode::UNKNOWN;
	}
}

std::string CommutatorIdfmLineReports::getLineFromString(IdfmTransportMode mode, std::string str)
{
	std::string modeStr;
	std::string line;

	if (mode == IdfmTransportMode::METRO)
		modeStr = "Metro";
	if (mode == IdfmTransportMode::LOCAL_TRAIN)
		modeStr = "LocalTrain";
	if (mode == IdfmTransportMode::RAPID_TRANSIT)
		modeStr = "RapidTransit";

	size_t pos = str.find(modeStr);
	if (pos != std::string::npos) {
		line = str.substr(pos + modeStr.length());
	}

	return line;
}

void CommutatorIdfmLineReports::onPropertiesChanged(sdbus::Signal& signal)
{
	std::string interfaceName;
	std::map<std::string, sdbus::Variant> changedProperties;
	std::vector<std::string> invalidatedProperties;

	std::vector<std::pair<IdfmTransportMode, std::string>> vec;

	auto addPairIfNotExists = [&](const std::pair<IdfmTransportMode, std::string>& p) {
		if (std::find(vec.begin(), vec.end(), p) == vec.end()) {
			vec.push_back(p);
			BOOST_LOG_TRIVIAL(trace) << "CommutatorIdfmLineReports: Pair added: (" << idfmTransportModeToString(p.first) << ", " << p.second << ")";
		} else {
			BOOST_LOG_TRIVIAL(trace) << "CommutatorIdfmLineReports: Pair already present: (" << idfmTransportModeToString(p.first) << ", " << p.second << ")";
		}
	};

	signal >> interfaceName >> changedProperties >> invalidatedProperties;

	if (!changedProperties.empty()) {
		for (const auto& [propertyName, propertyValue] : changedProperties) {

			IdfmTransportMode mode = getModeFromString(interfaceName);
			std::string line = getLineFromString(mode, interfaceName);
			
			std::string valueStr = propertyValue.get<std::string>();
			BOOST_LOG_TRIVIAL(info) << "CommutatorIdfmLineReports: " << interfaceName <<  " New " << propertyName << " value: " << valueStr;

			addPairIfNotExists(std::make_pair(mode, line));
		}

		for (const auto& p : vec) {
			BOOST_LOG_TRIVIAL(trace) << "CommutatorIdfmLineReports: newData " << "(" << idfmTransportModeToString(p.first) << ", " << p.second << ")";
			newData(p.first, p.second);
		}
	}
}

CommutatorIdfmLineReports::CommutatorIdfmLineReports():
	Commutator("IdfmLineReports") {
	const char* destinationName = "com.commutator.IdfmLineReports";
	const char* objectPath = "/com/commutator/IdfmLineReports";
	std::unique_ptr<sdbus::IProxy> proxy = sdbus::createProxy(destinationName, objectPath);

	auto method = proxy->createMethodCall("org.freedesktop.DBus.Introspectable", "Introspect");
	auto reply = proxy->callMethod(method);
	std::string introspectionData;
	reply >> introspectionData;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(introspectionData.c_str());

	if (!result) {
		BOOST_LOG_TRIVIAL(error) << "CommutatorIdfmLineReports: Failed to parse XML: " << result.description();
		return;
	}

	for (pugi::xml_node node = doc.child("node").child("node"); node; node = node.next_sibling("node")) {
		const std::string lineName = node.attribute("name").as_string();
		const std::string destinationName = "com.commutator.IdfmLineReports";
		const std::string objectPath = "/com/commutator/IdfmLineReports/" + lineName;

		auto proxy = sdbus::createProxy(destinationName, objectPath);

		const char* interfaceName = "org.freedesktop.DBus.Properties";

		proxy->registerSignalHandler(interfaceName, "PropertiesChanged",
						   [this](sdbus::Signal& signal) {
							   this->onPropertiesChanged(signal);
						   });
		proxy->finishRegistration();

		auto method = proxy->createMethodCall(interfaceName, "GetAll");
		method << "";
		try {
			auto reply = proxy->callMethod(method);
			std::map<std::string, sdbus::Variant> properties;
			reply >> properties;

			std::string color, id, name, severityColor, severityEffect, textColor;
			for (const auto& [key, value] : properties) {
				std::string valueStr;
				valueStr = value.get<std::string>();

				if (key == "color")
					color = valueStr;
				else if (key == "id")
					id = valueStr;
				else if (key == "name")
					name = valueStr;
				else if (key == "severity_color")
					severityColor = valueStr;
				else if (key == "severity_effect")
					severityEffect = valueStr;
				else if (key == "text_color")
					textColor = valueStr;
				else
					throw std::out_of_range("Unhandled property " + key + "!");
			}

			IdfmTransportMode mode = getModeFromString(lineName);

			std::unique_ptr<IdfmLineStatus> lineStatus = std::make_unique<IdfmLineStatus>(mode, color, id, name, severityColor, severityEffect, textColor);
			BOOST_LOG_TRIVIAL(trace) << "CommutatorIdfmLineReports: line " << lineStatus->getName() << " created!";

			m_linesMap[std::move(lineStatus)] = std::move(proxy);
		}
		catch(const sdbus::Error& e) {
			BOOST_LOG_TRIVIAL(error) << "CommutatorIdfmLineReports: " << "Got error " << e.getName() << " with message " << e.getMessage();
		}
	}

	BOOST_LOG_TRIVIAL(info) << "CommutatorIdfmLineReports: " << "CommutatorIdfmLineReports created.";
}

std::string CommutatorIdfmLineReports::getLineSeverityEffect(IdfmTransportMode mode, std::string name) const {
	std::string str = "";

	for (const auto& [line, proxy] : m_linesMap) {
		if (mode == line->getTransportMode()
		    && line->getName() == name) {
			str = line->getSeverityEffect();
		}
	}

	return str;
}
