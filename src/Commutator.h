#ifndef COMMUTATOR_H
#define COMMUTATOR_H

#include <string>

class Commutator {
public:

	virtual ~Commutator() {}

	std::string getName() const {
		return m_name;
	}

protected:
	Commutator(std::string name) : m_name(name) {}
	
	const std::string m_name;
};

#endif // COMMUTATOR_H
