// CourseManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <csignal>

#include "teacher.h"
#include "student.h"
#include "lesson.h"
#include "course.h"
#include "utils.h"

#define PRINT_LIMIT 10

std::vector<Course> courses;
std::vector<Teacher> teachers;
std::vector<Student> students;

int is_changed = 0, is_changed_schedule = 0;
Student* currentStudent = nullptr;

std::vector<Teacher> readTeachersFromCSV(const std::string& filename) {
	std::vector<Teacher> teachers;
	std::ifstream file(filename);

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;

			// Split the line by commas
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// Assuming the format is: ID, Name
			if (tokens.size() == 2) {
				std::string id = tokens[0];
				std::string name = tokens[1];
				Teacher teacher(id, name);
				teachers.push_back(teacher);
			}
			else {
				std::cerr << "Invalid line format in Teachers.csv: " << line << std::endl;
			}
		}

		file.close();
	}
	else {
		//std::cerr << "Unable to open file: " << filename << std::endl;
	}

	return teachers;
}

std::vector<Student> readStudentsFromCSV(const std::string& filename) {
	std::vector<Student> students;
	std::ifstream file(filename);

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;

			// Split the line by commas
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// Assuming the format is: ID, Name, Password
			if (tokens.size() == 3) {
				std::string id = tokens[0];
				std::string name = tokens[1];
				std::string password = tokens[2];
				Student student(id, name, password);
				students.push_back(student);
			}
			else {
				std::cerr << "Invalid line format in Students.csv: " << line << std::endl;
			}
		}

		file.close();
	}
	else {
		//std::cerr << "Unable to open file: " << filename << std::endl;
	}

	return students;
}

void readLessonsFromCSV(const std::string& filename, std::vector<Lesson>& lessons) {
	std::ifstream file(filename);

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;

			// Split the line by commas
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// Assuming the format is: Group number, Day of the week, Start time, Duration, Teacher's Name, Classroom
			if (tokens.size() == 6) {
				std::string groupNumber = tokens[0];
				if (!isStringDigit(groupNumber)) {
					std::cout << "The Group number contains non-digit." << std::endl;
					return;
				}
				if (groupNumber.length() != 2) {
					std::cout << "The Group number should be 2-digit number." << std::endl;
					return;
				}

				std::string dayOfWeek = tokens[1];
				std::string startTime = tokens[2];
				int duration;
				try {
					duration = std::stoi(tokens[3]);
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid input: " << e.what() << std::endl;
					return;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
					return;
				}
				std::string teacherName = tokens[4];
				std::string classroom = tokens[5];

				Lesson lesson(groupNumber, dayOfWeek, startTime, duration, teacherName, classroom);
				lessons.push_back(lesson);

			}
			else {
				std::cerr << "Invalid line format in " << filename << ": " << line << std::endl;
			}
		}

		file.close();
	}
	else {
		//std::cerr << "Unable to open file: " << filename << std::endl;
	}
}

std::vector<Course> readCoursesFromCSV(const std::string& filename) {
	std::vector<Course> courses;
	std::ifstream file(filename);

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string token;
			std::vector<std::string> tokens;

			// Split the line by commas
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// Assuming the format is: Unique 5-digit ID, Course Name, Responsible Teacher Name, Number of Credit Points
			if (tokens.size() == 4) {
				int id = std::stoi(tokens[0]);
				std::string name = tokens[1];
				std::string responsibleTeacherName = tokens[2];
				int creditPoints = std::stoi(tokens[3]);

				Course course(id, name, responsibleTeacherName, creditPoints);

				// Read the associated lecture, tutorial, and lab files
				std::string filePrefix = getDigitString(id, 5);
				readLessonsFromCSV(filePrefix + "_lectures.csv", course.getLectures());
				readLessonsFromCSV(filePrefix + "_tutorials.csv", course.getTutorials());
				readLessonsFromCSV(filePrefix + "_labs.csv", course.getLabs());

				courses.push_back(course);
			}
			else {
				std::cerr << "Invalid line format in Courses.csv: " << line << std::endl;
			}
		}

		file.close();
	}
	else {
		//std::cerr << "Unable to open file: " << filename << std::endl;
	}

	return courses;
}

void changeAdminPassword() {
	// Code to change the admin password
	std::string newPassword;
	std::cout << "Enter new admin password: ";
	std::getline(std::cin, newPassword);

	// Store the new password securely (e.g., in an encrypted file or database)
	std::ofstream passwordFile("admin_password.txt");
	passwordFile << newPassword;
	passwordFile.close();

	std::cout << "Admin password changed successfully." << std::endl;

}

