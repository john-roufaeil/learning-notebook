from django.db import models

# Create your models here.
class Subject(models.Model):
  name = models.CharField(max_length=255, null=False, blank=False)
  code = models.CharField(max_length=255, primary_key=True)
  credits = models.IntegerField(null=False, blank=False)

  def __str__(self):
    return self.name