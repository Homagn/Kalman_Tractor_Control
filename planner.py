import socket
import time
import numpy as np
import serial
import pyttsx
import datetime
import math

from ServerCellphone import CellphoneServer
from ServerGPS import GPS_Server

class Planner(object):
	def __init__(self,CellphoneServer):
		self.CellphoneServer=CellphoneServer
		self.routeplan=[]

	def _plan(self,divisions):
		self.radius=radius
		if(self.targetGPS!="" and self.currentGPS!=""):
			tpx = float(self.CellphoneServer.targetGPS[0:9]) # GPS target sent over by cellphone to rover         
			tpy = float(self.CellphoneServer.targetGPS[11:-1]) 
			spx = float(self.CellphoneServer.startingGPS[0:9]) # GPS target sent over by cellphone to rover         
			spy = float(self.CellphoneServer.startingGPS[11:-1])
			#generate a list of tuples that approximate the entire path
			diffx=tpx-spx
			diffy=tpy-spy
			xdiv-fabs(diffx)/divisions
			ydiv=fabs(diffy)/divisions
			for i in range(xdiv+ydiv):#always accounts for x distance first, then y distance
				if(i>0 and i<xdiv):
					p=[spx+i*(diffx/xdiv), spy]
					self.routeplan.append(p)
				if(i>xdiv and i<xdiv+ydiv):
					p=[tpx, spy+i*(diffy/ydiv)]
					self.routeplan.append(p)

