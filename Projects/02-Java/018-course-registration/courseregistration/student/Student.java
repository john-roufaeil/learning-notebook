package courseregistration.student;

import courseregistration.course.Course;
import courseregistration.utils.Verifiable;
import java.util.ArrayList;

public class Student implements Verifiable {
    private static Integer studentsCount = 0;
    private final static ArrayList<Student> students = new ArrayList<>();
    private final Integer studentId;
    private String studentName;    
    private final ArrayList<CourseRegistration> registrations;

    public Student(String studentName) {
        this.studentId = studentsCount + 1;
        this.studentName = studentName;
        this.registrations = new ArrayList<>();
    }

    public static Student addStudent(String studentName) {
        Student student = new Student(studentName.trim());
        student.verify();
        students.add(student);
        studentsCount++;
        return student;
    }

    public static Integer getStudentsCount() {
        return studentsCount;
    }
    
    public Integer getStudentId() {
        return studentId;
    }

    public static ArrayList<Student> getStudents() {
        return students;
    }

    public String getStudentName() {
        return studentName;
    }

    public ArrayList<CourseRegistration> getRegistrations() {
        return registrations;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
        verify();
    }

    public void registerCourse(Course course, double grade) {
        for (CourseRegistration reg : registrations) {
            if (reg.getCourse().getCourseId().equals(course.getCourseId())) {
                throw new IllegalArgumentException("This student already has this course registered");
            }
        }   
        Double gradeWrapper = grade;
        CourseRegistration reg = new CourseRegistration(course, gradeWrapper);
        reg.verify();
        registrations.add(reg);
    }

    public void printReport() {
        StringBuilder sb = new StringBuilder("Student " + getStudentId() + ": " + getStudentName() + "\n");
        for (int i = 0; i < registrations.size(); i++) {
            Course curr = registrations.get(i).getCourse();
            sb.append("| ").append(curr.getCourseId()).append(". ").append(curr.getCourseName()).append(" (").append(curr.getCreditHours()).append("h), Grade = ").append(registrations.get(i).getGrade()).append("\n");
        }
        sb.append("-----------------------\n");
        System.out.println(sb.toString());
    }

    @Override
    public void verify() {
        if (studentName == null) throw new IllegalArgumentException("Name cannot be null");

        for (int i = 0; i < studentsCount; i++) {
            if (students.get(i).getStudentName().trim().equalsIgnoreCase(studentName.trim())) {
                throw new IllegalArgumentException("Student already exists.");
            }
        }

        if(!studentName.matches("[A-Za-z ]+")) throw new IllegalArgumentException("Name is invalid");
    }
    
    public class CourseRegistration implements Verifiable {
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
            verify();
        }

        public void setCourse(Course course) {
            this.course = course;
            verify();
        }

        @Override
        public void verify() {
            if(grade < 1 || grade > 100) throw new IllegalArgumentException("Grade must be within range 1-100");
            else if(course == null) throw new IllegalArgumentException("Course cannot be null");
        }
    }
}
