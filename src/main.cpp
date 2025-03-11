#include "Supervisor.h"
#include "CommutatorSytadin.h"
#include "WidgetSytadin.h"
#include "CommutatorVigicrues.h"
#include "WidgetVigicrues.h"
#include "IdfmTransportMode.h"
#include "CommutatorIdfmLineReports.h"
#include "WidgetIdfmLineReports.h"
#include "Display.h"

#include <iostream>
#include <fstream>
#include <csignal>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace po = boost::program_options;

bool running = true;

void signalHandler(int signum) {
    std::cout << "Catch exit signal" << std::endl;
    running = false;
}

std::shared_ptr<Widget> createWidget(const json config, const std::string resDirPath,  Display::WidgetPosition position, std::shared_ptr<CommutatorSytadin> commutatorSytadin, std::shared_ptr<CommutatorVigicrues> commutatorVigicrues, std::shared_ptr<CommutatorIdfmLineReports> commutatorIdfmLineReports)
{
	std::string positionStr = Display::positionToString(position);

	if (!config.contains("widgets") || !config["widgets"].is_object()) {
		throw std::runtime_error("Error : key 'widgets' is missing or is not an object.");
	}

	const json& widgets = config["widgets"];

	if (!widgets.contains(positionStr) || !widgets[positionStr].is_object()) {
		BOOST_LOG_TRIVIAL(warning) << "widget position " << positionStr << " is missing";
		return nullptr;
	}

	const json& widgetConfig = widgets[positionStr];

	if (!widgetConfig.contains("type") || !widgetConfig["type"].is_string()) {
		throw std::runtime_error("Error : key 'type' is missing or is not a string for widget '" + positionStr + "'.");
	}

	std::string widgetType = widgetConfig["type"];
	if (widgetType == "Sytadin") {
		return std::make_shared<WidgetSytadin>(commutatorSytadin, resDirPath);
	} else if (widgetType == "Vigicrues") {
		std::string capability = widgetConfig["capability"];
		float alert = widgetConfig["thresholds"]["alert"];
		float blocking = widgetConfig["thresholds"]["blocking"];
		return std::make_shared<WidgetVigicrues>(commutatorVigicrues, resDirPath,  WidgetVigicrues::stringToCapability(capability), alert, blocking);
	} else if (widgetType == "IdfmLineReports") {
		std::string transportMode = widgetConfig["transportMode"];
		std::string line = widgetConfig["line"];
		return std::make_shared<WidgetIdfmLineReports>(commutatorIdfmLineReports, resDirPath, WidgetIdfmLineReports::stringToTransportMode(transportMode), line);
	} else {
		throw std::runtime_error("Error : Unknown type '" + widgetType + "' in widget '" + positionStr + "'.");
	}

	return nullptr;
}

int main(int argc, char* argv[])
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	po::options_description desc("Options");
	desc.add_options()
		("help,h", "Print help messages")
		("config,c", po::value<std::string>(), "Configuration file path")
		("ressources,r", po::value<std::string>(), "Ressources directory path")
		("fake", "Enable fake epaper mode");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	std::string configPath = "/etc/commutator-display-manager/config.json";
	if (vm.count("config")) {
		configPath = vm["config"].as<std::string>();
	}

	std::ifstream configFile(configPath);
	BOOST_LOG_TRIVIAL(debug) << "Configuration file = " << configPath;

	if (!configFile.is_open()) {
		BOOST_LOG_TRIVIAL(error) << "Unable to open configuration file " << configPath;
		return 1;
	}

	json config;
	try {
		configFile >> config;
	} catch (const json::parse_error& e) {
		std::cerr << "Error: in JSON format : " << e.what() << std::endl;
		return 1;
	}

	std::string resDirPath = "/etc/commutator-display-manager/ressources";
	if (vm.count("ressources")) {
		resDirPath = vm["ressources"].as<std::string>();
	}
	BOOST_LOG_TRIVIAL(debug) << "Ressources directory path = " << resDirPath;

	Supervisor supervisor(vm.count("fake"));

	std::shared_ptr<CommutatorSytadin> commutatorSytadin = std::make_shared<CommutatorSytadin>();
	std::shared_ptr<CommutatorVigicrues> commutatorVigicrues = std::make_shared<CommutatorVigicrues>();
	std::shared_ptr<CommutatorIdfmLineReports> commutatorIdfmLineReports = std::make_shared<CommutatorIdfmLineReports>();

	std::shared_ptr<Widget> widgetTopLeft;
	std::shared_ptr<Widget> widgetTopRight;
	std::shared_ptr<Widget> widgetBottomLeft;
	std::shared_ptr<Widget> widgetBottomRight;

	try {
		widgetTopLeft = createWidget(config, resDirPath, Display::WidgetPosition::TOP_LEFT, commutatorSytadin, commutatorVigicrues, commutatorIdfmLineReports);
		widgetTopRight = createWidget(config, resDirPath,  Display::WidgetPosition::TOP_RIGHT, commutatorSytadin, commutatorVigicrues, commutatorIdfmLineReports);
		widgetBottomLeft = createWidget(config, resDirPath,  Display::WidgetPosition::BOTTOM_LEFT, commutatorSytadin, commutatorVigicrues, commutatorIdfmLineReports);
		widgetBottomRight = createWidget(config, resDirPath, Display::WidgetPosition::BOTTOM_RIGHT, commutatorSytadin, commutatorVigicrues, commutatorIdfmLineReports);
	} catch (const std::invalid_argument& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	if (widgetTopLeft && supervisor.registerWidget(widgetTopLeft,
						       Display::WidgetPosition::TOP_LEFT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget top left!";
		return -1;
	}

	if (widgetTopRight && supervisor.registerWidget(widgetTopRight,
							Display::WidgetPosition::TOP_RIGHT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget top right!";
		return -1;
	}

	if (widgetBottomLeft && supervisor.registerWidget(widgetBottomLeft,
							  Display::WidgetPosition::BOTTOM_LEFT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget bottom left!";
		return -1;
	}

	if (widgetBottomRight && supervisor.registerWidget(widgetBottomRight,
							   Display::WidgetPosition::BOTTOM_RIGHT) != 0) {
		BOOST_LOG_TRIVIAL(error) << "Unable to register widget bottom right!";
		return -1;
	}

	supervisor.start();

	while(running) {
		sleep(1);
	}

	return 0;
}
