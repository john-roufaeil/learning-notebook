from django.urls import path

from .views import students, student_delete

urlpatterns = [
    path("", students, name="students"),
    path("delete/<str:id>/", student_delete, name="student_delete"),
]