#include "WidgetSytadin.h"

#include "../Fonts/fonts.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

WidgetSytadin::WidgetSytadin(std::shared_ptr<CommutatorSytadin> commutator):
	Widget(),
	m_commutator(commutator),
	m_trafficValueThreshold(5),
	m_displayLine1RowsNb(35),
	m_displayLine1OffsetY(2)
{
	int cols = 80;
	
	m_trafficLevel = m_commutator->getTrafficLevel();
	m_trafficTendency = m_commutator->getTrafficTendency();
	m_trafficValue = m_commutator->getTrafficValue();
	BOOST_LOG_TRIVIAL(trace) << getName() << ": trafficLevel = " << m_trafficLevel << ", trafficTendency = " << m_trafficTendency << ", trafficValue = " << m_trafficValue;

	m_commutator->newData.connect(boost::bind(&WidgetSytadin::newDataSlot, this));

	m_carImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_carImage[i] = new Display::Color[cols];
	}
	if (loadImage("res/car.png", m_carImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load car.png");

	cols = 35;
	m_arrowTopRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowTopRightImage[i] = new Display::Color[cols];
	}
	if (loadImage("res/arrow-top-right.png", m_arrowTopRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-top-right.png");

	cols = 35;
	m_arrowRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowRightImage[i] = new Display::Color[cols];
	}
	if (loadImage("res/arrow-right.png", m_arrowRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-right.png");

	cols = 35;
	m_arrowBottomRightImage = new Display::Color*[m_displayLine1RowsNb];
	for (int i = 0; i < m_displayLine1RowsNb; ++i) {
	        m_arrowBottomRightImage[i] = new Display::Color[cols];
	}
	if (loadImage("res/arrow-bottom-right.png", m_arrowBottomRightImage, m_displayLine1RowsNb, cols) < 0)
		throw std::runtime_error("Unable to load arrow-bottom-right.png");

	redraw();
}

WidgetSytadin::~WidgetSytadin()
{
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

	tmpTrafficLevel = m_commutator->getTrafficLevel();
	if (tmpTrafficLevel != m_trafficLevel)
		update = true;

	tmpTrafficTendency = m_commutator->getTrafficTendency();
	if (tmpTrafficTendency != m_trafficTendency)
		update = true;

	tmpTrafficValue = m_commutator->getTrafficValue();
	if (abs(tmpTrafficValue - m_trafficValue) >= m_trafficValueThreshold)
		update = true;

	if (update) {
		BOOST_LOG_TRIVIAL(trace) << "WidgetSytadin: need update widget";

		m_trafficLevel = tmpTrafficLevel;
		m_trafficTendency = tmpTrafficTendency;
		m_trafficValue = tmpTrafficValue;

		redraw();
	}
}

void WidgetSytadin::redraw()
{
	Display::Color textColor, backgroundColor;
	const int xCarOffset = 5;
	const int xArrowOffset = 85;
	const int xTrafficOffset1Digit = 32;
	const int xTrafficOffset2Digit = 22;
	const int xTrafficOffset3Digit = 12;
	const int yTrafficOffset = 40;
	int xTrafficOffset;
	std::string trafficValueStr;
	Display::Color** arrowImage;

	if (m_trafficLevel == "Unusual") {
		textColor = Display::Color::RED;
		backgroundColor = Display::Color::WHITE;
	} else if (m_trafficLevel == "Exceptional") {
		textColor = Display::Color::BLACK;
		backgroundColor = Display::Color::RED;
	} else {
		textColor = Display::Color::BLACK;
		backgroundColor = Display::Color::WHITE;
	}

	if (m_trafficTendency == "Increasing")
		arrowImage = m_arrowTopRightImage;
	else if (m_trafficTendency == "Stable")
		arrowImage = m_arrowRightImage;
	else
		arrowImage = m_arrowBottomRightImage;

	/* redraw background */
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			displayArray[i][j] = backgroundColor;
		}
	}

	/* redraw car image */
	Display::drawImage(xCarOffset, m_displayLine1OffsetY, m_carImage, m_displayLine1RowsNb, 80, textColor, backgroundColor, displayArray);

	/* redraw arrow */
	Display::drawImage(xArrowOffset, m_displayLine1OffsetY, arrowImage, m_displayLine1RowsNb, 35, textColor, backgroundColor, displayArray);

	/* redraw traffic value */
	trafficValueStr = std::to_string(m_trafficValue) + " km";
	if (trafficValueStr.length() == 4)
		xTrafficOffset = xTrafficOffset1Digit;
	else if (trafficValueStr.length() == 5)
		xTrafficOffset = xTrafficOffset2Digit;
	else
		xTrafficOffset = xTrafficOffset3Digit;
	Display::drawString(xTrafficOffset, yTrafficOffset, trafficValueStr, &Font24, textColor, backgroundColor, displayArray);

	generateDebugImage();
}
