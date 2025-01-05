#include "IdfmLineStatus.h"

IdfmLineStatus::IdfmLineStatus(IdfmTransportMode transportMode, std::string color, std::string id, std::string name, std::string severityColor, std::string severityEffect, std::string textColor)
	: m_transportMode(transportMode), m_color(color), m_id(id), m_name(name), m_severityColor(severityColor), m_severityEffect(severityEffect), m_textColor(textColor) {}

void IdfmLineStatus::setSeverityColor(const std::string& severityColor) {
	m_severityColor = severityColor;
}

void IdfmLineStatus::setSeverityEffect(const std::string& severityEffect) {
	m_severityEffect = severityEffect;
}

void IdfmLineStatus::setTextColor(const std::string& textColor) {
	m_textColor = textColor;
}

IdfmTransportMode IdfmLineStatus::getTransportMode() const {
	return m_transportMode;
}

std::string IdfmLineStatus::getColor() const {
	return m_color;
}

std::string IdfmLineStatus::getId() const {
	return m_id;
}

std::string IdfmLineStatus::getName() const {
	return m_name;
}

std::string IdfmLineStatus::getSeverityColor() const {
	return m_severityColor;
}

std::string IdfmLineStatus::getSeverityEffect() const {
	return m_severityEffect;
}

std::string IdfmLineStatus::getTextColor() const {
	return m_textColor;
}
