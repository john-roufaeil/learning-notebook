from rest_framework import serializers
from .models import Student

class StudentSerializer(serializers.ModelSerializer):
  class Meta:
    model = Student
    fields = "__all__"

class StudentUpdateSerializer(serializers.ModelSerializer):
  class Meta:
    model = Student
    fields = "__all__"
    extra_kwargs = {"email": {"validators": []}}