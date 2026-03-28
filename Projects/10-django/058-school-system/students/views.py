from django.shortcuts import render, redirect
from .models import Student

def students(request):
  if not request.session.get('email'):
    return redirect('login')

  if request.method == "GET":
    students = Student.objects.all()
    return render(request, "students.html", {'students': students})

  firstName = request.POST.get("firstName")
  lastName = request.POST.get("lastName")
  studentId = request.POST.get("studentId")
  email = request.POST.get("email")
  gender = request.POST.get("gender")
  Student.objects.create(firstName=firstName, lastName=lastName, studentId=studentId, gender=gender, email=email)
  return redirect("students")

def student_delete(request, id):
  if not request.session.get('email'):
    return redirect('login')
    
  student = Student.objects.get(pk=id)
  student.delete()
  return redirect("students")