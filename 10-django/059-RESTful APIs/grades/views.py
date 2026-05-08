from rest_framework import viewsets
from rest_framework.authentication import TokenAuthentication
from rest_framework.permissions import IsAuthenticated
from django_filters.rest_framework import DjangoFilterBackend
from .serializers import GradeSerializer
from .models import Grade

class GradeViewSet(viewsets.ModelViewSet):
  permission_classes = [IsAuthenticated]
  authentication_classes = [TokenAuthentication]
  queryset = Grade.objects.all()
  serializer_class = GradeSerializer
  filter_backends = [DjangoFilterBackend]
  filterset_fields = ['student', 'subject', 'score']