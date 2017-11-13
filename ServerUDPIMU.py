import socket
import math
import pyttsx
import datetime

# UDP_IP = "192.168.1.3"
# #UDP_IP = "10.27.176.183"
# #UDP_IP = "192.168.42.23"
# UDP_PORT = 5555

class IMU(object):
    def __init__(self,UDP_IP,UDP_PORT):
        open('Datalogs/yaw.txt', 'w').close()
        self.datacount=0
        self.UDP_IP=UDP_IP
        self.UDP_PORT=UDP_PORT
        self.engine = pyttsx.init()
        self.engine.setProperty('voice', 'HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\TTS_MS_EN-US_ZIRA_11.0')
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP
        self.sock.bind((self.UDP_IP, self.UDP_PORT))

    def _listen(self): #need intellugent sensing algorithms based on sensed yaw
        self.yaw=0.0
        self.data, self.addr = self.sock.recvfrom(2056) # buffer size is 1024 bytes
        print "received message:", self.data
        
        try:
            print "yaw: ",float(self.data.split(',')[14][1:-1])
            self.yaw=float(self.data.split(',')[14][1:-1])
        except:
            print "Get lost"
        
        if(self.datacount<100000):
            self.datacount=datacount+1
            timestring=datetime.datetime.time(datetime.datetime.now()))
            yy= open("Datalogs/yaw.txt", "a")     
            yy.write("%s %s " %str(self.yaw),timestring)
            yy.write("\n")
            yy.close()
        else:
            datacount=0
            open('Datalogs/yaw.txt', 'w').close()
            timestring=datetime.datetime.time(datetime.datetime.now()))
            yy= open("Datalogs/yaw.txt", "a")     
            yy.write("%s %s " %str(self.yaw),timestring)
            yy.write("\n")
            yy.close()





    	