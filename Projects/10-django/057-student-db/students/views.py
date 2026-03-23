from django.shortcuts import render, redirect
from .models import Student

def students_page(request):
    if request.method == "GET":
        students = Student.objects.all()
        return render(request, "students.html", {'students': students})

    name = request.POST.get("name")
    age = request.POST.get("age")
    email = request.POST.get("email")
    img = request.FILES.get("img")
    Student.objects.create(name=name, age=age, email=email, img=img)
    return redirect("students_page")

def student_delete(request, id):
    student = Student.objects.get(id=id)
    student.delete()
    students = Student.objects.all()
    return redirect("students_page")