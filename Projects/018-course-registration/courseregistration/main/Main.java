package courseregistration.main;

import courseregistration.course.Course;
import courseregistration.student.Student;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

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
                case 2: // addCourse(courses);  break;
                case 3: // registerCourseForStudent(students, courses); break;
                case 4: // printStudentReport(students); break;
                case 5: System.out.println("Goodbye :)"); break;
                default: System.out.println("Invalid choice.");
            }
        } while (choice != 5);
    }

    public static void addStudent(ArrayList<Student> students) {
        clearConsole();
        Scanner scanner = new Scanner(System.in);
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