bool checkAdminPassword(const std::string& password) {
	// Check if the provided password matches the admin password
	bool isFirstLogin = true;
	std::string adminPassword = "admin"; // Initial admin password is "admin"
	std::ifstream passwordFile("admin_password.txt");
	if (passwordFile.is_open()) {
		std::getline(passwordFile, adminPassword);
		passwordFile.close();
		isFirstLogin = false;
	}

	if (password == adminPassword) {
		if (isFirstLogin) {
			char choice;
			do {
				std::cout << "1. Change admin password" << std::endl;
				std::cout << "2. Exit" << std::endl;
				std::cout << "Enter your choice: ";
				std::cin >> choice;
				std::cin.ignore(); // Ignore the newline character

				switch (choice) {
				case '1':
					changeAdminPassword();
					break;
				case '2':
					std::cout << "Exiting..." << std::endl;
					break;
				default:
					std::cout << "Invalid choice. Please try again." << std::endl;
				}
			} while (choice != '2');
		}
		return true;
	}
	else {
		return false;
	}
}

void printCourse(Course& course) {
	std::cout << "Course ID: " << getDigitString(course.getId(),5) << ", Name: " << course.getName() << ", Responsible Teacher: " << course.getResponsibleTeacherName() << ", Credit Points: " << course.getCreditPoints() << std::endl;

	std::cout << "Lectures:" << std::endl;
	for (Lesson& lecture : course.getLectures()) {
		//std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		std::cout << " " << lecture << std::endl;
	}

	std::cout << "Tutorials:" << std::endl;
	for (Lesson& lecture : course.getTutorials()) {
		//std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		std::cout << " " << lecture << std::endl;
	}

	std::cout << "Labs:" << std::endl;
	for (Lesson& lecture : course.getLabs()) {
		//std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		std::cout << " " << lecture << std::endl;
	}

	std::cout << std::endl;

}

void printCourses(std::vector<Course>& courses, int start, int count) {
	unsigned int end = start + count;
	for (unsigned int i = start; i < end && i < courses.size(); ++i) {
		printCourse(courses[i]);
	}
}

void loadData() {
	teachers = readTeachersFromCSV("Teachers.csv");

	// Print the teachers
	/*for (Teacher& teacher : teachers) {
		std::cout << "ID: " << teacher.getId() << ", Name: " << teacher.getName() << std::endl;
	}*/

	students = readStudentsFromCSV("Students.csv");

	// Print the students
	/*for (const Student& student : students) {
		std::cout << "ID: " << student.getId() << ", Name: " << student.getName() << ", Password: " << student.getPassword() << std::endl;
	}*/

	courses = readCoursesFromCSV("Courses.csv");

	// Print the courses and their associated lessons
	/*for (Course& course : courses) {
		std::cout << "Course ID: " << course.getId() << ", Name: " << course.getName() << ", Responsible Teacher: " << course.getResponsibleTeacherName() << ", Credit Points: " << course.getCreditPoints() << std::endl;

		std::cout << "Lectures:" << std::endl;
		for (Lesson& lecture : course.getLectures()) {
			std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		}

		std::cout << "Tutorials:" << std::endl;
		for (Lesson& lecture : course.getTutorials()) {
			std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		}

		std::cout << "Labs:" << std::endl;
		for (Lesson& lecture : course.getTutorials()) {
			std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
		}

		std::cout << std::endl;

		// Print tutorials and labs in a similar manner
		// ...
	}*/
}

