#include "WidgetVigicrues.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

#include <sstream>
#include <iomanip>

WidgetVigicrues::WidgetVigicrues(std::shared_ptr<CommutatorVigicrues> commutator, std::string resDirPath, Capability capability, float alertThreshold, float floodThreshold):
	Widget(),
	m_commutator(commutator),
	m_alertThreshold(alertThreshold),
	m_floodThreshold(floodThreshold),
	m_displayLine1RowsNb(35),
	m_displayLine1OffsetY(6),
	m_capability(capability)
{
	int cols;

	BOOST_LOG_TRIVIAL(trace) << getName() << ": alert threshold = " << m_alertThreshold;
	BOOST_LOG_TRIVIAL(trace) << getName() << ": flood threshold = " << m_floodThreshold;

	if (m_alertThreshold > m_floodThreshold)
		throw std::out_of_range("The vigicrues alert level must be lower than flood threshold!");

	if (m_capability == Capability::FLOW) {
		m_tendency = m_commutator->getFlowTendency();
		m_value = m_commutator->getFlowValue();
		BOOST_LOG_TRIVIAL(trace) << getName() << ": tendency = " << m_tendency << ", value = " << m_value;
	}
	else if (m_capability == Capability::WATER_LEVEL) {
		m_tendency = m_commutator->getWaterLevelTendency();
		m_value = m_commutator->getWaterLevelValue();
		BOOST_LOG_TRIVIAL(trace) << getName() << ": tendency = " << m_tendency << ", value = " << m_value;
	}
	else {
		throw std::out_of_range("Unknown capabiliy!");
	}
	
	m_commutator->newData.connect(boost::bind(&WidgetVigicrues::newDataSlot, this));

	cols = 35;
	m_waveImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_waveImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/wave.png", m_waveImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load wave.png");

	if (m_capability == Capability::FLOW) {
		cols = 20;
		m_symbolImage = new Display::Color*[m_displayLine1RowsNb];
		for (int i = 0; i < m_displayLine1RowsNb; ++i) {
			m_symbolImage[i] = new Display::Color[cols];
		}
		if (loadImage(resDirPath + "/water-flow-symbol.png", m_symbolImage, m_displayLine1RowsNb, cols) < 0)
			throw std::runtime_error("Unable to load water-flow-symbol.png");

	} else if (m_capability == Capability::WATER_LEVEL) {
		cols = 20;
		m_symbolImage = new Display::Color*[m_displayLine1RowsNb];
		for (int i = 0; i < m_displayLine1RowsNb; ++i) {
			m_symbolImage[i] = new Display::Color[cols];
		}
		if (loadImage(resDirPath + "/water-level-symbol.png", m_symbolImage, m_displayLine1RowsNb, cols) < 0)
			throw std::runtime_error("Unable to load water-level-symbol.png");
		
	} else {
		throw std::out_of_range("Unknown capabiliy!");
	}

	cols = 35;
	m_arrowTopRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowTopRightImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/arrow-top-right.png", m_arrowTopRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-top-right.png");

	cols = 35;
	m_arrowRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowRightImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/arrow-right.png", m_arrowRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-right.png");

	cols = 35;
	m_arrowBottomRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowBottomRightImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/arrow-bottom-right.png", m_arrowBottomRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-bottom-right.png");

	redraw();
}

WidgetVigicrues::~WidgetVigicrues()
{
	BOOST_LOG_TRIVIAL(trace) << getName() << ": destructor!";

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_waveImage[i];
	}
	delete[] m_waveImage;

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_symbolImage[i];
	}
	delete[] m_symbolImage;

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_arrowTopRightImage[i];
	}
	delete[] m_arrowTopRightImage;

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_arrowRightImage[i];
	}
	delete[] m_arrowRightImage;

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_arrowBottomRightImage[i];
	}
	delete[] m_arrowBottomRightImage;
}

void WidgetVigicrues::newDataSlot() {
	BOOST_LOG_TRIVIAL(debug) << getName() << ": new data!";

	std::string tmpTendency;
	float tmpValue;

	if (m_capability == Capability::FLOW) {
		tmpTendency = m_commutator->getFlowTendency();
		tmpValue = m_commutator->getFlowValue();
	}
	else if (m_capability == Capability::WATER_LEVEL) {
		tmpTendency = m_commutator->getWaterLevelTendency();
		tmpValue = m_commutator->getWaterLevelValue();
	}
	else {
		BOOST_LOG_TRIVIAL(error) << getName() << ": Unknown capability!";
		return;
	}

	if (!areEqual(m_value, tmpValue) || (m_tendency != tmpTendency)) {
		BOOST_LOG_TRIVIAL(trace) << getName() << ": need update widget";

		m_tendency = tmpTendency;
		m_value = tmpValue;

		redraw();
	}
}

void WidgetVigicrues::redraw()
{
	Display::Color textColor, backgroundColor;
	Display::Color** arrowImage;
	const int xWaveOffset = 22;
	const int xSymbolOffset = 69;
	const int xArrowOffset = 94;
	const int xValueOffset = 20;
	const int yValueOffset = 47;
	std::string valueStr;
	int cols;

	BOOST_LOG_TRIVIAL(trace) << getName() << ": redraw";

	if (m_value < m_alertThreshold) {
		textColor = Display::Color::Black;
		backgroundColor = Display::Color::White;
	} else if (m_alertThreshold < m_value && m_value < m_floodThreshold) {
		textColor = Display::Color::Red;
		backgroundColor = Display::Color::White;	
	} else {
		textColor = Display::Color::White;
		backgroundColor = Display::Color::Red;
	}

	if (m_tendency == "Increasing")
		arrowImage = m_arrowTopRightImage;
	else if (m_tendency == "Stable")
		arrowImage = m_arrowRightImage;
	else
		arrowImage = m_arrowBottomRightImage;

	/* redraw background */
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			m_displayArray[i][j] = backgroundColor;
		}
	}

	/* redraw wave image */
	cols = 35;
	Display::drawImage(xWaveOffset, m_displayLine1OffsetY, m_waveImage, m_displayLine1RowsNb, cols, textColor, backgroundColor, m_displayArray);

	/* redraw symbol image */
	cols = 20;
	Display::drawImage(xSymbolOffset, m_displayLine1OffsetY, m_symbolImage, m_displayLine1RowsNb, cols, textColor, backgroundColor, m_displayArray);

	/* redraw arrow */
	cols = 35;
	Display::drawImage(xArrowOffset, m_displayLine1OffsetY, arrowImage, m_displayLine1RowsNb, cols, textColor, backgroundColor, m_displayArray);

	/* redraw value */
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << m_value;
	valueStr = oss.str() + " m";
	Display::drawString(xValueOffset, yValueOffset, valueStr, &Font24, textColor, backgroundColor, m_displayArray);

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

std::string WidgetVigicrues::capabilityToString(Capability capability) const {

	std::string str;

	switch(capability) {
	case (Capability::FLOW): {
		str = "FLOW";
		break;
	}
	case (Capability::WATER_LEVEL): {
		str= "WATER_LEVEL";
		break;
	}
	default: {
		str = "UNKNOWN";
		break;
	}
	}

	return str;
}
