#include "IdfmTransportMode.h"

#include <string>

class IdfmLineStatus {
public:
	IdfmLineStatus(IdfmTransportMode transportMode,
		       std::string color,
		       std::string id,
		       std::string name,
		       std::string severityColor,
		       std::string severityEffect,
		       std::string textColor);

	void setSeverityColor(const std::string& severityColor);
	void setSeverityEffect(const std::string& severityEffect);
	void setTextColor(const std::string& textColor);

	IdfmTransportMode getTransportMode() const;
	std::string getColor() const;
	std::string getId() const;
	std::string getName() const;
	std::string getSeverityColor() const;
	std::string getSeverityEffect() const;
	std::string getTextColor() const;

private:
	const IdfmTransportMode m_transportMode;
	const std::string m_color;
	const std::string m_id;
	const std::string m_name;
	std::string m_severityColor;
	std::string m_severityEffect;
	std::string m_textColor;
};
