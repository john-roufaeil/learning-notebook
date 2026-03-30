from django.shortcuts import render
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import TokenAuthentication
from .models import Student
from .serializers import StudentSerializer, StudentUpdateSerializer

class StudentListView(APIView):
  authentication_classes = [TokenAuthentication]
  permission_classes = [IsAuthenticated]
  
  def get(self, request, studentId = None):
    if studentId:
      student = Student.objects.get(studentId=studentId)
      serializer = StudentSerializer(student)
      return Response(serializer.data, status=200)
    else:
      students = Student.objects.all()
      serializer = StudentSerializer(students, many=True)
      return Response(serializer.data, status=200)

  def post(self, request):
    serializer = StudentSerializer(data=request.data)
    if serializer.is_valid():
      serializer.save()
      return Response(serializer.data, status=200)
    return Response(serializer.errors, status=400)
  
  def patch(self, request,studentId):
      student = Student.objects.get(studentId=studentId)
      serializer = StudentUpdateSerializer(student, data=request.data, partial=True)
      if serializer.is_valid():
          serializer.save()
          return Response(serializer.data, status=200)
      return Response(serializer.errors, status=400)

  def delete(self, request,studentId):
      student = Student.objects.get(studentId=studentId)
      student.delete()
      return Response({"msg": "Student deleted successfully."}, status=200)