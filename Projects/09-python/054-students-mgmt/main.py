from studentService import *
from gradeService import *

clearStudents()
clearGrades()

insertStudent(1, "John")
insertStudent(2, "Zeyad")
insertStudent(3, "Ahmed")
insertStudent(4, "Yasmine")
insertStudent(5, "Anne")

insertGrade(1, "Python", 98)
insertGrade(1, "English", 24)
insertGrade(1, "JS", 35)
insertGrade(2, "Python", 23)
insertGrade(2, "English", 84)
insertGrade(2, "JS", 28)
insertGrade(3, "Python", 19)
insertGrade(3, "English", 94)
insertGrade(3, "JS", 39)
insertGrade(4, "Python", 94)
insertGrade(4, "English", 12)
insertGrade(4, "JS", 85)
insertGrade(5, "Python", 93)
insertGrade(5, "English", 35)
insertGrade(5, "JS", 75)

printStudentsNames()

print(f"\nGrades of Python:")
printGradesOfSubject("Python")

print(f"\nGrades of English:")
printGradesOfSubject("English")

print(f"\nGrades of JS:")
printGradesOfSubject("JS")

printStudentsAvgGrade()

while (True):
  printStudentDetails()