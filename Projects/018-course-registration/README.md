# Course Registration Console App

A console-based system where:
- The user can add students.
- Each student can register for multiple courses.
- The system prints reports using StringBuilder.
- Input for multiple courses is parsed using StringTokenizer.
- Use wrapper classes for IDs, grades, etc., to show autoboxing/unboxing.
- Use an interface for actions (e.g., registration).
- Use an inner class to represent each student’s course registration.

## Class: Student

### Attributes inside Student
- Integer studentId → The unique ID of the student (uses wrapper class).
- String name → The student’s full name.
- ArrayList<CourseRegistration> registrations → A list that contains all courses the student is registered in. This uses the inner class CourseRegistration.


### Methods inside Student

- registerCourse(Course course, Double grade)
    - Lets the student register for a course and optionally store a grade.
    - Uses autoboxing with Double.

- printReport()
    - Prints all the courses and grades of the student.
    - This method should use a StringBuilder to build the formatted report.

- getStudentId()
    - Returns the student ID.

- getName()
    - Returns the student’s name.

- getRegistrations()
    - Returns the list of CourseRegistration items.


## Inner Class: CourseRegistration

This class is inside Student, because it belongs only to a student.

### Attributes of CourseRegistration:
- Course course → The course that the student took.
- Double grade → The student’s grade in that course.

### Methods of CourseRegistration:
- getCourse() → Returns the course.
- getGrade() → Returns the grade.


## Class: Course
Represents a course that students can register in.
Attributes inside Course
 - Integer courseId → Unique ID of the course.
 - String courseName → Name of the course (e.g., OOP, Databases).
 - Integer creditHours → Number of credit hours.
Methods inside Course
 - getCourseId() → Returns courseID.
 - getCourseName () → Returns course name.
 - getCreditHours () → Returns credit hours.
