#define led_red 4
#define led_yellow 5


#define led_green 6
#define led_common_cathode 7
#define transmitter_1 3
#define transmitter_2 2
#define mic A1
#define mic_activator 12
#define level_1 A2
#define level_2 A3
#define level_3 A4
#define level_base A0
int shot=1;
int counter_1=0;
boolean glass_full=false;
boolean control_1=false;
boolean control_2=false;
boolean control_3=false;
boolean control_4=false;
boolean control_5=false;
boolean control_6=false;
boolean control_7=false;
void setup()
{
  initialise(); 
}

void loop()
{
    level_sense();
  music();
  glass_detect_oscillator();

}





void initialise()
{
  Serial.begin(9600);
  pinMode(led_red,OUTPUT);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_common_cathode,OUTPUT);
  pinMode(transmitter_1,OUTPUT);
  pinMode(transmitter_2,OUTPUT);
  pinMode(mic_activator,OUTPUT);
  pinMode(mic,INPUT);
  pinMode(level_1,INPUT);
  pinMode(level_2,INPUT);
  pinMode(level_3,INPUT);
  pinMode(level_base,OUTPUT);
  digitalWrite(led_red,LOW);
  digitalWrite(led_yellow,LOW);
  digitalWrite(led_green,LOW);
  digitalWrite(led_common_cathode,LOW);
  digitalWrite(transmitter_1,HIGH);
  digitalWrite(transmitter_2,HIGH);
  digitalWrite(mic_activator,LOW);
  analogWrite(level_1,0);
  analogWrite(level_2,0);
  analogWrite(level_3,0);
  //analogWrite(level_base,1023);

}




void glass_detect_oscillator()
{
  counter_1 = counter_1 + 1;
  if (counter_1%200==0)
  {
  digitalWrite(transmitter_2,HIGH);
  counter_1=1;
  }
  if (counter_1%100==0)
  {
  digitalWrite(transmitter_2,LOW);
  }
}



int level_detect()
{
  if (analogRead(level_3)>1000)
  {
    return(3);
  }
  if (analogRead(level_2)>1000)
  {
    return(2);
  }
  if (analogRead(level_1)>1000)
  {
    return(1);
  }
  else 
  {
    return(0); 
  }

}

void level_sense()
{

  switch(level_detect())
  {
  case 0: 
    glass_full=false;
    break;
  case 3: 
    glass_full=true;
    break;
  default:;
  }

  if (glass_full || control_1)
  {
    control_1=true;
    if (glass_full==false || control_2)
    {
      control_2=true;
      if (glass_full || control_3)
      {
        control_3=true;
        shot=2;
        if (glass_full==false || control_4)
        {
          control_4=true;
          if (glass_full || control_5)
          {
            control_5=true;
            shot=3;
            if (glass_full==false || control_6)
            {
              control_6=true;
              if (glass_full || control_7)
              {
                control_7=true;
                shot=3;
                digitalWrite(transmitter_1,LOW); 
              }
            } 
          }
        }
      }
    }
  }

}




void music()
{
  int mic_val = analogRead(mic);
  if(mic_val>100)
  {

    switch(shot)
    {
    case 1: 
      digitalWrite(led_green,HIGH); 
      break;
    case 2: 
      digitalWrite(led_yellow,HIGH); 
      break;
    case 3: 
      digitalWrite(led_red,HIGH); 
      break;
      delay(2);
    }
  }
  else
  {

    digitalWrite(led_green,LOW);
    digitalWrite(led_yellow,LOW);
    digitalWrite(led_red,LOW);

  }
}







