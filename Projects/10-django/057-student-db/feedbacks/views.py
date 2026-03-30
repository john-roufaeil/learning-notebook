from django.shortcuts import render, redirect
from .models import Feedback

def contact_page(request):
    if request.method == "GET":
        return render(request, "contact.html")
    name = request.POST.get("name")
    email = request.POST.get("email")
    message = request.POST.get("message")
    Feedback.objects.create(name=name, email=email, message=message)
    return redirect("contact_page")