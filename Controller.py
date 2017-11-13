import socket
import time
import numpy as np
import serial
import pyttsx
import datetime

from ServerCellphone import CellphoneServer
from ServerGPS import GPS_Server
from planner import Planner

class Controller(object):
	def __init__(self,tolerance):
		
		self.engine = pyttsx.init()
		self.engine.setProperty('voice', 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_ZIRA_11.0')
		self.ser = serial.Serial('COM4', 9600, timeout=0)
		

		self.tolerance=tolerance
		self.printf=1 #its a correction term for the GPS communication such that only the postion values are received
		self.move=""

		

		self.tpx=0.0 #target gps coordinate
		self.tpy=0.0

		self.straight=0.0
		self.targetreached=False
		self.targetset=False
		self.orientsig=0 #signal received from arduino telling that is has completed its control for a turn
		self.orientation=""
		self.fN=True #orientations statuses, chance of facing north, south, west, etc
		self.fW=True
		self.fE=True
		self.fS=True
		self.buf="" #the read buffer

		self.turnstatus="turned"
		self.lastturn="none"

	def _Decide(self,KalmanFilter,Planner):
		self.buf = self.connection.recv(53)
		self.orientsig=self.ser.read()
		
		#Relaying aceleration increase and decrease commands to ARDUINO so that it know how much to push the gas pedal next time
		if (CellphoneServer.acceleration=="accelerating"):
			ser.write('a')
			CellphoneServer.acceleration="neutral"	
		if (CellphoneServer.acceleration=="deccelerating"):
			ser.write('d')
			CellphoneServer.acceleration="neutral"

		self.cpx=GPS_Server.cpx
		self.cpy=GPS_Server.cpy 
		
		try: 
			self.tpx = float(CellphoneServer.targetGPS[0:9]) # GPS target sent over by cellphone to rover         
			self.tpy = float(CellphoneServer.targetGPS[11:-1])
			#print("GPS TARGET ",datalx)
			#print("GPS TARGET ",dataly)        
		except ValueError as e:   
			print("Could not convert to float ")
		self.targetset=CellphoneServer.newtargetflag
		self.orientation = CellphoneServer.orientation
		self.move=CellphoneServer.movestatus
		#Direct query to arduino giving the status of completion of its turn execution control (PI control to be used directly in ARDUINO)
		
		#checking if tractor reached target
		
		if(self.move=="stop"):
			self.ser.write('s')			






		