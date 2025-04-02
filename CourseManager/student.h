#pragma once
#include <string>
#include <map>
#include "lesson.h"

class Student {
private:
	std::string id;
	std::string name;
	std::string password;
	std::map<int, std::vector<std::pair<int, std::string>>> schedules;
	static int instanceCount;

public:
	Student()
	{
		id = "";
		name = "";
		password = "";
		++instanceCount;
	}

	Student(const std::string& id, const std::string& name, const std::string& password)
		: id(id), name(name), password(password) 
	{
		++instanceCount;
	}

	const std::string& getId() const {
		return id;
	}

	const std::string& getName() const {
		return name;
	}

	const std::string& getPassword() const {
		return password;
	}

	static int getInstanceCount() { return instanceCount; }
	static void decreaseInstanceCount() { --instanceCount; }

	void setId(const std::string& newId) {
		id = newId;
	}

	void setName(const std::string& newName) {
		name = newName;
	}

	void setPassword(const std::string& newPassword) {
		password = newPassword;
	}

	const std::map<int, std::vector<std::pair<int, std::string>>>& getSchedules() const { return schedules; } // Change this line
	void setSchedules(const std::map<int, std::vector<std::pair<int, std::string>>>& schedules) { this->schedules = schedules; }

	void addSchedule(int scheduleId) {
		schedules[scheduleId] = std::vector<std::pair<int, std::string>>(); // Change this line
	}

	void removeSchedule(int scheduleId) {
		auto it = schedules.find(scheduleId);
		if (it != schedules.end()) {
			schedules.erase(it);
			std::map<int, std::vector<std::pair<int, std::string>>> newSchedules; // Change this line
			int newId = 1;
			for (const auto& schedule : schedules) {
				newSchedules[newId++] = schedule.second;
			}
			schedules = std::move(newSchedules);
		}
	}

	void addLessonToSchedule(int scheduleId, int courseId, std::string groupNumber) {
		auto it = schedules.find(scheduleId);
		if (it != schedules.end()) {
			it->second.emplace_back(courseId, groupNumber); // Change this line
		}
	}

	void removeLessonFromSchedule(int scheduleId, int courseId, std::string groupNumber) {
		auto it = schedules.find(scheduleId);
		if (it != schedules.end()) {
			auto lessonIt = std::find_if(it->second.begin(), it->second.end(), [courseId, groupNumber](const std::pair<int, std::string>& lesson) { // Change this line
				return lesson.first == courseId && lesson.second == groupNumber;
				});
			if (lessonIt != it->second.end()) {
				it->second.erase(lessonIt);
			}
		}
	}
};

int Student::instanceCount = 0;

std::ostream& operator<<(std::ostream& os, const Student& student) {
	os << "Student ID: " << student.getId() << ", Name: " << student.getName();
	return os;
}

std::istream& operator>>(std::istream& is, Student& student) {
	std::string id;
	std::string name, password;

	std::cout << "Enter Student ID: ";
	std::getline(is, id);
	std::cout << "Enter Student's Name: ";
	std::getline(is, name);
	std::cout << "Enter Password: ";
	std::getline(is, password);

	student = Student(id, name, password);
	return is;
}
