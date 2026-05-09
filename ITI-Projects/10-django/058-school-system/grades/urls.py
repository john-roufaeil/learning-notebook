from django.urls import path

from .views import grades, grade_delete

urlpatterns = [
    path("", grades, name="grades"),
    path("delete/<str:id>/", grade_delete, name="grade_delete"),
]