void saveData() {
	// Save courses to Courses.csv
	if (!is_changed)
		return;

	std::ofstream coursesFile("Courses.csv");
	if (coursesFile.is_open()) {
		for (Course& course : courses) {
			coursesFile << getDigitString(course.getId(), 5) << "," << course.getName() << "," << course.getResponsibleTeacherName() << "," << course.getCreditPoints() << std::endl;

			// Save lectures to xxxxx_lectures.csv
			std::string digitString = getDigitString(course.getId(), 5);
			std::string lecturesFilename = digitString + "_lectures.csv";
			std::ofstream lecturesFile(lecturesFilename);
			if (lecturesFile.is_open()) {
				for (Lesson& lecture : course.getLectures()) {
					lecturesFile << lecture.getGroupNumber() << "," << lecture.getDayOfWeek() << "," << lecture.getStartTime() << "," << lecture.getDuration() << "," << lecture.getTeacherName() << "," << lecture.getClassroom() << std::endl;
				}
				lecturesFile.close();
			}

			// Save tutorials to xxxxx_tutorials.csv
			std::string tutorialsFilename = digitString + "_tutorials.csv";
			std::ofstream tutorialsFile(tutorialsFilename);
			if (tutorialsFile.is_open()) {
				for (Lesson& tutorial : course.getTutorials()) {
					tutorialsFile << tutorial.getGroupNumber() << "," << tutorial.getDayOfWeek() << "," << tutorial.getStartTime() << "," << tutorial.getDuration() << "," << tutorial.getTeacherName() << "," << tutorial.getClassroom() << std::endl;
				}
				tutorialsFile.close();
			}

			// Save labs to xxxxx_labs.csv
			std::string labsFilename = digitString + "_labs.csv";
			std::ofstream labsFile(labsFilename);
			if (labsFile.is_open()) {
				for (Lesson& lab : course.getLabs()) {
					labsFile << lab.getGroupNumber() << "," << lab.getDayOfWeek() << "," << lab.getStartTime() << "," << lab.getDuration() << "," << lab.getTeacherName() << "," << lab.getClassroom() << std::endl;
				}
				labsFile.close();
			}
		}
		coursesFile.close();
	}

	// Save teachers to Teachers.csv
	std::ofstream teachersFile("Teachers.csv");
	if (teachersFile.is_open()) {
		for (Teacher& teacher : teachers) {
			teachersFile << teacher.getId() << "," << teacher.getName() << std::endl;
		}
		teachersFile.close();
	}

	// Save students to Students.csv
	std::ofstream studentsFile("Students.csv");
	if (studentsFile.is_open()) {
		for (Student& student : students) {
			studentsFile << student.getId() << "," << student.getName() << "," << student.getPassword() << std::endl;
		}
		studentsFile.close();
	}
}


void addCourse() {
	int id;
	std::string strinput, name, responsibleTeacherName;
	int creditPoints;

	std::cout << "Enter Course ID: ";
	std::getline(std::cin, strinput);
	try {
		id = std::stoi(strinput);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid input: " << e.what() << std::endl;
		return;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
		return;
	}

	std::cout << "Enter Course Name: ";
	std::getline(std::cin, name);

	std::cout << "Enter Responsible Teacher's Name: ";
	std::getline(std::cin, responsibleTeacherName);

	std::cout << "Enter Number of Credit Points: ";
	std::getline(std::cin, strinput);
	try {
		creditPoints = std::stoi(strinput);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid input: " << e.what() << std::endl;
		return;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
		return;
	}

	Course course(id, name, responsibleTeacherName, creditPoints);
	courses.push_back(course);
}

void rmCourse(int courseId) {
	auto it = std::find_if(courses.begin(), courses.end(), [courseId](const Course& course) {
		return course.getId() == courseId;
		});

	if (it != courses.end()) {
		courses.erase(it);
		std::cout << "Course with ID " << courseId << " has been removed." << std::endl;
	}
	else {
		std::cout << "Course with ID " << courseId << " not found." << std::endl;
	}
}

void addTeacher() {
	std::string id;
	std::string name;

	std::cout << "Enter Teacher ID: ";
	std::getline(std::cin, id);
	
	std::cout << "Enter Teacher's Name: ";
	std::getline(std::cin, name);

	Teacher teacher(id, name);
	teachers.push_back(teacher);
}

void rmTeacher(std::string& teacherId) {
	auto it = std::find_if(teachers.begin(), teachers.end(), [teacherId](const Teacher& teacher) {
		return teacher.getId() == teacherId;
		});

	if (it != teachers.end()) {
		teachers.erase(it);
		std::cout << "Teacher with ID " << teacherId << " has been removed." << std::endl;
	}
	else {
		std::cout << "Teacher with ID " << teacherId << " not found." << std::endl;
	}
}

void addStudent() {
	Student student;
	std::cout << "Enter student details:" << std::endl;
	std::cin >> student;

	students.push_back(student);
}

void rmStudent(std::string& studentId) {
	auto it = std::find_if(students.begin(), students.end(), [studentId](const Student& student) {
		return student.getId() == studentId;
		});

	if (it != students.end()) {
		students.erase(it);
		std::cout << "Student with ID " << studentId << " has been removed." << std::endl;
	}
	else {
		std::cout << "Student with ID " << studentId << " not found." << std::endl;
	}
}

