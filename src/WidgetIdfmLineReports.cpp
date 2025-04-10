#include "WidgetIdfmLineReports.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetIdfmLineReports::WidgetIdfmLineReports(std::shared_ptr<CommutatorIdfmLineReports> commutator, std::string resDirPath, IdfmTransportMode mode, std::string name):
	Widget(),
	m_commutator(commutator),
	m_displayLineRowsNb(45),
	m_displayLineOffsetY(17),
	m_mode(mode),
	m_name(name)
{
	int cols;
	std::string nameImageStr, errorStr;

	m_severityEffect = m_commutator->getLineSeverityEffect(m_mode, m_name);
	if (m_severityEffect == "") {
		BOOST_LOG_TRIVIAL(error) << getName() << ": unknown line " << m_name;
		throw std::runtime_error("unknown line!");
	} else {
		BOOST_LOG_TRIVIAL(trace) << getName() << ": m_severityEffect = " << m_severityEffect;
	}
	
	m_commutator->newData.connect(boost::bind(&WidgetIdfmLineReports::newDataSlot, this, std::placeholders::_1, std::placeholders::_2));

	/* load transport mode image */
	cols = 45;
	if (m_mode == IdfmTransportMode::METRO) {
		m_modeImage = new Display::Color*[m_displayLineRowsNb];
		for (int i = 0; i < m_displayLineRowsNb; ++i) {
			m_modeImage[i] = new Display::Color[cols];
		}
		if (loadImage(resDirPath + "/metro.png", m_modeImage, m_displayLineRowsNb, cols) < 0)
			throw std::runtime_error("Unable to load metro.png");

		nameImageStr = resDirPath + "/metro-" + toLowerCase(m_name) + ".png";

	} else if (m_mode == IdfmTransportMode::RAPID_TRANSIT) {
		m_modeImage = new Display::Color*[m_displayLineRowsNb];
		for (int i = 0; i < m_displayLineRowsNb; ++i) {
			m_modeImage[i] = new Display::Color[cols];
		}
		if (loadImage(resDirPath + "/rer.png", m_modeImage, m_displayLineRowsNb, cols) < 0)
			throw std::runtime_error("Unable to load rer.png");

		nameImageStr = resDirPath + "/rer-" + toLowerCase(m_name) + ".png";

	} else if (m_mode == IdfmTransportMode::LOCAL_TRAIN) {
		m_modeImage = new Display::Color*[m_displayLineRowsNb];
		for (int i = 0; i < m_displayLineRowsNb; ++i) {
			m_modeImage[i] = new Display::Color[cols];
		}
		if (loadImage(resDirPath + "/transilien.png", m_modeImage, m_displayLineRowsNb, cols) < 0)
			throw std::runtime_error("Unable to load transilien.png");

		nameImageStr = resDirPath + "/transilien-" + toLowerCase(m_name) + ".png";
	} else {
		throw std::runtime_error("Unknown transport mode!");
	}

	/* load line name image */
	cols = 45;
	m_nameImage = new Display::Color*[m_displayLineRowsNb];
	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		m_nameImage[i] = new Display::Color[cols];
	}
	errorStr = "Unable to load " + nameImageStr;
	if (loadImage(nameImageStr, m_nameImage, m_displayLineRowsNb, cols) < 0)
		throw std::runtime_error(errorStr);

	/* load traffic states */

	cols = 42;
	m_trafficWarningImage = new Display::Color*[m_displayLineRowsNb];
	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		m_trafficWarningImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/warning.png", m_trafficWarningImage, m_displayLineRowsNb, cols) < 0)
		throw std::runtime_error(errorStr);

	cols = 42;
	m_trafficStoppedImage = new Display::Color*[m_displayLineRowsNb];
	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		m_trafficStoppedImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/cross.png", m_trafficStoppedImage, m_displayLineRowsNb, cols) < 0)
		throw std::runtime_error(errorStr);

	redraw();
}

