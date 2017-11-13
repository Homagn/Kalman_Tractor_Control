import socket
import serial
import pyttsx
import datetime
class CellphoneServerManual(object):
	def __init__(self,bind_addr,bind_port):
		open('Datalogs/potentiometer.txt', 'w').close()
		open('Datalogs/wheelencoder.txt', 'w').close()
		self.engine = pyttsx.init()
		self.engine.setProperty('voice', 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_ZIRA_11.0')
		self.ser = serial.Serial('COM3', 9600, timeout=0)
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.serversocket.bind((bind_addr, bind_port))
		#serversocket.bind(('192.168.1.4', 8089))
		self.serversocket.listen(5) # become a server socket, maximum 5 connections
		#insert functions for writing the 4 different types of text files
		self.readbuffer=""

		self.connection, self.address = self.serversocket.accept()
		self.acceleration="neutral"
		self.movestatus="stop"
		self.targetGPS=""
		self.startingGPS=""

		self.newtargetflag=False
		self.newstartingGPS=False
		self.orientation="N"
		self.datacount=0;

	def _listen(self):
		self.readbuffer = self.connection.recv(64)
		if len(self.readbuffer) > 0:
			print self.readbuffer
			if( self.readbuffer[0:-1]=='acc'):
				print("Increase acceleration")
				self.engine.say('Increase acceleration.')
				self.engine.runAndWait()
				self.acceleration="accelerating"
				self.ser.write('a')
			if( self.readbuffer[0:-1]=='dcc'):
				print("Decrease acceleration")
				self.engine.say('Decrease acceleration.')
				self.engine.runAndWait()
				self.acceleration="deccelerating"
				self.ser.write('d')
			if( self.readbuffer[0:-1]=='cc'):
				print("Base acceleration")
				self.engine.say('Base acceleration.')
				self.engine.runAndWait()
				self.acceleration="neutral"
			if( self.readbuffer[0:-1]=='moveF'):
				print("Issued move forward command")
				self.engine.say('Issued move forward command.')
				self.engine.runAndWait()
				self.movestatus="moving"
				self.ser.write('f')
			if( self.readbuffer[0:-1]==' '):
				print("Issued immediate stop command")
				self.movestatus="stop"
				self.engine.say('Issued immediate stop command.')
				self.engine.runAndWait()
				self.ser.write('s')
			if(len(self.readbuffer)>6 and self.newtargetflag==False):
				print("Entered a target GPS coordinate")
				self.targetGPS=self.readbuffer
				print("New target flag set to true")
				self.newtargetflag=True
				self.engine.say('Entered a target GPS coordinate.')
				self.engine.runAndWait()
				self.engine.say('New target flag set to true.')
				self.engine.runAndWait()
			if(len(self.readbuffer)>6 and self.newstartingflag==False):
				print("Entered the current GPS coordinate")
				self.startingGPS=self.readbuffer
				self.newstartingflag=True
				print("New current flag set to true")
				self.engine.say('Entered the current GPS coordinate.')
				self.engine.runAndWait()
				self.engine.say('New current flag set to true.')
				self.engine.runAndWait()

			if( self.readbuffer[0:-1]=='resettarget'):
				self.newtargetflag=False
				self.engine.say('Requested reset target.')
				self.engine.runAndWait()
			if( self.readbuffer[0:-1]=='resetstarting'):
				self.newstartingflag=False
				self.engine.say('Requested reset starting.')
				self.engine.runAndWait()

			if( self.readbuffer[0:-1]=='N'):
				print("Manual orientation set")
				self.orientation="N"
				self.engine.say('Manual orientation set to facing north.')
				self.engine.runAndWait()
			if( self.readbuffer[0:-1]=='W'):
				print("Manual orientation set")
				self.orientation="W"
				self.engine.say('Manual orientation set to facing west.')
				self.engine.runAndWait()
			if( self.readbuffer[0:-1]=='E'):
				print("Manual orientation set")
				self.orientation="E"
				self.engine.say('Manual orientation set to facing east.')
				self.engine.runAndWait()
			if( self.readbuffer[0:-1]=='S'):
				print("Manual orientation set")
				self.orientation="S"
				self.engine.say('Manual orientation set to facing south.')
				self.engine.runAndWait()

			if( self.readbuffer[0:-1]=='l'):
				print("turning steering to left")
				self.orientation="N"
				self.engine.say('turning steering to left.')
				self.engine.runAndWait()
				self.ser.write('l')
			if( self.readbuffer[0:-1]=='m'):
				print("Hold steering")
				self.orientation="W"
				self.engine.say('Hold steering.')
				self.engine.runAndWait()
				self.ser.write('m')
			if( self.readbuffer[0:-1]=='r'):
				print("turning steering to right")
				self.orientation="E"
				self.engine.say('turning steering to right.')
				self.engine.runAndWait()
				self.ser.write('r')
			self.ser.write('q')
			logvalues=self.ser.readbuffer[0:-1]
			timestring=datetime.datetime.time(datetime.datetime.now())
			if(self.datacount<100000):#save last 100000 GPS data
				loc= open("Datalogs/potentiometer.txt", "a")     
				loc.write("%s %s " %logvalues[0:-2],timestring)
				self.datacount=datacount+1
				loc.write("\n")
				loc.close()
				loc= open("Datalogs/wheelencoder.txt", "a")     
				loc.write("%s %s " %logvalues[-1:],timestring)
				loc.write("\n")
				loc.close()
			else:
				self.datacount=0
				open('Datalogs/potentiometer.txt', 'w').close()
				loc= open("Datalogs/potentiometer.txt", "a")     
				loc.write("%s %s " %logvalues[0:-2],timestring)
				self.datacount=datacount+1
				loc.write("\n")
				loc.close()
				loc= open("Datalogs/wheelencoder.txt", "a")     
				loc.write("%s %s " %logvalues[-1:],timestring)
				loc.write("\n")
				loc.close()	
	

'''
#Testing working
CS=CellphoneServer('192.168.1.3', 8089)
while True:
	CS._listen()
	print(CS.movestatus)
	print(CS.orientation)
'''