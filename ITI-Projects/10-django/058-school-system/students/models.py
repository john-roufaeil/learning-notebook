from django.db import models

# Create your models here.
class Student(models.Model):
  firstName = models.CharField(max_length=255, null=False, blank=False)
  lastName = models.CharField(max_length=255, null=False, blank=False)
  studentId = models.CharField(max_length=255, primary_key=True)
  email = models.EmailField(unique=True, null=False, blank=False)
  gender = models.CharField(max_length=255, choices=[("Male", "Male"), ("Female", "Female")])
  
  def __str__(self):
    return f"{self.firstName} {self.lastName}"