void addLesson() {
	int courseId; //, groupNumber, duration;
	std::string strinput, lessonType, dayOfWeek, startTime, teacherName, classroom;

	std::cout << "Enter Course ID: ";
	std::getline(std::cin, strinput);
	if (!isStringDigit(strinput)) {
		std::cout << "The Group number contains non-digit." << std::endl;
		return;
	}
	if (strinput.length() != 5) {
		std::cout << "The Course ID should be 5-digit number." << std::endl;
		return;
	}
	courseId = std::stoi(strinput);

	std::cout << "Enter Lesson Type (lecture, tutorial, lab): ";
	std::getline(std::cin, lessonType);

	Lesson lesson;
	std::cout << "Enter lesson details:" << std::endl;
	std::cin >> lesson;

	// Add the lesson to the appropriate course and vector (lectures, tutorials, or labs)
	for (Course& course : courses) {
		if (course.getId() == courseId) {
			if (lessonType == "lecture") {
				course.addLecture(lesson);
			}
			else if (lessonType == "tutorial") {
				course.addTutorial(lesson);
			}
			else if (lessonType == "lab") {
				course.addLab(lesson);
			}
			break;
		}
	}
}

void rmLesson() {
	int courseId;
	std::string strinput, groupNumber;

	std::cout << "Enter Course ID: ";
	std::getline(std::cin, strinput);
	if (!isStringDigit(groupNumber)) {
		std::cout << "The Group number contains non-digit." << std::endl;
		return;
	}
	if (groupNumber.length() != 2) {
		std::cout << "The Group number should be 2-digit number." << std::endl;
		return;
	}
	courseId = std::stoi(strinput);

	std::cout << "Enter Group Number: ";
	std::getline(std::cin, groupNumber);
	if (!isStringDigit(groupNumber)) {
		std::cout << "The Group number contains non-digit." << std::endl;
		return;
	}
	if (groupNumber.length() != 2) {
		std::cout << "The Group number should be 2-digit number." << std::endl;
		return;
	}

	// Find the course with the given courseId
	auto courseIt = std::find_if(courses.begin(), courses.end(), [courseId](const Course& course) {
		return course.getId() == courseId;
		});

	if (courseIt != courses.end()) {
		// Remove the lesson with the given groupNumber from the lectures vector
		auto lectureIt = std::find_if(courseIt->getLectures().begin(), courseIt->getLectures().end(), [groupNumber](const Lesson& lesson) {
			return lesson.getGroupNumber() == groupNumber;
			});
		if (lectureIt != courseIt->getLectures().end()) {
			courseIt->getLectures().erase(lectureIt);
			std::cout << "Lecture with Group Number " << groupNumber << " has been removed from Course ID " << courseId << "." << std::endl;
		}
		else {
			std::cout << "Lecture with Group Number " << groupNumber << " not found in Course ID " << courseId << "." << std::endl;
		}

		// Remove the lesson with the given groupNumber from the tutorials vector
		auto tutorialIt = std::find_if(courseIt->getTutorials().begin(), courseIt->getTutorials().end(), [groupNumber](const Lesson& lesson) {
			return lesson.getGroupNumber() == groupNumber;
			});
		if (tutorialIt != courseIt->getTutorials().end()) {
			courseIt->getTutorials().erase(tutorialIt);
			std::cout << "Tutorial with Group Number " << groupNumber << " has been removed from Course ID " << courseId << "." << std::endl;
		}
		else {
			std::cout << "Tutorial with Group Number " << groupNumber << " not found in Course ID " << courseId << "." << std::endl;
		}

		// Remove the lesson with the given groupNumber from the labs vector
		auto labIt = std::find_if(courseIt->getLabs().begin(), courseIt->getLabs().end(), [groupNumber](const Lesson& lesson) {
			return lesson.getGroupNumber() == groupNumber;
			});
		if (labIt != courseIt->getLabs().end()) {
			courseIt->getLabs().erase(labIt);
			std::cout << "Lab with Group Number " << groupNumber << " has been removed from Course ID " << courseId << "." << std::endl;
		}
		else {
			std::cout << "Lab with Group Number " << groupNumber << " not found in Course ID " << courseId << "." << std::endl;
		}
	}
	else {
		std::cout << "Course with ID " << courseId << " not found." << std::endl;
	}
}

