from django.shortcuts import render
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import TokenAuthentication
from .models import Subject
from .serializers import SubjectSerializer

class SubjectListView(APIView):
  authentication_classes = [TokenAuthentication]
  permission_classes = [IsAuthenticated]

  def get(self, request, code = None):
    if code:
      subject = Subject.objects.get(code=code)
      serializer = SubjectSerializer(subject)
      return Response(serializer.data, status=200)
    else:
      subjects = Subject.objects.all()
      serializer = SubjectSerializer(subjects, many=True)
      return Response(serializer.data, status=200)

  def post(self, request):
    serializer = SubjectSerializer(data=request.data)
    if serializer.is_valid():
      serializer.save()
      return Response(serializer.data, status=200)
    return Response(serializer.errors, status=400)

  def patch(self, request,code):
      subject = Subject.objects.get(code=code)
      serializer = SubjectSerializer(subject, data=request.data, partial=True)
      if serializer.is_valid():
          serializer.save()
          return Response(serializer.data, status=200)
      return Response(serializer.errors, status=400)

  def delete(self, request,code):
      subject = Subject.objects.get(code=code)
      subject.delete()
      return Response({"msg": "Subject deleted successfully."}, status=200)
