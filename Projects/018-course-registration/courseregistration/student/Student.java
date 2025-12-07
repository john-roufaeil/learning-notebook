package courseregistration.student;

import courseregistration.course.Course;
import java.util.ArrayList;

public class Student {
    private static Integer studentsCount = 0;
    private Integer studentId;
    private String studentName;    
    private ArrayList<CourseRegistration> registrations;

    public Student(String studentName) {
        this.studentId = studentsCount + 1;
        this.studentName = studentName;
        this.registrations = new ArrayList<>();
        studentsCount++;
    }
    
    public Integer getStudentId() {
        return studentId;
    }

    public String getName() {
        return studentName;
    }

    public ArrayList<CourseRegistration> getRegistrations() {
        return registrations;
    }

    public void registerCourse(Course course, double grade) {
        Double gradeWrapper = grade;
        registrations.add(new CourseRegistration(course, gradeWrapper));
    }

    public void printReport() {
        StringBuilder sb = new StringBuilder("Student " + getStudentId() + ": " + getName() + "\n");
        for (int i = 0; i < registrations.size(); i++) {
            Course curr = registrations.get(i).getCourse();
            sb.append("Course ").append(curr.getCourseId()).append(": ").append(curr.getCourseName()).append(" (").append(curr.getCreditHours()).append("h) -> ").append(registrations.get(i).getGrade()).append("\n");
        }
        sb.append("----------\n");
        System.out.println(sb.toString());
    }

    class CourseRegistration {
        private Course course;
        private Double grade;

        public CourseRegistration(Course course, Double grade) {
            this.course = course;
            this.grade = grade;
        }

        public Course getCourse() {
            return course;
        }
        public Double getGrade() {
            return grade;
        }
    }
}