void search(const std::string& searchText) {
	std::cout << "Courses:" << std::endl;
	for (const Course& course : courses) {
		if (course.getName().find(searchText) != std::string::npos ||
			course.getResponsibleTeacherName().find(searchText) != std::string::npos) {
			std::cout << "  " << course.getId() << ": " << course.getName() << std::endl;
		}
	}

	std::cout << "Teachers:" << std::endl;
	for (const Teacher& teacher : teachers) {
		if (teacher.getName().find(searchText) != std::string::npos) {
			std::cout << "  " << teacher.getId() << ": " << teacher.getName() << std::endl;
		}
	}

	std::cout << "Students:" << std::endl;
	for (const Student& student : students) {
		if (student.getName().find(searchText) != std::string::npos) {
			std::cout << "  " << student.getId() << ": " << student.getName() << std::endl;
		}
	}
}

void saveSchedules(const Student* student) {
	if (student == nullptr || !is_changed_schedule)
		return;

	std::ofstream file(student->getId() + ".txt");
	if (file.is_open()) {
		for (const auto& schedule : student->getSchedules()) {
			file << "Schedule " << schedule.first << ":" << std::endl;
			for (const auto& lesson : schedule.second) {
				file << getDigitString(lesson.first, 5) << "," << lesson.second << std::endl;
			}
			file << std::endl;
		}
		file.close();
	}
}

int loadSchedules(Student& student) {
	std::ifstream file(student.getId() + ".txt");
	int scheduleId = 1;
	if (file.is_open()) {
		std::string line;
		std::map<int, std::vector<std::pair<int, std::string>>> schedules;
		std::vector<std::pair<int, std::string>> currentSchedule;

		while (std::getline(file, line)) {
			if (line.find("Schedule") != std::string::npos) {
				if (!currentSchedule.empty()) {
					schedules[scheduleId++] = currentSchedule;
					currentSchedule.clear();
				}
			}
			else if (!line.empty()) {
				std::istringstream iss(line);
				std::string token;
				std::vector<std::string> tokens;

				while (std::getline(iss, token, ',')) {
					tokens.push_back(token);
				}

				if (tokens.size() == 2) {
					int courseId = std::stoi(tokens[0]);
					std::string groupNumber = tokens[1];
					currentSchedule.emplace_back(courseId, groupNumber);
				}
			}
		}

		if (!currentSchedule.empty()) {
			schedules[scheduleId] = currentSchedule;
		}

		student.setSchedules(schedules);
		file.close();
	}

	return scheduleId;
}

