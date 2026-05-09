from django.urls import path
from .views import students_page, student_delete

urlpatterns = [
    path("", students_page, name="students_page"),
    path("student_delete/<int:id>/", student_delete, name="student_delete"),
]
