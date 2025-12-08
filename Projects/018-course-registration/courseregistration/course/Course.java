package courseregistration.course;

public class Course {
    private static Integer coursesCount = 0;
    private final Integer courseId;
    private String courseName;
    private Integer creditHours;

    public Course(String courseName, Integer creditHours) {
        this.courseId = coursesCount + 1;
        this.courseName = courseName;
        this.creditHours = creditHours;
        coursesCount++;
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
        if (!courseName.isEmpty()) this.courseName = courseName;
    }

    public void setCreditHours(Integer creditHours) {
        this.creditHours = creditHours;
    }
} 
