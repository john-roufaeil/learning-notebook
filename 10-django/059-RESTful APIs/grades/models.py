from django.db import models
from students.models import Student
from subjects.models import Subject

class Grade(models.Model):
  student = models.ForeignKey(Student, on_delete=models.CASCADE)
  subject = models.ForeignKey(Subject, on_delete=models.CASCADE)
  score = models.IntegerField(null=False, blank=False)

  def __str__(self):
    return f"{self.student} - {self.subject} : {self.score}"