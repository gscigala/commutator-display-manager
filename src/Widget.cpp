#include "Widget.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

#include <opencv2/opencv.hpp>

Widget::Widget()
{
	m_displayArray = new Display::Color*[Display::WIDGET_ROWS];
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
	        m_displayArray[i] = new Display::Color[Display::WIDGET_COLS];
	}
	
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			m_displayArray[i][j] = Display::Color::White;
		}
	}
}

Widget::~Widget()
{
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		delete[] m_displayArray[i];
	}
	delete[] m_displayArray;
}

void Widget::generateDebugImage() const {
	cv::Mat image(Display::WIDGET_ROWS, Display::WIDGET_COLS, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			cv::Scalar color;
			switch (m_displayArray[i][j]) {
			case Display::Color::Black:
				color = cv::Scalar(3, 11, 18);
				break;
			case Display::Color::Red:
				color = cv::Scalar(11, 25, 161);
				break;
			case Display::Color::Yellow:
				color = cv::Scalar(14, 180, 255);
				break;
			case Display::Color::White:
			default:
				color = cv::Scalar(208, 203, 203);
				break;
			}
			image.at<cv::Vec3b>(i, j) = cv::Vec3b(color[0], color[1], color[2]);
		}
	}

	cv::imwrite("/tmp/" + getName() + ".png", image);
}

int Widget::loadImage(std::string imagePath, Display::Color** const array, int rows, int cols) const {

	cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);

	if (image.empty()) {
		BOOST_LOG_TRIVIAL(error) << getName() << ": Unable to load image " << imagePath;
		return -1;
	}

	if (image.channels() != 4) {
		BOOST_LOG_TRIVIAL(error) << getName() << ": image " << imagePath << " doesn't have alpha";
		return -1;
	}

	for (int x = 0; x < rows; ++x) {
		for (int y = 0; y < cols; ++y) {
			cv::Vec4b pixel = image.at<cv::Vec4b>(x, y);

			if (pixel[3] == 255)
				array[x][y] = Display::Color::Black;
			else
				array[x][y] = Display::Color::White;
		}
	}

	return 0;
}
