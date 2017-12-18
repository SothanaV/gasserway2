from django.db import models

# Create your models here.
class Node(models.Model):
	describe = models.CharField(max_length=100)

	def __unicode__(self):
		return "%s %s"%(self.id, self.describe)

class ParamValue(models.Model):
	node = models.ForeignKey(Node)
	time = models.DateTimeField(auto_now_add=True)
	value = models.FloatField()