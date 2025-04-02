#pragma once
#include <string>
#include <vector>
#include "lesson.h"
#include "utils.h"

class Course {
private:
	int id;
	std::string name;
	std::string *responsibleTeacherName;
	int creditPoints;
	std::vector<Lesson> lectures;
	std::vector<Lesson> tutorials;
	std::vector<Lesson> labs;

public:
	Course(int id, const std::string& name, const std::string& responsibleTeacherName, int creditPoints)
		: id(id), name(name), responsibleTeacherName(new std::string(responsibleTeacherName)), creditPoints(creditPoints) {}

	int getId() const { return id; }
	const std::string& get5DigitId() const { return getDigitString(id, 5); }
	const std::string& getName() const { return name; }
	const std::string& getResponsibleTeacherName() const { return *responsibleTeacherName; }
	int getCreditPoints() const { return creditPoints; }

	std::vector<Lesson>& getLectures() { return lectures; }
	std::vector<Lesson>& getTutorials() { return tutorials; }
	std::vector<Lesson>& getLabs() { return labs; }

	void setCourseId(int id) {
		id = id;
	}

	void setCourseName(const std::string& name) {
		this->name = name;
	}
	
	void setTeacher(const std::string& newTeacher) {
		*responsibleTeacherName = newTeacher; // Dereference the pointer
	}

	void setCredits(int credits) {
		creditPoints = credits;
	}

	void addLecture(const Lesson& lecture) { lectures.push_back(lecture); }
	void addTutorial(const Lesson& tutorial) { tutorials.push_back(tutorial); }
	void addLab(const Lesson& lab) { labs.push_back(lab); }
};
