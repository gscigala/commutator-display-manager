#include "Widget.h"

#include <boost/signals2.hpp>
#include <boost/log/trivial.hpp>

#include <opencv2/opencv.hpp>

Widget::Widget()
{
	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			displayArray[i][j] = Display::Color::WHITE;
		}
	}
}

void Widget::generateDebugImage() const {
	cv::Mat image(Display::WIDGET_ROWS, Display::WIDGET_COLS, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int i = 0; i < Display::WIDGET_ROWS; ++i) {
		for (int j = 0; j < Display::WIDGET_COLS; ++j) {
			cv::Scalar color;
			switch (displayArray[i][j]) {
			case Display::Color::BLACK:
				color = cv::Scalar(0, 0, 0);
				break;
			case Display::Color::RED:
				color = cv::Scalar(0, 0, 255);
				break;
			case Display::Color::YELLOW:
				color = cv::Scalar(0, 255, 255);
				break;
			case Display::Color::WHITE:
			default:
				color = cv::Scalar(255, 255, 255);
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
				array[x][y] = Display::Color::BLACK;
			else
				array[x][y] = Display::Color::WHITE;
		}
	}

	return 0;
}