WidgetIdfmLineReports::~WidgetIdfmLineReports()
{
	BOOST_LOG_TRIVIAL(trace) << getName() << ": destructor!";
	
	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		delete[] m_modeImage[i];
	}
	delete[] m_modeImage;

	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		delete[] m_nameImage[i];
	}
	delete[] m_nameImage;

	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		delete[] m_trafficWarningImage[i];
	}
	delete[] m_trafficWarningImage;

	for (int i = 0; i < m_displayLineRowsNb; ++i) {
		delete[] m_trafficStoppedImage[i];
	}
	delete[] m_trafficStoppedImage;
}

void WidgetIdfmLineReports::newDataSlot(IdfmTransportMode mode, const std::string& name)
{
	BOOST_LOG_TRIVIAL(debug) << getName() << ": new data from (" << idfmTransportModeToString(mode) << " " << name << ")";

	if ((mode == m_mode) && (name == m_name)) {
		std::string severityEffect = m_commutator->getLineSeverityEffect(m_mode, m_name);
		BOOST_LOG_TRIVIAL(debug) << getName() << ": new data match with this widget!";
		if (m_severityEffect != severityEffect) {
			m_severityEffect = severityEffect;

			redraw();
		}
	}
}

void WidgetIdfmLineReports::redraw()
{
	Display::Color textColor, backgroundColor;

	const int xModeOffsetWithoutIssue = 19;
	const int xNameOffsetWithoutIssue = 83;
	const int xModeOffsetWithIssue = 6;
	const int xNameOffsetWithIssue = 57;
	const int xtrafficIssueOffset = 103;

	int xModeOffset;
	int xNameOffset;
	int cols;

	BOOST_LOG_TRIVIAL(trace) << getName() << ": redraw";

	if (m_severityEffect == "SIGNIFICANT_DELAYS") {
		textColor = Display::Color::Red;
		backgroundColor = Display::Color::White;
		xModeOffset = xModeOffsetWithIssue;
		xNameOffset = xNameOffsetWithIssue;
	} else if (m_severityEffect == "NO_SERVICE") {
		textColor = Display::Color::White;
		backgroundColor = Display::Color::Red;
		xModeOffset = xModeOffsetWithIssue;
		xNameOffset = xNameOffsetWithIssue;
	} else {
		textColor = Display::Color::Black;
		backgroundColor = Display::Color::White;
		xModeOffset = xModeOffsetWithoutIssue;
		xNameOffset = xNameOffsetWithoutIssue;
	}

	/* redraw background */
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			m_displayArray[i][j] = backgroundColor;
		}
	}

	/* redraw transport mode image */
	cols = 45;
	Display::drawImage(xModeOffset, m_displayLineOffsetY, m_modeImage, m_displayLineRowsNb, cols, textColor, backgroundColor, m_displayArray);

	/* redraw line name image */
	cols = 45;
	Display::drawImage(xNameOffset, m_displayLineOffsetY, m_nameImage, m_displayLineRowsNb, cols, textColor, backgroundColor, m_displayArray);

	/* redraw traffic alert */
	if (m_severityEffect == "SIGNIFICANT_DELAYS") {
		cols = 42;
		Display::drawImage(xtrafficIssueOffset, m_displayLineOffsetY, m_trafficWarningImage, m_displayLineRowsNb, cols, textColor, backgroundColor, m_displayArray);
	} else if (m_severityEffect == "NO_SERVICE") {
		cols = 42;
		Display::drawImage(xtrafficIssueOffset, m_displayLineOffsetY, m_trafficStoppedImage, m_displayLineRowsNb, cols, textColor, backgroundColor, m_displayArray);
	} else {
		/* Do nothing, no traffic alert */
	}

	/* redraw debug border */
	/*for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			if ((j == 0) || (j == Display::WIDGET_COLS-1)) {
				m_displayArray[i][j] = Display::Color::YELLOW;	
			}
			if ((i == 0) || i == (Display::WIDGET_ROWS-1)) {
				m_displayArray[i][j] = Display::Color::YELLOW;
			}
		}
		}*/

	generateDebugImage();

	newData();
}

std::string WidgetIdfmLineReports::toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lowerStr;
}
