from django.db import models

# Create your models here.
class Feedback(models.Model):
  id = models.AutoField(primary_key=True)
  name = models.CharField(unique=True, max_length=256)
  email = models.EmailField(unique=True, max_length=25)
  message = models.CharField(max_length=1000)