from gradeService import readGrades

def clearStudents():
  try:
    file = open("students.txt", "w")
    file.write("")
  except:
    print("Error in students clear")
  finally:
    file.close()
    
def insertStudent(student_id, name):
  try:
    file = open("students.txt", "a")
    file.write(f"{student_id},{name}\n")
  except:
    print("Error in student insertion")
  finally:
    file.close()

def readStudents():
  students = []
  try:
    file = open("students.txt", "r")
    for line in file:
      line = line.strip()
      parts = line.split(",")
      student = (parts[0], parts[1])
      students.append(student)
  except:
    print("Error in students read")
  finally:
    file.close()
  return students

def printStudentsNames():
  students = readStudents()
  for student in students:
    print(student[1])

def printStudentDetails():
  student_id = input("Enter student ID: ")
  students = readStudents()
  grades = readGrades()
  for student in students:
    if student[0] == student_id:
      print(f"Student Name: {student[1]}")
      for grade in grades:
        if grade[0] == student_id:
          print(f"Grade in {grade[1]} is {grade[2]}")
      return
  print("This student does not exist")

def printStudentsAvgGrade():
  students = readStudents()
  grades = readGrades()
  for student in students:
    gradesCount = 0
    gradesSum = 0
    for grade in grades:
      if grade[0] == student[0]:
        gradesCount+=1
        gradesSum+=int(grade[2])
    print(f"Student '{student[1]}' has average grade of {(gradesSum/gradesCount):.2f}")
    print("----------")
