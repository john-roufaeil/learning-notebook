from django.urls import path

from .views import SubjectListView

urlpatterns = [
    path("", SubjectListView.as_view(), name="subjects"),
    path("<str:code>/", SubjectListView.as_view(), name="subject_detail"),
]