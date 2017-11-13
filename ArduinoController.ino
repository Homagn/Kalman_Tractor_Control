
//Author Homagni
#include <Stepper.h>

const int turnpinR = 9;      // turnpins control the signal to steering
const int turnpinL = 8;

const int forwards = 10;      // these control forward and backward acceleration
const int backwards = 11;

boolean turnleft=false;// states indicating which directions it turned
boolean turnright=false;

boolean goback=false; //states indicating accelerations
boolean goforward=false;

int nstate=0; //whether moving forward of backward or standing
int astate=0; //indicates how many acc dcc have been applied


int steerPin = A0;    // analog input pin for steering position sensor
int wheelcounter= 12; //digital input pin for counting wheel rotations



int res=750; //acceleration delay
int acc=500; //acceleration delay

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {
  delay(100);
  byte directionn=0;
  if (Serial.available()) {//possible signals a d l r m f b s q
    directionn = Serial.read();
    //specifying flags for go forward, go backwards and stop command
    if (directionn=='q') //q for querying the sensors attached to the arduino
    {
        int thet = analogRead(steerPin);
        int u = digitalRead(wheelcounter);
        Serial.write(String(thet)+String(u));
    }
    if (directionn=='b')
    {
        goback=true;
        goforward=false;
    }
    if (directionn=='s')
    {
        turnright=false;
        turnleft=false;
        goback=false;
        goforward=false;
    }
    if (directionn=='f')
    {
        goback=false;
        goforward=true;
    }
    if (directionn=='l')//keep turn wheels left
    {
        turnright=false;
        turnleft=true;
        
    }
    if (directionn=='r')//keep turn wheels right
    {
        turnright=true;
        turnleft=false;
        
    }
    if (directionn=='m')//hold the current steering postition
    {
        turnright=false;
        turnleft=false;
        
    }
    if(turnright==true && turnleft==false)
    {
      digitalWrite(turnpinR, HIGH);
      delay(200);
      digitalWrite(turnpinL, LOW);
    }
    if(turnleft==true && turnright==false)
    {
      digitalWrite(turnpinR, LOW);
      delay(200);
      digitalWrite(turnpinL, HIGH);
    }
    if(turnright==false && turnleft==false)
    {
      digitalWrite(turnpinR, LOW);
      delay(200);
      digitalWrite(turnpinL, LOW);
    }
    if(directionn=='a')//command for acceleration
    {
      astate=astate+1;
      if(nstate==1)
      {
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(acc);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(acc);// wait 2 seconds
       res=res+acc;
      }
      if(nstate==-1) //if previous state was going backwards we have to push the gas longer to achieve acceleration position
      {
       digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       res=res+acc;
      }
     
    }
    if(directionn=='d')//command for decceleration
    {
      astate=astate-1;
      if(nstate==1)
      {
       digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       res=res-acc;
      }
      if(nstate==-1)
      {
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(acc);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(acc);// wait 2 seconds
       res=res-acc;
      }
     
    }

  
  if(goback==true && goforward==false)//going backwards
  {
    if(nstate==0)
    {
       digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       
       nstate=nstate-1;
    }
    if(nstate==1)
    {
       digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       
       nstate=nstate-1;

       digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       nstate=nstate-1;
       
    }
    
  }
  if(goback==false && goforward==true)//moving forward
  {
    if(nstate==0)
    {
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(res);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
      
       nstate=nstate+1;
    }
    if(nstate==-1)
    {
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(res);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
      
       nstate=nstate+1;
       
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(res);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       
       nstate=nstate+1;
    }
  }
  if(goforward==false && goback==false)//coming to a stop
  {
    if(nstate==-1)
    {
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, LOW);//Activate the relay the other direction, they must be different to move the motor
       delay(res);// wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       
       nstate=nstate+1;
    }
    if(nstate==1)
    {
      digitalWrite(forwards, LOW);
       digitalWrite(backwards, HIGH);//Activate the relay one direction, they must be different to move the motor
       delay(res); // wait 2 seconds
      
       digitalWrite(forwards, HIGH);
       digitalWrite(backwards, HIGH);//Deactivate both relays to brake the motor
       delay(res);// wait 2 seconds
       
       nstate=nstate-1;
    }
  }
  
  }
}

