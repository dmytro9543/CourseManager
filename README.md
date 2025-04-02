# CourseManager
Project Description: </br>
In this project, you are required to implement an application for managing courses. To do this, you 
need to prepare the following files:</br>
- Students.csv -</br>
This file will describe each student on a separate line with the following properties 
(separated by commas):</br>
• ID</br>
• Name</br>
• Password</br>
- Teachers.csv -</br>
This file will describe each teacher on a separate line with the following properties 
(separated by commas):</br>
• ID</br>
• Name</br>
- Courses.csv -</br>
This file will describe each course on a separate line with the following properties (separated 
by commas):</br>
• Unique 5-digit ID</br>
• Course Name</br>
• Responsible Teacher Name</br>
• Number of Credit Points</br>
For each course in the list, there will be 3 additional files:</br>
• xxxxx_lectures.csv (xxxxx describing the number of the course)</br>
• xxxxx_tutorials.csv (xxxxx describing the number of the course)</br>
• xxxxx_labs.csv (xxxxx describing the number of the course)</br>
In these files, each lesson (lecture, tutorial, or lab) will be described on a separate line with the 
following properties (separated by commas):</br>
• Group number (2-digit number). Each lesson (lecture, tutorial, or lab) will have a different 
number.</br>
• Day of the week</br>
• Start time.</br>
• Duration</br>
• Teacher's Name</br>
• Classroom (string)</br>
Note:</br>
• The final project consists of two sub-projects:</br>
1. A static library containing all the classes managing the data. </br>
2. Executable with user interface based on DOS, without without a graphical interface. </br>
Such an interface is called a Command Line Interface (CLI).</br>
• When entering the application for the first time, the user will be given the option to enter as 
an admin or a student. A password is required. The initial password for the admin will be </br>
"admin", and it can be changed after the first login. The student's password is shown in his 
record.</br>
Here are the actions that will be available to the user on the screen. Note: When a word appears in 
angle brackets, for example <unique_id>, the user should replace the word and the angle brackets. 
For example, to print details of a course with ID 1000, the user should enter: Print 1000.
All users (admin and student) will have the following actions available:</br>
• Print <course_id> - Print the details of the course to the screen.</br>
• PrintCourses - Print the first 10 courses in the course list. If there are fewer than 10 courses, 
only the existing courses will be printed.</br>
• More - Print the next 10 courses in the course list. Choosing this option again will print the 
next 10, and so on. If there are no more courses, a corresponding message will be printed to 
the screen. If there are fewer than 10 courses at any point, only the existing courses will be 
printed.</br>
For admins, the following actions will also be available:</br>
• AddCourse - Create a new course. </br>
After this command, the system will ask the user to enter the following details: </br>
o Course ID </br>
o Course Name </br>
o Responsible Teacher's Name </br>
o Number of Credit Points</br>
• RmCourse <course_id> - Delete a course based on its ID.</br>
• AddLecturer - Add a lecturer. </br>
After this command, the system will ask the user to enter the following details: </br>
o ID </br>
o Lecturer's Name</br>
• RmLecturer <lecturer_id> - Delete a lecturer based on their ID.</br>
• AddStudent - Add a student. </br>
After this command, the system will ask the user to enter the following details: </br>
o ID </br>
o Student's Name </br>
o Password</br>
• RmStudent <student_id> - Delete a student based on their ID.</br>
• AddLesson - Create a new lesson for an existing course. </br>
After this command, the system will ask the user to enter the following details:</br>
o Course ID</br>
o Lesson Type (lecture, tutorial, lab)</br>
o Group number (2-digit number). Each lesson (lecture, tutorial, or lab) will have a different 
number.</br>
o Day of the week</br>
o Start time</br>
o Duration</br>
o Teacher's Name</br>
o Classroom (string)</br>
• RmLesson - Delete an existing lesson. </br>
After this command, the system will ask the user to enter the following details:</br>
o Course ID</br>
o Group number</br>
• Search <search_text> - Search all records (courses, lecturers, students) for the search string. </br>
Print all search results, starting with courses, then lecturers, and finally students, with 
appropriate headings indicating whether they are courses, lecturers, or students.
At the end of the program, overwrite the files with the updated lists.</br>
For students, the following actions will also be available:</br>
• Schedule - Build a schedule. </br>
The student can build as many schedules as they want, numbered sequentially starting from </br>
1. After choosing this option, the user will receive a new menu of options:</br>
• Print <schedule_id> - Print the schedule according to its number. The schedule will 
be printed in a table format, with the days as rows and the hours as columns (in one hour resolution). For each course, the following details will be printed: course ID, 
lecture, tutorial, or lab, and classroom. There is no need to print the time and 
duration, as they will be described in the table margins. If a lesson lasts more than 
an hour, it should be printed in multiple rows according to its duration. If there is a 
conflict between courses, all of them should be printed.</br>
• PrintAll - Print all schedules created by the student, in the format described above 
for each schedule.</br>
• AddSchedule - Add an empty schedule. The number will be automatically 
determined to be the largest existing number plus 1.</br>
• RmSchedule <schedule_id> - Delete a schedule. After deleting the schedule, the 
identifying numbers will be condensed so that there are no gaps in the numbering.
• Add <schedule_id> <course_id> <group_id> - Add the course to the schedule (the 
group number can describe a lecture, tutorial, or lab).</br>
• Rm <schedule_id> <course_id> <group_id> - Delete a course from the schedule 
based on the course number and group number.</br>
• Search <course_id> - Print all records of the course (lectures, tutorials, and labs) 
clearly.</br>
• Back - Return to the main screen. </br>
• When you finish the program and log in again with the same student, the schedules you 
created will not be deleted. The Print command will print all the schedules the student has 
built so far. To do this, you must save the schedules in files. We recommend creating a file 
for each student, with the same name as the student's ID.</br>
Requirements for Project Implementation:</br>
• More than 3 classes. At least one of them has a pointer.</br>
• All classes have fields and get/set methods.</br>
• Use of const, initialization list, call by reference where needed.</br>
• Use of operator overloading where needed.</br>
• Use of >> operator for each class.</br>
• At least one class has a static variable and static methods.</br>
• Inheritance at least once.</br>
• Polymorphism and abstract class.</br>
• Use of STL for at least one of your data structures (note that all data structures should be 
efficient).</br>
• Use of iterator for the same data structure as STL.</br>
• Use of try-throw-catch where necessary.</br>
Prohibited:</br>
• Memory leaks</br>
• Runtime errors (note that you should handle illegal input).</br>
• Infinite loops</br>
• Code duplication.</br>
Note: File operations are slow. Access to files will be done once at the beginning of the program and 
once at the end. That's it.
