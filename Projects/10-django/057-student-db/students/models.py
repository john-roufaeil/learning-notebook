from django.db import models

class Student(models.Model):
  id = models.AutoField(primary_key=True)
  name = models.CharField(unique=True, max_length=256)
  email = models.EmailField(unique=True, max_length=25)
  age = models.IntegerField()
  img = models.ImageField(upload_to='students/')
  date_added = models.DateTimeField(auto_now_add=True)
  date_updated = models.DateTimeField(auto_now=True)
