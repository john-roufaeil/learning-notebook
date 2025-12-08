package courseregistration.course;

import courseregistration.utils.Verifiable;
import java.util.ArrayList;

public class Course implements Verifiable {
    private static Integer coursesCount = 0;
    private final static ArrayList<Course> courses = new ArrayList<>();
    private final Integer courseId;
    private String courseName;
    private Integer creditHours;

    public Course(String courseName, Integer creditHours) {
        this.courseId = coursesCount + 1;
        this.courseName = courseName;
        this.creditHours = creditHours;
    }

    public static Course addCourse(String courseName, Integer creditHours) {
        Course course = new Course(courseName, creditHours);
        course.verify();
        courses.add(course);
        coursesCount++;
        return course;
    }

    public static Integer getCoursesCount() {
        return coursesCount;
    }

    public Integer getCourseId() {
        return courseId;
    }

    public String getCourseName() {
        return courseName;
    }

    public Integer getCreditHours() {
        return creditHours;
    }

    public void setCourseName(String courseName) {
        this.courseName = courseName;
        this.verify();
    }

    public void setCreditHours(Integer creditHours) {
        this.creditHours = creditHours;
        this.verify();
    }

    @Override
    public void verify() {
        if (creditHours < 1) {
            throw new IllegalArgumentException("Credit Hours cannot be less than 1");
        } else if (creditHours > 8) {
            throw new IllegalArgumentException("Credit Hours cannot be more than 8.");
        } else if (courseName.isBlank()) {
            throw new IllegalArgumentException("Course Name cannot be blank");
        }
        for (int i = 0; i < courses.size(); i++) {
            Course c = courses.get(i);
            if (c.getCourseName().trim().equalsIgnoreCase(courseName.trim()) && !c.getCourseId().equals(courseId)) {
                throw new IllegalArgumentException("Course already exists.");
            }
        }
    }
} 
