package courseregistration.main;

import courseregistration.course.Course;
import courseregistration.student.Student;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Main {
    static Scanner scanner = new Scanner(System.in);
    public static void main(String[] args) {
        ArrayList<Student> students = new ArrayList<>();
        ArrayList<Course> courses = new ArrayList<>();
        courses.add(new Course("Object-Oriented Programming", 4));
        courses.add(new Course("Data Structures", 4));
        courses.add(new Course("Database Systems", 3));
        courses.add(new Course("Web Development", 2));

        int choice;
        do {
            clearConsole();
            System.out.println("\n=== Course Registration System ===");
            System.out.println("1. Add Student");
            System.out.println("2. Add Course");
            System.out.println("3. Register Courses for Student");
            System.out.println("4. Print Student Report");
            System.out.println("5. Exit");
            System.out.print("Enter choice: ");
            
            choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1: addStudent(students); break;
                case 2: addCourse(courses); break;
                case 3: registerCourseForStudent(students, courses); break;
                case 4: printStudentReport(students, courses); break;
                case 5: System.out.println("Goodbye :)"); break;
                default: System.out.println("Invalid choice.");
            }
        } while (choice != 5);
    }

    public static void addStudent(ArrayList<Student> students) {
        clearConsole();
        int choice;

        do {
            System.out.print("Enter student's name: ");
            String name = scanner.nextLine();

            students.add(new Student(name));
            System.out.println("\nStudent " + name + " (ID " + Student.getStudentsCount() + ") " + "added successfully");
            
            System.out.println("\nAdd anohter student?");
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice == 1);
    }

    public static void addCourse(ArrayList<Course> courses) {
        clearConsole();
        int choice;

        do { 
            System.out.print("Enter course details \"NAME CREDIT_HOURS\": ");
            StringTokenizer st = new StringTokenizer(scanner.nextLine());
            String courseName = st.nextToken();
            int creditHours = Integer.parseInt(st.nextToken());
            courses.add(new Course(courseName, creditHours));
            System.out.println("\nCourse " + courseName + " - " + creditHours + "h (ID " + Course.getCoursesCount() + ") " + "added successfully");
            
            System.out.println("\nAdd anohter course?");
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice == 1);
    }

    public static void registerCourseForStudent(ArrayList<Student> students, ArrayList<Course> courses) {
        clearConsole();
        int choice;

        do { 
            System.out.println("Available Students:");
            for (int i = 0; i < students.size(); i++) {
                Student s = students.get(i);
                System.out.println(s.getStudentId() + ". " + s.getStudentName());
            }

            System.out.println("\nAvailable Courses:");
            for (int i = 0; i < courses.size(); i++) {
                Course c = courses.get(i);
                System.out.println(c.getCourseId() + ". " + c.getCourseName() + " (" + c.getCreditHours() + "h)");
            }
            
            System.out.print("\nEnter registration choice \"STUDENT_ID COURSE_ID GRADE\": ");
            StringTokenizer st = new StringTokenizer(scanner.nextLine());
            int studentId = Integer.parseInt(st.nextToken());
            int courseId = Integer.parseInt(st.nextToken());
            double courseGrade = Double.parseDouble(st.nextToken());
            
            students.get(studentId - 1).registerCourse(courses.get(courseId - 1), courseGrade);
            System.out.println("\nStudent " + studentId + " successfully registered in course " +  courseId + " with grade " + courseGrade);
            
            System.out.println("\nRegister another?");
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice == 1);
    }

    public static void printStudentReport(ArrayList<Student> students, ArrayList<Course> courses) {
        clearConsole();
        System.out.println("=== Students Report ===");
        for (int i = 0; i < students.size(); i++) {
            students.get(i).printReport();
        }
        System.out.println("\nEnter any key to return to main menu");
        scanner.next();
    }

    public static void clearConsole() {
        try {
            String os = System.getProperty("os.name").toLowerCase();
            
            if (os.contains("win")) {
                new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            } else {
                new ProcessBuilder("clear").inheritIO().start().waitFor();
            }
        } catch (IOException | InterruptedException e) {
            System.out.print("\033[H\033[2J");
            System.out.flush();
        }
    }
}