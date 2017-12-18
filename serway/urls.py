from django.conf.urls import url
from . import views
from django.contrib.auth import views as auth_views



urlpatterns = [
	url(r'^$', views.home, name='home'),
	url(r'control/^$', views.control, name='control'),
	url(r'^graph/', views.graph),
    url(r'^getdata/', views.getdata, name='getdata'),
    url(r'^getdata2/', views.getdata2, name='getdata2'),
    url(r'^getdata3/', views.getdata3, name='getdata3'),
	url(r'^data/(?P<nodeid>\d+)/(?P<value>\d+\.\d+)/?$', views.passdata),
]