void printSchedule(const std::map<int, std::vector<std::pair<int, std::string>>>& schedules, int scheduleId) {
	if (schedules.find(scheduleId) != schedules.end()) {
		std::cout << "Schedule " << scheduleId << ":" << std::endl;

		const std::vector<std::string> days = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
		const int numHours = 24;

		// Create a 2D vector to store the schedule
		std::vector<std::vector<std::string>> coursestr(numHours * 10, std::vector<std::string>(days.size(), ""));
		std::vector<std::vector<std::string>> lessonstr(numHours * 10, std::vector<std::string>(days.size(), ""));
		std::vector<std::vector<std::string>> classroomstr(numHours * 10, std::vector<std::string>(days.size(), ""));

		for (int hour = 0; hour < numHours*10; ++hour) {
			for (int day = 0; day < (int)days.size(); ++day) {
				coursestr[hour][day] = "";
				lessonstr[hour][day] = "";
				classroomstr[hour][day] = "";
			}
		}

		for (const auto& lesson : schedules.at(scheduleId)) {
			int courseId = lesson.first;
			std::string groupNumber = lesson.second;

			for (Course& course : courses) {
				if (course.getId() == courseId) {
					for (const Lesson& lecture : course.getLectures()) {
						if (lecture.getGroupNumber() == groupNumber) {
							int startHour = std::stoi(lecture.getStartTime().substr(0, 2));
							int duration = lecture.getDuration();
							int dayIndex = std::distance(days.begin(), std::find(days.begin(), days.end(), lecture.getDayOfWeek()));
							if (dayIndex >= 7)
								break;
							for (int i = 0; i < duration; ++i) {
								int j = 0;
								while (coursestr[(startHour + i) * 10 + j][dayIndex] != "" && j < 9)
									j++;
								coursestr[(startHour + i) * 10 + j][dayIndex] = getDigitString(courseId, 5);
								lessonstr[(startHour + i) * 10 + j][dayIndex] = "Lecture";
								classroomstr[(startHour + i) * 10 + j][dayIndex] =lecture.getClassroom();
							}
						}
					}
					for (const Lesson& tutorial : course.getTutorials()) {
						if (tutorial.getGroupNumber() == groupNumber) {
							int startHour = std::stoi(tutorial.getStartTime().substr(0, 2));
							int duration = tutorial.getDuration();
							int dayIndex = std::distance(days.begin(), std::find(days.begin(), days.end(), tutorial.getDayOfWeek()));
							if (dayIndex >= 7)
								break;
							for (int i = 0; i < duration; ++i) {
								int j = 0;
								while (coursestr[(startHour + i) * 10 + j][dayIndex] != "" && j < 9)
									j++;
								coursestr[(startHour + i) * 10 + j][dayIndex] = getDigitString(courseId, 5);
								lessonstr[(startHour + i) * 10 + j][dayIndex] = "Tutorial";
								classroomstr[(startHour + i) * 10 + j][dayIndex] = tutorial.getClassroom();
							}
						}
					}
					for (const Lesson& lab : course.getLabs()) {
						if (lab.getGroupNumber() == groupNumber) {
							int startHour = std::stoi(lab.getStartTime().substr(0, 2));
							int duration = lab.getDuration();
							int dayIndex = std::distance(days.begin(), std::find(days.begin(), days.end(), lab.getDayOfWeek()));
							if (dayIndex >= 7)
								break;
							for (int i = 0; i < duration; ++i) {
								int j = 0;
								while (coursestr[(startHour + i) * 10 + j][dayIndex] != "" && j < 9)
									j++;
								coursestr[(startHour + i) * 10 + j][dayIndex] = getDigitString(courseId, 5);
								lessonstr[(startHour + i) * 10 + j][dayIndex] = "Lab";
								classroomstr[(startHour + i) * 10 + j][dayIndex] = lab.getClassroom();
							}
						}
					}
					break;
				}
			}
		}

		// Print the schedule
		std::cout << std::endl;

		std::cout << std::setw(8) << " ";
		for (const auto& day : days) {
			std::cout << std::setw(15) << day;
		}
		std::cout << std::endl;

		for (int hour = 0; hour < numHours; ++hour) {
			int isprint;
			for (int j = 0; j < 10; j++) {
				isprint = 0;
				for (int day = 0; day < (int)days.size(); ++day) {
					if (coursestr[hour * 10 + j][day] != "") {
						isprint = 1;
						break;
					}
				}
				if (isprint) {
					std::cout << std::endl;
					if(j == 0)
						std::cout << std::setw(5) << hour << ":00";
					else
						std::cout << std::setw(8) << " ";

					for (int day = 0; day < (int)days.size(); ++day) {
						std::cout << std::setw(15) << coursestr[hour * 10 + j][day];
					}
					std::cout << std::endl;

					std::cout << std::setw(8) << " ";
					for (int day = 0; day < (int)days.size(); ++day) {
						std::cout << std::setw(15) << lessonstr[hour * 10 + j][day];
					}
					std::cout << std::endl;

					std::cout << std::setw(8) << " ";
					for (int day = 0; day < (int)days.size(); ++day) {
						std::cout << std::setw(15) << classroomstr[hour * 10 + j][day];
					}
					std::cout << std::endl;
				}
			}
			if (isprint)
				std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "Schedule " << scheduleId << " not found." << std::endl;
	}
}

void studentMenu(Student& student) {
	std::string userInput, strinput;
	int scheduleId;
	/*if (student.getSchedules().empty()) {
		student.addSchedule(scheduleId++);
	}*/
	scheduleId = loadSchedules(student);

	while (true) {
		std::cout << "Enter an action (Print, PrintAll, AddSchedule, RmSchedule, Add, Rm, Search, Back): ";
		std::getline(std::cin, userInput);

		if (userInput == "Print") {
			int scheduleId;
			std::cout << "Enter Schedule ID: ";
			std::getline(std::cin, strinput);
			try {
				scheduleId = std::stoi(strinput);
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
				return;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "Out of range: " << e.what() << std::endl;
				return;
			}
			printSchedule(student.getSchedules(), scheduleId);
		}
		else if (userInput == "PrintAll") {
			for (const auto& schedule : student.getSchedules()) {
				printSchedule(student.getSchedules(), schedule.first);
			}
		}
		else if (userInput == "AddSchedule") {
			student.addSchedule(scheduleId++);
			is_changed_schedule = 1;
		}
		else if (userInput == "RmSchedule") {
			int scheduleId;
			std::cout << "Enter Schedule ID: ";
			std::getline(std::cin, strinput);
			try {
				scheduleId = std::stoi(strinput);
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid input: " << e.what() << std::endl;
				return;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "Out of range: " << e.what() << std::endl;
				return;
			}
			student.removeSchedule(scheduleId);
			is_changed_schedule = 1;
		}
		else if (userInput.substr(0, 4) == "Add ") {
			std::istringstream iss(userInput.substr(4));
			std::string token;
			std::vector<std::string> tokens;

			while (std::getline(iss, token, ' ')) {
				tokens.push_back(token);
			}

			if (tokens.size() == 3) {
				int scheduleId = std::stoi(tokens[0]);
				int courseId = std::stoi(tokens[1]);
				std::string groupNumber = tokens[2];

				student.addLessonToSchedule(scheduleId, courseId, groupNumber);
				is_changed_schedule = 1;
			}
			else {
				std::cout << "Invalid input format for Add command." << std::endl;
			}
		}
		else if (userInput.substr(0, 3) == "Rm ") {
			std::istringstream iss(userInput.substr(3));
			std::string token;
			std::vector<std::string> tokens;

			while (std::getline(iss, token, ' ')) {
				tokens.push_back(token);
			}

			if (tokens.size() == 3) {
				int scheduleId = std::stoi(tokens[0]);
				int courseId = std::stoi(tokens[1]);
				std::string groupNumber = tokens[2];

				student.removeLessonFromSchedule(scheduleId, courseId, groupNumber);
				is_changed_schedule = 1;
			}
			else {
				std::cout << "Invalid input format for Rm command." << std::endl;
			}
		}
		else if (userInput.substr(0, 7) == "Search ") {
			int courseId = std::stoi(userInput.substr(7));
			for (Course& course : courses) {
				if (course.getId() == courseId) {
					std::cout << "Course ID: " << course.getId() << ", Name: " << course.getName() << ", Responsible Teacher: " << course.getResponsibleTeacherName() << ", Credit Points: " << course.getCreditPoints() << std::endl;

					std::cout << "Lectures:" << std::endl;
					for (Lesson& lecture : course.getLectures()) {
						std::cout << "  Group: " << lecture.getGroupNumber() << ", Day: " << lecture.getDayOfWeek() << ", Start Time: " << lecture.getStartTime() << ", Duration: " << lecture.getDuration() << ", Teacher: " << lecture.getTeacherName() << ", Classroom: " << lecture.getClassroom() << std::endl;
					}

					std::cout << "Tutorials:" << std::endl;
					for (Lesson& tutorial : course.getTutorials()) {
						std::cout << "  Group: " << tutorial.getGroupNumber() << ", Day: " << tutorial.getDayOfWeek() << ", Start Time: " << tutorial.getStartTime() << ", Duration: " << tutorial.getDuration() << ", Teacher: " << tutorial.getTeacherName() << ", Classroom: " << tutorial.getClassroom() << std::endl;
					}

					std::cout << "Labs:" << std::endl;
					for (Lesson& lab : course.getLabs()) {
						std::cout << "  Group: " << lab.getGroupNumber() << ", Day: " << lab.getDayOfWeek() << ", Start Time: " << lab.getStartTime() << ", Duration: " << lab.getDuration() << ", Teacher: " << lab.getTeacherName() << ", Classroom: " << lab.getClassroom() << std::endl;
					}
					break;
				}
			}
		}
		else if (userInput == "Back") {
			saveSchedules(currentStudent);
			break;
		}
		else {
			std::cout << "Invalid action!" << std::endl;
		}
	}
}


void signalHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << "Interrupt signal (Ctrl+C) received. Overwriting files..." << std::endl;
		saveData();
		saveSchedules(currentStudent);
		std::exit(0); // Exit the program
	}
}

