#include "WidgetSytadin.h"

#include "../Fonts/fonts.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetSytadin::WidgetSytadin(std::shared_ptr<CommutatorSytadin> commutator, std::string resDirPath):
	Widget(),
	m_commutator(commutator),
	m_trafficValueThreshold(5),
	m_displayLine1RowsNb(35),
	m_displayLine1OffsetY(6),
	m_onlyTrafficValueThreshold(40)
{
	int cols;

	m_trafficLevel = m_commutator->getTrafficLevel();
	m_trafficTendency = m_commutator->getTrafficTendency();
	m_trafficValue = m_commutator->getTrafficValue();
	BOOST_LOG_TRIVIAL(trace) << getName() << ": trafficLevel = " << m_trafficLevel << ", trafficTendency = " << m_trafficTendency << ", trafficValue = " << m_trafficValue;

	if (m_trafficValue > m_onlyTrafficValueThreshold)
		m_onlyTrafficValue = false;
	else
		m_onlyTrafficValue = true;

	m_commutator->newData.connect(boost::bind(&WidgetSytadin::newDataSlot, this));

	cols = 80;
	m_carImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_carImage[i] = new Display::Color[cols];
	}
	if (loadImage(resDirPath + "/car.png", m_carImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load car.png");

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

WidgetSytadin::~WidgetSytadin()
{
	BOOST_LOG_TRIVIAL(trace) << getName() << ": destructor!";

	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
		delete[] m_carImage[i];
	}
	delete[] m_carImage;

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

void WidgetSytadin::newDataSlot()
{
	std::string tmpTrafficLevel, tmpTrafficTendency;
	int tmpTrafficValue;
	bool update = false;

	BOOST_LOG_TRIVIAL(debug) << "WidgetSytadin: new data!";

	tmpTrafficValue = m_commutator->getTrafficValue();
	if (abs(tmpTrafficValue - m_trafficValue) >= m_trafficValueThreshold)
		update = true;

	if (tmpTrafficValue > m_onlyTrafficValueThreshold) {
		/* traffic level and tendency only available
		 * upper traffic threshold */
		tmpTrafficLevel = m_commutator->getTrafficLevel();
		if (tmpTrafficLevel != m_trafficLevel)
			update = true;

		tmpTrafficTendency = m_commutator->getTrafficTendency();
		if (tmpTrafficTendency != m_trafficTendency)
			update = true;

		m_onlyTrafficValue = false;
	} else {
		m_onlyTrafficValue = true;
	}

	if (update) {
		BOOST_LOG_TRIVIAL(debug) << "WidgetSytadin: need update widget";

		m_trafficLevel = tmpTrafficLevel;
		m_trafficTendency = tmpTrafficTendency;
		m_trafficValue = tmpTrafficValue;

		redraw();
	} else {
		BOOST_LOG_TRIVIAL(debug) << "WidgetSytadin: display update not needed";
	}
}

void WidgetSytadin::redraw()
{
	Display::Color textColor, backgroundColor;
	Display::Color** arrowImage;
	const int xCarNotAloneOffset = 12;
	const int xCarAloneOffset = 31;
	int xCarOffset;
	const int xArrowOffset = 92;
	const int xTrafficOffset1Digit = 39;
	const int xTrafficOffset2Digit = 29;
	const int xTrafficOffset3Digit = 19;
	const int yTrafficOffset = 47;
	int xTrafficOffset;
	std::string trafficValueStr;
	int cols;

	BOOST_LOG_TRIVIAL(trace) << getName() << ": redraw";

	if (!m_onlyTrafficValue) {
		if (m_trafficLevel == "Unusual") {
			textColor = Display::Color::Red;
			backgroundColor = Display::Color::White;
		} else if (m_trafficLevel == "Exceptional") {
			textColor = Display::Color::White;
			backgroundColor = Display::Color::Red;
		} else {
			textColor = Display::Color::Black;
			backgroundColor = Display::Color::White;
		}

		if (m_trafficTendency == "Increasing")
			arrowImage = m_arrowTopRightImage;
		else if (m_trafficTendency == "Stable")
			arrowImage = m_arrowRightImage;
		else
			arrowImage = m_arrowBottomRightImage;
	} else {
		textColor = Display::Color::Black;
		backgroundColor = Display::Color::White;

		arrowImage = nullptr;
	}

	/* redraw background */
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			m_displayArray[i][j] = backgroundColor;
		}
	}

	/* redraw car image */
	cols = 80;
	xCarOffset = (m_onlyTrafficValue ? xCarAloneOffset : xCarNotAloneOffset);
	Display::drawImage(xCarOffset, m_displayLine1OffsetY, m_carImage, m_displayLine1RowsNb, cols, textColor, backgroundColor, m_displayArray);

	if (!m_onlyTrafficValue) {
		/* redraw arrow */
		cols = 35;
		Display::drawImage(xArrowOffset, m_displayLine1OffsetY, arrowImage, m_displayLine1RowsNb, cols, textColor, backgroundColor, m_displayArray);
	}

	/* redraw traffic value */
	trafficValueStr = std::to_string(m_trafficValue) + " km";
	if (trafficValueStr.length() == 4)
		xTrafficOffset = xTrafficOffset1Digit;
	else if (trafficValueStr.length() == 5)
		xTrafficOffset = xTrafficOffset2Digit;
	else
		xTrafficOffset = xTrafficOffset3Digit;
	Display::drawString(xTrafficOffset, yTrafficOffset, trafficValueStr, &Font24, textColor, backgroundColor, m_displayArray);

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
