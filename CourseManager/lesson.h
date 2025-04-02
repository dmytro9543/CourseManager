#pragma once
#include <string>
#include "lessonbase.h"

class Lesson : public LessonBase {
private:
	std::string groupNumber;
	std::string dayOfWeek;
	std::string startTime;
	int duration;
	std::string *teacherName;
	std::string classroom;
	static int instanceCount;

public:
	Lesson()
	{
		groupNumber = "";
		dayOfWeek = "";
		startTime = "00:00";
		duration = 1;
		teacherName = new std::string("");
		classroom = "";
		++instanceCount;
	}

	Lesson(const std::string& groupNumber, const std::string& dayOfWeek, const std::string& startTime, int duration, const std::string& teacherName, const std::string& classroom)
		: groupNumber(groupNumber), dayOfWeek(dayOfWeek), startTime(startTime), duration(duration), teacherName(new std::string(teacherName)), classroom(classroom)
	{
		++instanceCount;
	}

	~Lesson() {
		--instanceCount;
	}

	const std::string& getGroupNumber() const { return groupNumber; }
	const std::string& getDayOfWeek() const { return dayOfWeek; }
	const std::string& getStartTime() const { return startTime; }
	int getDuration() const { return duration; }
	const std::string& getTeacherName() const { return *teacherName; }
	const std::string& getClassroom() const { return classroom; }

	static int getInstanceCount() { return instanceCount; }

	void setGroup(const std::string& newGroup) {
		groupNumber = newGroup;
	}

	void setDay(const std::string& newDay) {
		dayOfWeek = newDay;
	}

	void setStartTime(const std::string& newStartTime) {
		startTime = newStartTime;
	}

	void setDuration(int newDuration) {
		duration = newDuration;
	}

	void setTeacher(const std::string& newTeacher) {
		*teacherName = newTeacher; // Dereference the pointer
	}

	void setClassroom(const std::string& newClassroom) {
		classroom = newClassroom;
	}

	void print(std::ostream& os) const override {
		os << "Group: " << groupNumber << ", Day: " << dayOfWeek << ", Start Time: " << startTime << ", Duration: " << duration
			<< ", Teacher: " << *teacherName << ", Classroom: " << classroom;
	}

	friend std::istream& operator>>(std::istream& is, Lesson& lesson);

};

int Lesson::instanceCount = 0;

std::ostream& operator<<(std::ostream& os, const Lesson& lesson) {
	lesson.print(os);
	return os;
}
std::istream& operator>>(std::istream& is, Lesson& lesson) {
	std::string strinput;
	std::cout << "Enter Group Number (2-digit): ";
	std::getline(is, lesson.groupNumber);
	std::cout << "Enter Day of the Week: ";
	std::getline(is, lesson.dayOfWeek);
	std::cout << "Enter Start Time(hh:mm): ";
	std::getline(is, lesson.startTime);
	std::cout << "Enter Duration: ";
	std::getline(is, strinput);
	try {
		lesson.duration = std::stoi(strinput);
	}
	catch (const std::invalid_argument& e) {
		lesson.duration = 1;
		std::cerr << "Invalid input: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		lesson.duration = 1;
		std::cerr << "Out of range: " << e.what() << std::endl;
	}
	std::cout << "Enter Teacher's Name: ";
	std::getline(is, *(lesson.teacherName));
	std::cout << "Enter Classroom: ";
	std::getline(is, lesson.classroom);
	return is;
}
