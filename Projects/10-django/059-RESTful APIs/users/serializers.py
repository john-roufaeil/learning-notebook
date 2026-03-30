from rest_framework import serializers
from django.contrib.auth.models import User

class UserGetSerializer(serializers.ModelSerializer):
  class Meta:
    model = User
    fields = ['username']

class UserCreateSerializer(serializers.ModelSerializer):
  class Meta:
    model = User
    fields = ['username', 'password']
    extra_kwargs = {
        'password': {'write_only': True}
    }

  def create(self, validated_data):
    return User.objects.create_user(**validated_data)