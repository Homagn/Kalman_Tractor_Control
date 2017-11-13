import socket
import time
import numpy as np
import serial
import pyttsx
import datetime

from ServerCellphone import CellphoneServer

class GPS_Server(object):
	def __init__(self,bind_addr,bind_port):
		open('Datalogs/GPS.txt', 'w').close()
		self.datacount=0 #counts the number of readings in the datalog file for GPS
		self.engine = pyttsx.init()
		self.engine.setProperty('voice', 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_ZIRA_11.0')
		
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.serversocket.bind((bind_addr,bind_port))
		self.serversocket.listen(5) # become a server socket, maximum 5 connections
		self.connection, self.address = self.serversocket.accept()

		
		self.printf=1 #its a correction term for the GPS communication such that only the postion values are received
		
		self.cpx=0.0 #current position x and y
		self.cpy=0.0

		self.buf="" #the read buffer


	def _listen(self):
		self.buf = self.connection.recv(53) 
		if len(self.buf) > 0: #trying to get the current GPS coordinates correctly
			print len(self.buf)
			if(self.printf==7):       
				#print buf[25:-1]#25:53
				#print buf
				try:
					self.cpx = float(self.buf[25:38])
					self.cpy = float(self.buf[40:-1])
					print("current location", self.cpx)
					print("current location", self.cpy)
					timestring=datetime.datetime.time(datetime.datetime.now()))
					if(self.datacount<100000):#save last 100000 GPS data
						loc= open("Datalogs/GPS.txt", "a")     
						loc.write("%s %s %s " %str(cpx),%str(cpy),timestring)
						self.datacount=datacount+1
						loc.write("\n")
						loc.close()
					else:
						self.datacount=0
						open('Datalogs/GPS.txt', 'w').close()
						loc= open("Datalogs/GPS.txt", "a")     
						loc.write("%s %s %s " %str(cpx),%str(cpy),timestring)
						self.datacount=datacount+1
						loc.write("\n")
						loc.close()			
				except:
					print "get lost"
					self.printf=self.printf+1
			if(self.printf==21):
				self.printf=0

			if(self.printf<21):
				self.printf=self.printf+1