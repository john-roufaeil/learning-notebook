def clearGrades():
  try:
    file = open("grades.txt", "w")
    file.write("")
  except:
    print("Error in grade clear")
  finally:
    file.close()

def insertGrade(student_id, subject, grade):
  try:
    file = open("grades.txt", "a")
    file.write(f"{student_id},{subject},{grade}\n")
  except:
    print("Error in grade insertion")
  finally:
    file.close()

def readGrades():
  grades = []
  try:
    file = open("grades.txt", "r")
    for line in file:
      line = line.strip()
      parts = line.split(",")
      grade = (parts[0], parts[1], parts[2])
      grades.append(grade)
  except:
    print("Error in grades read")
  finally:
    file.close()
  return grades

def printGradesOfSubject(subject):
  grades = readGrades()
  for grade in grades:
    if grade[1] == subject:
      print(grade[2])