int main()
{
	signal(SIGINT, signalHandler);

    std::cout << "Hello World!\n";

	loadData();

	std::cout << "Number of student instances: " << Student::getInstanceCount() << std::endl;

	std::string userInput;
	bool isAdmin = true;
	std::string studentId;

	std::cout << "Welcome to the Course Management Application!" << std::endl;
	std::cout << "Enter 'admin' to login as an admin or 'student's Id' to login as a student: ";
	std::getline(std::cin, userInput);

	if (userInput == "admin") {
		std::string password;
		std::cout << "Enter the admin password: ";
		std::getline(std::cin, password);

		if (checkAdminPassword(password)) {
			std::cout << "Admin access granted!" << std::endl;
			// Admin functionality goes here
		}
		else {
			std::cout << "Invalid admin password!" << std::endl;
			return 0;
		}

		isAdmin = true;
	}
	else {
		studentId = userInput;

		for (Student& student : students) {
			if (student.getId() == studentId) {
				currentStudent = &student;
				break;
			}
		}

		if (currentStudent != nullptr) {
			std::string password;
			std::cout << "Enter your password: ";
			std::getline(std::cin, password);

			if (password == currentStudent->getPassword()) {
				std::cout << "Student access granted!" << std::endl;
				// Student functionality goes here
			}
			else {
				std::cout << "Invalid student password!" << std::endl;
				return 0;
			}
		}
		else {
			std::cout << "Invalid student ID!" << std::endl;
			return 0;
		}

		isAdmin = false;
	}


	int currentIndex = 0;

	
	while (true) {
		if (isAdmin) {
			std::cout << "Enter an action (AddCourse, RmCourse, AddLecturer, RmLecturer, AddStudent, RmStudent, AddLesson, RmLesson, Search, Print, PrintCourses, More): ";
			std::getline(std::cin, userInput);

			if (userInput == "AddCourse") {
				addCourse();
				is_changed = 1;
			}
			else if (userInput == "RmCourse") {
				int courseId;
				std::string strinput;
				std::cout << "Enter Course ID: ";
				std::getline(std::cin, strinput);
				try {
					courseId = std::stoi(strinput);
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid input: " << e.what() << std::endl;
					break;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
					break;
				}
				rmCourse(courseId);
				is_changed = 1;
			}
			else if (userInput == "AddLecturer") {
				addTeacher();
				is_changed = 1;
			}
			else if (userInput == "RmLecturer") {
				std::string teacherId;
				std::cout << "Enter Teacher ID: ";
				std::getline(std::cin, teacherId);
				rmTeacher(teacherId);
				is_changed = 1;
			}
			else if (userInput == "AddStudent") {
				addStudent();
				is_changed = 1;
			}
			else if (userInput == "RmStudent") {
				std::string studentId;
				std::cout << "Enter Student ID: ";
				std::getline(std::cin, studentId);
				rmStudent(studentId);
				is_changed = 1;
			}
			else if (userInput == "AddLesson") {
				addLesson();
				is_changed = 1;
			}
			else if (userInput == "RmLesson") {
				rmLesson();
				is_changed = 1;
			}
			else if (userInput.substr(0, 7) == "Search ") {
				std::string searchText = userInput.substr(7);
				search(searchText);
			}
			else if (userInput == "PrintCourses") {
				currentIndex = 0;
				printCourses(courses, currentIndex, PRINT_LIMIT);
				currentIndex += PRINT_LIMIT;
			}
			else if (userInput == "More") {
				printCourses(courses, currentIndex, PRINT_LIMIT);
				currentIndex += PRINT_LIMIT;
			}
			else if (userInput.substr(0, 6) == "Print ") {
				int courseId = 0;
				try {
					courseId = std::stoi(userInput.substr(6));
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid input: " << e.what() << std::endl;
					continue;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
					continue;
				}

				for (Course& course : courses) {
					if (course.getId() == courseId) {
						printCourse(course);
						break;
					}
				}
			}
			else if (userInput == "noinput") {
				
			}
			else {
				std::cout << "Invalid action!" << std::endl;
			}
		}
		else {
			std::cout << "Enter an action (Schedule, Print, PrintCourses, More): ";
			std::getline(std::cin, userInput);

			if (userInput == "PrintCourses") {
				printCourses(courses, currentIndex, PRINT_LIMIT);
				currentIndex += PRINT_LIMIT;
			}
			else if (userInput == "More") {
				printCourses(courses, currentIndex, PRINT_LIMIT);
				currentIndex += PRINT_LIMIT;
			}
			else if (userInput.substr(0, 6) == "Print ") {
				int courseId = 0;
				try {
					courseId = std::stoi(userInput.substr(6));
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid input: " << e.what() << std::endl;
					continue;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
					continue;
				}

				for (Course& course : courses) {
					if (course.getId() == courseId) {
						printCourse(course);
						break;
					}
				}
			}
			else if (userInput == "Schedule") {
				if (currentStudent != nullptr) {
					studentMenu(*currentStudent);
				}
			}
			else if (userInput == "noinput") {
				
			}
			else {
				std::cout << "Invalid student ID!" << std::endl;
			}
		}

		userInput = "noinput";
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
