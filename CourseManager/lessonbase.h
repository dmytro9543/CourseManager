#pragma once
// Abstract base class for lessons
class LessonBase {
public:
	virtual ~LessonBase() = default;
	virtual void print(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& os, const LessonBase& lesson);
};

std::ostream& operator<<(std::ostream& os, const LessonBase& lesson) {
	lesson.print(os);
	return os;
}

