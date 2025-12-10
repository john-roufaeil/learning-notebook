package courseregistration.main;

import courseregistration.course.Course;
import courseregistration.student.Student;
import java.io.IOException;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Main {
    static Scanner scanner = new Scanner(System.in);
    public static void main(String[] args) {
        Course.addCourse("Object-Oriented Programming", 4);
        Course.addCourse("Data Structures", 4);
        Course.addCourse("Database Systems", 3);
        Course.addCourse("Web Development", 2);

        int choice = -1;
        do {
            clearConsole();
            System.out.println("\n=== Course Registration System ===");
            System.out.println("1. Add Student");
            System.out.println("2. Add Course");
            System.out.println("3. Register Courses for Student");
            System.out.println("4. Print Student Report");
            System.out.println("5. Exit");
            System.out.print("Enter choice: ");
            
            String input = scanner.nextLine();
            if (input.isEmpty()) {
                clearConsole();
                continue;
            }
            try {
                choice = Integer.parseInt(input);
            } catch(NoSuchElementException | NumberFormatException e) {
                System.out.println("Invalid Input");
            }

            switch (choice) {
                case 1: addStudent(); break;
                case 2: addCourse(); break;
                case 3: registerCourseForStudent(); break;
                case 4: printStudentReport(); break;
                case 5: System.out.println("Goodbye :)"); break;
                default: System.out.println("Invalid choice.");
            }
        } while (choice != 5);
    }

    public static void addStudent() {
        clearConsole();
        int choice = -1;

        do {
            System.out.print("Enter student's name: ");
            String input = scanner.nextLine().trim();
            if (input.isEmpty()) {
                clearConsole();
                continue;
            }

            try {
                Student.addStudent(input);
                System.out.println("\nStudent " + input + " (ID " + Student.getStudentsCount() + ") " + "added successfully");
                System.out.println("\nAdd another student?");
            } catch(IllegalArgumentException e) {
                System.out.println("ERROR: " + e.getMessage());
                System.out.println("\nTry again?");
            }
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice != 2);
    }

    public static void addCourse() {
        clearConsole();
        int choice = -1;
        
        do { 
            System.out.print("Enter course details \"NAME|CREDIT_HOURS\": ");
            String input = scanner.nextLine().trim();
            if (input.isEmpty()) {
                clearConsole();
                continue;
            }

            StringTokenizer st = new StringTokenizer(input, "|");
            try {
                String courseName = st.nextToken();
                int creditHours = Integer.parseInt(st.nextToken());
                Course.addCourse(courseName, creditHours);
                System.out.println("\nCourse " + courseName + " - " + creditHours + "h (ID " + Course.getCoursesCount() + ") " + "added successfully");
                System.out.println("\nAdd another course?");
            } catch (IllegalArgumentException | NoSuchElementException e) {
                System.out.println("ERROR" + (e.getMessage() == null ? "" : ": " + e.getMessage()));
                System.out.println("\nTry again?");
            }
            
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice != 2);
    }

    public static void registerCourseForStudent() {
        clearConsole();
        int choice = -1;

        do { 
            System.out.println("Available Students:");
            for (int i = 0; i < Student.getStudentsCount(); i++) {
                Student s = Student.getStudents().get(i);
                System.out.println(s.getStudentId() + ". " + s.getStudentName());
            }

            System.out.println("\nAvailable Courses:");
            for (int i = 0; i < Course.getCoursesCount(); i++) {
                Course c = Course.getCourses().get(i);
                System.out.println(c.getCourseId() + ". " + c.getCourseName() + " (" + c.getCreditHours() + "h)");
            }
            
            System.out.print("\nEnter registration choice \"STUDENT_ID COURSE_ID GRADE\": ");
            String input = scanner.nextLine().trim();
            if (input.isEmpty()) {
                clearConsole();
                continue;
            }

            StringTokenizer st = new StringTokenizer(input);
            
            try {
                int studentId = Integer.parseInt(st.nextToken());
                int courseId = Integer.parseInt(st.nextToken());
                double courseGrade = Double.parseDouble(st.nextToken());
                if (studentId <= 0 || courseId <= 0) throw new IllegalArgumentException("ID cannot be non positive");
                if (studentId > Student.getStudentsCount()) throw new IllegalArgumentException("Student " + studentId + " doesn't exist");
                if (courseId > Course.getCoursesCount()) throw new IllegalArgumentException("Course " + courseId + " doesn't exist");
                Student.getStudents().get(studentId - 1).registerCourse(Course.getCourses().get(courseId - 1), courseGrade);
                System.out.println("\nStudent " + studentId + " successfully registered in course " +  courseId + " with grade " + courseGrade);
                System.out.println("\nRegister another?");
            } catch (IllegalArgumentException | NoSuchElementException e) {
                System.out.println("ERROR" + (e.getMessage() == null ? "" : ": " + e.getMessage()));
                System.out.println("\nTry again?");
            }
            
            System.out.println("1. Yes");
            System.out.println("2. No, return to main menu");
            choice = scanner.nextInt();
            scanner.nextLine();
            clearConsole();
        } while (choice != 2);
    }

    public static void printStudentReport() {
        clearConsole();
        System.out.println("=== Students Report ===");
        for (int i = 0; i < Student.getStudentsCount(); i++) {
            Student.getStudents().get(i).printReport();
        }
        System.out.println("\nPress Enter to return to main menu...");
        scanner.nextLine();
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