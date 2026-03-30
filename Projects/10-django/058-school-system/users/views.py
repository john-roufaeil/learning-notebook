from django.shortcuts import render, redirect
from django.http import HttpResponse
from .models import User

# Create your views here.
def login(request):
  if request.method == 'GET':
    return render(request, 'login.html')
  email = request.POST.get('email')
  password = request.POST.get('password')
  user = User.objects.filter(email=email, password=password).first()
  if user:
    request.session['email'] = user.email
    return redirect('students')
  else:
    return HttpResponse('Invalid email or password')

def register(request):
  if request.method == 'GET':
    return render(request, 'register.html')
  email = request.POST.get('email')
  password = request.POST.get('password')
  user = User.objects.create(email=email, password=password)
  if user:
    return redirect('login')
  else:
    return HttpResponse('Registration failed')
    
def logout(request):
  request.session.pop('email', None)
  return redirect('login')

def profile(request):
  if not request.session.get('email'):
    return redirect('login')
  email = request.session.get('email')
  user = User.objects.filter(email=email).first()
  return render(request, 'profile.html', {'user': user})

def profile_update_email(request):
  if not request.session.get('email'):
    return redirect('login')
  if request.method == 'POST':
    email = request.POST.get('new_email')
    user = User.objects.filter(email=request.session.get('email')).first()
    user.email = email
    user.save()
    request.session['email'] = email
    return redirect('profile')
  else:
    return HttpResponse('Invalid request method')

def profile_update_password(request):
  if not request.session.get('email'):
    return redirect('login')
  if request.method == 'POST':
    current_password = request.POST.get('current_password')
    password = request.POST.get('new_password')
    user = User.objects.filter(email=request.session.get('email')).first()
    if user.password == current_password:
      user.password = password
      user.save()
      return redirect('profile')
    else:
      return HttpResponse('Current password is incorrect')
  else:
    return HttpResponse('Invalid request method')

def profile_delete(request):
  if not request.session.get('email'):
    return redirect('login')
  user = User.objects.filter(email=request.session.get('email')).first()
  user.delete()
  request.session.pop('email', None)
  return redirect('register')