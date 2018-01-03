from django.shortcuts import render,redirect
from flask_socketio import SocketIO, send, emit
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse,HttpResponse
from .models import *
# Create your views here.
global command1
global command2
global command3
command1 = "S"
command2 = "S"
command3 = "S"


@login_required
def home(request):
	return redirect('http://128.199.198.154:5001/home')

def control(request):
	return render(request,'control.html')

def passdata(request, nodeid,value):
	if request.method == 'GET':
		current_node = Node.objects.get(id=nodeid)
		ParamValue.objects.create(node=current_node,value=value)
	
	return HttpResponse( '%s %s'%(nodeid,value) )
def getdata(request):
	data = ParamValue.objects.filter(node__id=1).order_by('-time').values('time', 'value')[:500]
	return JsonResponse( list(data) , safe= False)
def getdata2(request):
	data = ParamValue.objects.filter(node__id=2).order_by('-time').values('time', 'value')[:500]
	return JsonResponse( list(data) , safe= False)
def getdata3(request):
	data = ParamValue.objects.filter(node__id=3).order_by('-time').values('time', 'value')[:500]
	return JsonResponse( list(data) , safe= False)

@login_required
def graph(request):
	return render(request, 'first.html')

@login_required
def home2(request):
	#return render(request, 'home2.html')
	return render(request, 'first2.html')
