#pragma once
#include <string>

class Teacher {
private:
	std::string id;
	std::string *name;

public:
	Teacher(std::string& id, const std::string& name)
		: id(id), name(new std::string(name)) {}

	const std::string& getId() const {
		return id;
	}

	const std::string& getName() const {
		return *name;
	}

	void setId(const std::string& newId) {
		id = newId;
	}

	void setName(const std::string& newName) {
		*name = newName;
	}
};
