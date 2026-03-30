from django.shortcuts import render
from django.http import HttpResponse
from django.contrib.auth.models import User
from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.contrib.auth import logout as django_logout

from django.contrib.auth import authenticate
from rest_framework.decorators import authentication_classes, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.authentication import TokenAuthentication
from rest_framework.authtoken.models import Token
from .serializers import UserGetSerializer, UserCreateSerializer

@api_view(['POST'])
def login(request):
  username = request.data.get('username')
  password = request.data.get('password')
  
  if not username or not password:
    return Response({'error': 'username and password are required'}, status=400)
  user = authenticate(request, username=username, password=password)

  if user:
    Token.objects.filter(user=user).delete()
    newToken = Token.objects.create(user=user)
    return Response({"token": newToken.key}, status=200)
  else:
    return Response({'error': 'Invalid username or password'}, status=401)

@api_view(['POST'])
def register(request):
  user = UserCreateSerializer(data=request.data)
  if user.is_valid():
    new_user = user.save()
    token = Token.objects.create(user=new_user)
    return Response({'token': token.key}, status=201) 
  else:
    return Response(user.errors, status=400) 

@api_view(["POST"])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def logout(request):
  Token.objects.filter(user_id=request.user.id).delete()
  django_logout(request)
  return Response({"message": "Logged out"}, status=200)

@api_view(['GET'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def profile(request):
  user = request.user
  return Response({'username': user.username}, status=200)

@api_view(['PUT'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def profile_update_password(request):
  user = request.user
  current_password = request.data.get('current_password')
  new_password = request.data.get('new_password')
  if not current_password or not new_password:
    return Response({'error': 'Current password and new password are required'}, status=400)
  if not user.check_password(current_password):
    return Response({'error': 'Current password is incorrect'}, status=400)
  user.set_password(new_password)
  user.save()
  return Response({'message': 'Password updated successfully'}, status=200)
  

@api_view(['DELETE'])
@authentication_classes([TokenAuthentication])
@permission_classes([IsAuthenticated])
def profile_delete(request):
  user = request.user
  if user:
    user.delete()
    return Response({'message': 'Profile deleted successfully'}, status=200)
  return Response({'error': 'User not found'}, status=404)