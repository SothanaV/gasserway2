from django.contrib import admin
from .models import *
# Register your models here.
class NodeAdmin(admin.ModelAdmin):
	list_display = ('id', 'describe')
	list_editable=('describe',)
class ValueAdmin(admin.ModelAdmin):
	list_display = ('node', 'time', 'value')
	list_editable = ('value',)

admin.site.register(Node, NodeAdmin)
admin.site.register(ParamValue, ValueAdmin)