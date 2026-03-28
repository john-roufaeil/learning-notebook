from django.shortcuts import render, redirect
from .models import Grade
from students.models import Student
from subjects.models import Subject

def grades(request):
  if not request.session.get('email'):
    return redirect('login')

  if request.method == "GET":
    grades = Grade.objects.all()
    students = Student.objects.all()
    subjects = Subject.objects.all()
    return render(request, "grades.html", {'grades': grades, 'students': students, 'subjects': subjects})

  studentId = request.POST.get("student")
  subjectCode = request.POST.get("subject")
  score = request.POST.get("score")
  student = Student.objects.get(pk=studentId)
  subject = Subject.objects.get(pk=subjectCode)
  Grade.objects.create(student=student, subject=subject, score=score)
  return redirect("grades")

def grade_delete(request, id):
  if not request.session.get('email'):
    return redirect('login')
  grade = Grade.objects.get(pk=id)
  grade.delete()
  return redirect("grades")
