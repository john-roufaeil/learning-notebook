from django.urls import path

from .views import StudentListView 

urlpatterns = [
    path("", StudentListView.as_view(), name="students"),
    path("<str:studentId>/", StudentListView.as_view(), name="student_detail"),
]