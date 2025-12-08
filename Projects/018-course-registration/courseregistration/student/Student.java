package courseregistration.student;

import courseregistration.course.Course;
import java.util.ArrayList;

public class Student {
    private static Integer studentsCount = 0;
    private final Integer studentId;
    private String studentName;    
    private ArrayList<CourseRegistration> registrations;

    public Student(String studentName) {
        this.studentId = studentsCount + 1;
        this.studentName = studentName;
        this.registrations = new ArrayList<>();
        studentsCount++;
    }

    public static Integer getStudentsCount() {
        return studentsCount;
    }
    
    public Integer getStudentId() {
        return studentId;
    }

    public String getStudentName() {
        return studentName;
    }

    public ArrayList<CourseRegistration> getRegistrations() {
        return registrations;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
    }

    public void setRegistrations(ArrayList<CourseRegistration> registrations) {
        this.registrations = registrations;
    }

    public void registerCourse(Course course, double grade) {
        Double gradeWrapper = grade;
        registrations.add(new CourseRegistration(course, gradeWrapper));
    }

    public void printReport() {
            // System.out.println(s.getStudentId() + ". " + s.getStudentName());
            // ArrayList<CourseRegistration> regs = s.getRegistrations();

            // for (int j = 0; j < regs.size(); j++) {
            //     CourseRegistration cr = regs.get(j);
            //     Course c = courses.get((cr.getCourse().getCourseId()));
            //     System.out.println("| " + c.getCourseId() + ". " + c.getCourseName() + " (" + c.getCreditHours() + "), Grade = " + cr.getGrade());
            // }
            // System.out.println("-----------------------");
        StringBuilder sb = new StringBuilder("Student " + getStudentId() + ": " + getStudentName() + "\n");
        for (int i = 0; i < registrations.size(); i++) {
            Course curr = registrations.get(i).getCourse();
            sb.append("| ").append(curr.getCourseId()).append(". ").append(curr.getCourseName()).append(" (").append(curr.getCreditHours()).append("h), Grade = ").append(registrations.get(i).getGrade()).append("\n");
            // sb.append("Course ").append(curr.getCourseId()).append(": ").append(curr.getCourseName()).append(" (").append(curr.getCreditHours()).append("h) -> ").append(registrations.get(i).getGrade()).append("\n");
        }
        sb.append("-----------------------\n");
        System.out.println(sb.toString());
    }

    public class CourseRegistration {
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

        public void setGrade(Double grade) {
            this.grade = grade;
        }

        public void setCourse(Course course) {
            this.course = course;
        }
    }
}
