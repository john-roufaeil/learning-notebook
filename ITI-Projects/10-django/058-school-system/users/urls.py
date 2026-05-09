from django.urls import path
from .views import register, logout, login, profile, profile_update_email, profile_update_password, profile_delete

urlpatterns = [
  path('login/', login, name='login'),
  path('register/', register, name='register'),
  path('logout/', logout, name='logout'),
  path('profile/', profile, name='profile'),
  path('profile_update_email/', profile_update_email, name='profile_update_email'),
  path('profile_update_password/', profile_update_password, name='profile_update_password'),
  path('profile_delete/', profile_delete, name='profile_delete'),
]