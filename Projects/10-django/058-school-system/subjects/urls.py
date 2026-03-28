from django.urls import path

from .views import subjects, subject_delete

urlpatterns = [
    path("", subjects, name="subjects"),
    path("delete/<str:id>/", subject_delete, name="subject_delete"),
]