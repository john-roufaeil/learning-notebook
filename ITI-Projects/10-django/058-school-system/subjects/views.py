from django.shortcuts import render, redirect
from .models import Subject

def subjects(request):
  if not request.session.get('email'):
    return redirect('login')

  if request.method == "GET":
    subjects = Subject.objects.all()
    return render(request, "subjects.html", {'subjects': subjects})

  name = request.POST.get("name")
  code = request.POST.get("code")
  credits = request.POST.get("credits")
  Subject.objects.create(name=name, code=code, credits=credits)
  return redirect("subjects")

def subject_delete(request, id):
  if not request.session.get('email'):
    return redirect('login')
    
  subject = Subject.objects.get(pk=id)
  subject.delete()
  return redirect("subjects")