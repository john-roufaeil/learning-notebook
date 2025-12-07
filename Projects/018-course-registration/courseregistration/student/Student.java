package courseregistration.student;

import java.util.ArrayList;
import courseregistration.course;

public class Student {
    private static Integer studentsCount;
    private Integer studentId;
    private String studentName;    
    private ArrayList<Course> registrations;

    public Student(String studentName, ArrayList<Course> registrations) {
        this.studentId = studentsCount + 1;
        this.studentName = studentName;
        this.registrations = registrations;
        studentsCount++;
    }
    
    public Integer getStudentId() {
        return studentId;
    }

    public String getName() {
        return studentName;
    }

    public ArrayList<Courses> CourseRegistration() {
        return registrations;
    }

    public void registerCourse(Course course, Double grade) {

    }

    public void printReport() {
        StringBuilder sb = new StringBuilder();
    }
}

// This uses the inner class CourseRegistration.

// registerCourse(Course course, Double grade)
// Lets the student register for a course and optionally store a grade.
// Uses autoboxing with Double.

// printReport()
// Prints all the courses and grades of the student.
// This method should use a StringBuilder to build the formatted report.