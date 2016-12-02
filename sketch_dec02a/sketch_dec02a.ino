#define irwp 5
#define irwd 13 
#define irep 7
#define ired 8
#define westRed 2
#define westYellow 3
#define westGreen 4
#define eastRed 10
#define eastYellow 11
#define eastGreen 12
#define yellowBlinkTime 500 // 0.5 seconds for yellow light blink
int trafficWest = 1; // west = true, east = false
int flowTime = 10000; // amount of time to let traffic flow
int changeDelay = 2000; // amount of time between color changes
int west=0;
int east=0;
int x=1;
int y=0;
int c1=0,c2=0,c3=0,c4=0;
void setup()
{   Serial.begin(9600);
    // setup digital I/O pins
    servoEast.attach(0);  // Set east servo to digital pin 10
    servoWest.attach(1);// Set west servo to digital pin 
    pinMode(irwp, INPUT);
    pinMode(irep, INPUT);
    pinMode(irwd, INPUT);
    pinMode(ired, INPUT);
    pinMode(westRed, OUTPUT);
    pinMode(westYellow, OUTPUT);
    pinMode(westGreen, OUTPUT);
    pinMode(eastRed, OUTPUT);
    pinMode(eastYellow, OUTPUT);
    pinMode(eastGreen, OUTPUT);
    // set initial state for lights - west side is green first
    digitalWrite(westRed, LOW);
    digitalWrite(westYellow, LOW);
    digitalWrite(westGreen, HIGH);
    digitalWrite(eastRed, HIGH);
    digitalWrite(eastYellow, LOW);
    digitalWrite(eastGreen, LOW);  
}

void loop()
{
  comp();
  if ( x == 1 ) // request west>east traffic flow
        {
          
        if ( trafficWest != 1 )
        // only continue if traffic flowing in the opposite (east) direction
        {
          trafficWest = 1; // change traffic flow flag to west>east           
          digitalWrite(eastGreen, LOW); // change east-facing lights from green
          delay(500);
          servoWest.write(90);   // Turn Servo Left to 0 degrees
          delay(500);
          // to yellow to red
          digitalWrite(eastYellow, HIGH);
          moddelay(changeDelay);
          digitalWrite(eastYellow, LOW);
          digitalWrite(eastRed, HIGH);          
          moddelay(changeDelay);
          for ( int a = 0; a < 3; a++ ) // blink yellow light
            {
                digitalWrite(westYellow, LOW);
                moddelay(yellowBlinkTime);
                digitalWrite(westYellow, HIGH);
                moddelay(yellowBlinkTime);
            }
          digitalWrite(westYellow, LOW);
          digitalWrite(westRed, LOW); // change west-facing lights from red to green
          digitalWrite(westGreen, HIGH);
        }
        
        }
        if ( y == 1) // request east>west traffic flow
        {
          
                if ( trafficWest == 1 )
                // only continue if traffic flow is in the opposite (west) direction
                {
                trafficWest = 0; // change traffic flow flag to east>west                
                digitalWrite(westGreen, LOW);
                delay(500);
                servoWest.write(0);   // Turn Servo Left to 0 degrees
                delay(500);
                // change west lights from green to yellow to red
                digitalWrite(westYellow, HIGH);
                moddelay(changeDelay);
                digitalWrite(westYellow, LOW);                
                digitalWrite(westRed, HIGH);
                moddelay(changeDelay);
                for ( int a = 0 ; a < 5 ; a++ ) // blink yellow light
                {
                      digitalWrite(eastYellow, LOW);
                      moddelay(yellowBlinkTime);
                      digitalWrite(eastYellow, HIGH);
                      moddelay(yellowBlinkTime);
                }
                digitalWrite(eastYellow, LOW);
                digitalWrite(eastRed, LOW); // change east-facing lights from red to green
                digitalWrite(eastGreen, HIGH);
                }
        }
        moddelay(flowTime);// give time for traffic to flow
}

void moddelay(int t)
{   
    c1=0;
    c2=0;
    c3=0;
    c4=0;
    for(int i=1;i<=t;i++)
    {
        if(digitalRead(irwd) == HIGH )
        {
            c1=1;
        }
        if(digitalRead(irwd) == LOW && c1==1)
        {
              west++;
              disp();
              c1=0;
        }
        if(digitalRead(irwp) == HIGH )
        { 
            c2=1;
        }
        if(digitalRead(irwp) == LOW && c2==1)
        {
              west--;
              disp();
              c2=0;
        }
        if(digitalRead(ired) == HIGH )
        {
            c3=1;
        }
        if(digitalRead(ired) == LOW && c3==1)
        {
              east++;
              disp();
              c3=0;
        }
        if(digitalRead(irep) == HIGH )
        {
              c4=1;
        }
        if(digitalRead(irep) == LOW && c4==1)
        {
               east--;
               disp();
               c4=0;
        }
        delay(1);   
    }

}
void disp()
{
  Serial.print(west);
  Serial.print("west \t");
  Serial.print(east);
  Serial.print("east \n");
}
void comp(){
    if(west>east)
      {x=1;y=0;}
    else if(west<east)
      {y=1;x=0;}
}
