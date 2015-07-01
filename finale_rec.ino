#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

boolean developers_mode=false;
boolean checkpoint=false;
boolean storage_fault_locator=false;
boolean gsm_data_print=true;

int mode=1;
int network_check_repeater=0;
int gsm_reply_checking_integer=1;
char gsm_stored_data[40];
int gsm_stimulator=0;
int lcd_column=0;
int lcd_row=0;
char gsm_reply;

int liquid_crystal_display_pins[16] = {
  0,53,51,49,47,45,0,0,0,0,43,41,39,37,35,33};
//button start
int phone_no[11] = {
  0,9,7,6,9,6,2,6,4,0,9};

//#define gsm_rx
//#define gsm_tx
//SoftwareSerial gsm(gsm_tx, gsm_rx); // RX, TX
#define key_0 A11
#define key_1 A12
#define key_2 A8
#define key_3 A7
#define key_4 A13
#define key_5 A9
#define key_6 A6
#define key_7 A14
#define key_8 A10
#define key_9 A5
#define key_l A15
#define key_r A4
//#define keypad_activate 13
//#define network_led 3
//#define pwr_led 3             

//button end



#define keypad_1 A12
#define keypad_2 A8
#define keypad_3 A7
#define keypad_4 A13
#define keypad_5 A9
#define keypad_6 A6
#define keypad_7 A14
#define keypad_8 A10
#define keypad_9 A5
#define keypad_0 A11

#define keypad_l A15
#define keypad_r A4
#define gsm_rx 11
#define gsm_tx 10

#define receiver_1 34
#define receiver_2 36
#define receiver_3 38


#define led_common_cathode 50 

#define gsm_led_red 48
#define gsm_led_green 52
#define power_led_red 46
#define power_led_green 44
#define glass_led_red 40
#define glass_led_green 42
#define lcd_led_anode liquid_crystal_display_pins[14]
#define lcd_led_cathode liquid_crystal_display_pins[15]
#define lcd_contrast liquid_crystal_display_pins[2] 

LiquidCrystal lcd(liquid_crystal_display_pins[3], liquid_crystal_display_pins[5], liquid_crystal_display_pins[10],liquid_crystal_display_pins[11],liquid_crystal_display_pins[12],liquid_crystal_display_pins[13]);
SoftwareSerial gsm(gsm_tx, gsm_rx);


//******************************************************************************SETUP FUNCTION******************************************************************************



void setup()
{
  if (developers_mode)
  {
    Serial.println("By Default Developers Mode is ON !"); 
    developing_state();

  }
  initialize();
  start_button();
  if (mode!=3)
  {
    enter_phone_no();
    splash_screen();
  }
}


//******************************************************************************LOOP FUNCTION******************************************************************************




void loop()
{
  switch(mode)
  {
  case 1:
    while(1)
    {
      gsm_check();
      drunk_detected();
      glass_detect(100000);
    }
  case 2:
    while(1)
    {
      just_gsm();
    }

  case 3:
    gsm_direct_communication_initialiser();
    while(1)
    {
      gsm_direct_communication();
    }
  }


}


void miss_call()
{
  gsm_check();
  int check=0;
  gsm_check();
  check=network_check(1);
  if (check==2)
  {
    if (developers_mode)
    {
      Serial.print("Computer: ATD");

      if (phone_no[0]=='0')
      {
        for (int pn=0;pn<11;pn++)
        {
          Serial.print(phone_no[pn]);
        }
      }

      else
      {
        Serial.print("0");
        for (int pn=0;pn<10;pn++)
        {
          Serial.print(phone_no[pn]);
        }
      } 
      Serial.println(";");
    }
    gsm.print("ATD");

    if (phone_no[0]=='0')
    {
      for (int pn=0;pn<11;pn++)
      {
        gsm.print(phone_no[pn]);
      }
    } 
    else
    {
      gsm.print("0");
      for (int pn=0;pn<10;pn++)
      {
        gsm.print(phone_no[pn]);
      }
    } 
    gsm.println(";");
    gsm_reply_checking_integer=1;
    while(gsm_reply_checking_integer)
    {
      gsm_check();
      network_check(1);
    }
    delay(10000);           
    if (developers_mode)
    {
      Serial.println("Computer: ATH");
    }
    gsm.println("ATH");
    gsm_reply_checking_integer=1;
    while(gsm_reply_checking_integer)
    {
      gsm_check();
      network_check(1);
    }

    delay(3000);    
  }
}

void glow_test()
{

  digitalWrite(gsm_led_red,LOW);
  digitalWrite(gsm_led_green,LOW);
  digitalWrite(power_led_red,LOW);
  digitalWrite(power_led_green,LOW);
  digitalWrite(glass_led_red,LOW);
  digitalWrite(glass_led_green,LOW);
  digitalWrite(led_common_cathode,LOW);
  digitalWrite(lcd_led_anode,LOW);

  delay(100); 

  digitalWrite(gsm_led_red,HIGH);
  digitalWrite(power_led_red,HIGH);
  digitalWrite(glass_led_red,HIGH);
  digitalWrite(lcd_led_anode,HIGH);
  delay(600);
  digitalWrite(glass_led_red,LOW);
  digitalWrite(power_led_red,LOW);
  digitalWrite(gsm_led_red,LOW);
  delay(100);

  digitalWrite(gsm_led_green,HIGH);
  digitalWrite(power_led_green,HIGH);
  digitalWrite(glass_led_green,HIGH);
  delay(600);

  digitalWrite(power_led_green,LOW);
  digitalWrite(glass_led_green,LOW);
  digitalWrite(gsm_led_green,LOW);
  digitalWrite(lcd_led_anode,LOW);
  delay(100);
  digitalWrite(lcd_led_anode,HIGH);
  digitalWrite(power_led_green,HIGH);
}

void initialize()
{



  pinMode(keypad_1,INPUT); 
  pinMode(keypad_2,INPUT); 
  pinMode(keypad_3,INPUT); 
  pinMode(keypad_4,INPUT); 
  pinMode(keypad_5,INPUT); 
  pinMode(keypad_6,INPUT); 
  pinMode(keypad_7,INPUT); 
  pinMode(keypad_8,INPUT); 
  pinMode(keypad_9,INPUT); 
  pinMode(keypad_0,INPUT); 
  pinMode(keypad_l,INPUT); 
  pinMode(keypad_r,INPUT);

  pinMode(receiver_1,INPUT);
  pinMode(receiver_2,INPUT);
  pinMode(receiver_3,INPUT);  
  pinMode(gsm_led_red,OUTPUT); 
  pinMode(gsm_led_green,OUTPUT); 
  pinMode(power_led_red,OUTPUT); 
  pinMode(power_led_green,OUTPUT); 
  pinMode(glass_led_red,OUTPUT); 
  pinMode(glass_led_green,OUTPUT);
  pinMode(led_common_cathode,OUTPUT);

  pinMode(liquid_crystal_display_pins[1],OUTPUT); 
  pinMode(liquid_crystal_display_pins[4],OUTPUT); 
  pinMode(lcd_led_anode,OUTPUT); 
  pinMode(lcd_led_cathode,OUTPUT); 
  pinMode(lcd_contrast,OUTPUT); 


  analogWrite(keypad_1,0);
  analogWrite(keypad_2,0);
  analogWrite(keypad_3,0);
  analogWrite(keypad_4,0);
  analogWrite(keypad_5,0);
  analogWrite(keypad_6,0);
  analogWrite(keypad_7,0);
  analogWrite(keypad_8,0);
  analogWrite(keypad_9,0);
  analogWrite(keypad_0,0);
  analogWrite(keypad_l,0);
  analogWrite(keypad_r,0);
  digitalWrite(receiver_1,LOW);
  digitalWrite(receiver_2,LOW);
  digitalWrite(receiver_3,LOW);

  digitalWrite(gsm_led_red,LOW);
  digitalWrite(gsm_led_green,LOW);
  digitalWrite(power_led_red,LOW);
  digitalWrite(power_led_green,LOW);
  digitalWrite(glass_led_red,LOW);
  digitalWrite(glass_led_green,LOW);
  digitalWrite(led_common_cathode,LOW);

  digitalWrite(liquid_crystal_display_pins[1],HIGH);
  digitalWrite(liquid_crystal_display_pins[4],LOW);
  digitalWrite(lcd_led_anode,LOW);
  digitalWrite(lcd_led_cathode,LOW);



  digitalWrite(lcd_contrast,LOW);

  lcd.begin(16,2);

  Serial.begin(9600);

  gsm.begin(9600);

}

void start_button()
{

  /*
   0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
   * * * P R E S S   Z E R O * * *
   * * * * T O   S T A R T * * * *
   
   */
  digitalWrite(power_led_red,HIGH);   
  lcd.setCursor(3,0);
  lcd.print("PRESS ZERO");
  lcd.setCursor(4,1);
  lcd.print("TO START");
  while(true)
  {

    gsm_check();
    if(digitalRead(keypad_0))
    {
      mode=1;
      break;
    }
    else if(digitalRead(keypad_l))
    {
      mode=1;
      developers_mode=true;
      digitalWrite(glass_led_red,HIGH);
      delay(1000);
      digitalWrite(glass_led_red,LOW);
      break;
    }
    else if(digitalRead(keypad_r))
    {
      mode=2;
      /*
 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
       * * * * J U S T   G S M * * * *
       * * M O D E   E N T E R E D * *
       
       */

      lcd.clear();
      digitalWrite(lcd_led_anode,HIGH);
      lcd.setCursor(4,0);
      lcd.print("Just GSM");
      lcd.setCursor(2,1);
      lcd.print("Mode Entered");
      delay(1000);
      break;
    }

    else if(digitalRead(keypad_1))
    {
      mode=3;
      lcd.clear();
      digitalWrite(lcd_led_anode,HIGH);
      break;
    }

  }
  digitalWrite(lcd_led_anode,HIGH);
  lcd.clear();
  lcd.print("TEXTING TUMBLER");
  delay(1000);
  lcd.clear();
  if (developers_mode)
  {
    lcd.print(" The Glow Test");
    delay(1000);
    glow_test();
    lcd.clear();
  }
  digitalWrite(power_led_red,LOW);
  digitalWrite(power_led_green,HIGH); 
}

int button_press()
{

  if (digitalRead(key_0))
  {
    while(digitalRead(key_0));

    return(0); 

  }
  if (digitalRead(key_1))
  {
    while(digitalRead(key_1));

    return(1); 

  }
  if (digitalRead(key_2))
  {
    while(digitalRead(key_2));

    return(2); 

  }
  if (digitalRead(key_3))
  {
    while(digitalRead(key_3));

    return(3); 

  }
  if (digitalRead(key_4))
  {
    while(digitalRead(key_4));

    return(4); 

  }
  if (digitalRead(key_5))
  {
    while(digitalRead(key_5));

    return(5); 

  }
  if (digitalRead(key_6))
  {
    while(digitalRead(key_6));

    return(6); 

  }
  if (digitalRead(key_7))
  {
    while(digitalRead(key_7));

    return(7); 

  }
  if (digitalRead(key_8))
  {
    while(digitalRead(key_8));

    return(8); 

  }
  if (digitalRead(key_9))
  {
    while(digitalRead(key_9));
    delay(100);  //For correction of double pressing of key 9
    return(9); 

  }
  if (digitalRead(key_l))
  {
    while(digitalRead(key_l));

    return(10); 

  }
  if (digitalRead(key_r))
  {
    while(digitalRead(key_r));
    lcd.clear();
    return(11); 

  }

  return(12);
}

void enter_phone_no()
{
  lcd.setCursor(1,0);
  lcd.print("Enter Phone No");

  int p=0;
  int button;
  int remain=1;

  while(remain)
  {
    gsm_check();
    network_check(2);
    button = button_press();
    delay(10);  //For preventing repetition of buttons 
    if (button < 10)
    {
      phone_no[p]=button;
      lcd.setCursor(p+2,1);
      lcd.print(button);
      p++;
    }
    if (button == 10)
    {
      lcd.clear();
      p=0;
      enter_phone_no();   
    }
    if (button == 11)
    {
      remain = 0; 
    }
  }

  if (developers_mode)
  {
    Serial.print("Entered Phone No = ");
    for(int ph=0;ph<11;ph++)
    {
      Serial.print(phone_no[ph]);
    }
    Serial.println();
  }

}


void gsm_check()
{
  if (gsm.available())
  { 
    for(int hh=0;hh<40;hh++)
    {
      gsm_stored_data[hh]='*'; 
    }
    gsm_reply_checking_integer=0;

    gsm_stored_data[0]=gsm.read();

    int i=1;
    while(gsm.available())
    {
      if (i==39)
      {
        i=0; 
      }
      gsm_stored_data[i]=gsm.read();
      i++;
    }
    network_check(1);
  } 
}






int network_check(int network_checking_integer)
{





  for (int t=0;t<40;t++)
  {
    //    checkpoint_checker_function(5);
    if (gsm_stored_data[t]=='C' || gsm_stored_data[t]=='R')
    {



      if (gsm_stored_data[t+1]=='P' || gsm_stored_data[t+1]=='F' || gsm_stored_data[t+1]=='E')
      {

        digitalWrite(gsm_led_green,LOW);
        digitalWrite(gsm_led_red,HIGH);




        return(1);
      }
      else if (gsm_stored_data[t+1]=='a' || gsm_stored_data[t+1]=='e')
      {
        digitalWrite(gsm_led_red,LOW);
        digitalWrite(gsm_led_green,HIGH);
        return(2); 
      }
    }
  }
  return(0);

}

/*
void checkpoint_checker_function (int k)
 {
 if(checkpoint_checker_checkpost)
 {
 Serial.print("Checkpoint Checker  ");
 Serial.println(k); 
 }
 
 }
 */
void sending_sms()
{
  digitalWrite(gsm_led_green,LOW);
  digitalWrite(gsm_led_red,LOW);
  if (developers_mode)
  {
    Serial.println("Computer: AT+CMGF=1");
  }
  gsm.println("AT+CMGF=1");
  gsm_reply_checking_integer=1;
  while(gsm_reply_checking_integer)
  {
    gsm_check();
  }
  delay(2000);
  if (developers_mode)
  {
    Serial.print("Computer: AT+CMGS=\"");
    for (int pn=0;pn<11;pn++)
    {
      Serial.print(phone_no[pn]);
    } 
    Serial.println("\"");
  }
  gsm.print("AT+CMGS=\"");
  if (phone_no[0]==0)
  {
    for (int pn=0;pn<11;pn++)
    {
      gsm.print(phone_no[pn]);
    }
  } 
  else
  {
    gsm.print("0");
    for (int pn=0;pn<10;pn++)
    {
      gsm.print(phone_no[pn]);
    }
  } 

  gsm.println("\"");
  gsm_reply_checking_integer=1;
  while(gsm_reply_checking_integer)
  {
    gsm_check();
  }
  digitalWrite(gsm_led_green,HIGH);
  delay(2000);   
  if (developers_mode)
  {
    Serial.println("Computer: Help Me!, I am drinking too much alcohol and I am out of control!");
  }
  gsm.print("Help Me ! , I am drinking too much alcohol and I am out of control !");
  if (developers_mode)
  {
    Serial.println();  
  }
  gsm_reply_checking_integer=1;
  while(gsm_reply_checking_integer)
  {
    gsm_check();
  }
  digitalWrite(gsm_led_green,LOW);
  delay(2000);
  if (developers_mode)
  {
    Serial.println("Computer: Ctrl + z");
  }
  gsm.write(0x1A);
  // delay(10);
  if (developers_mode)
  {
    Serial.println("Computer: Carriage Return");
  }
  gsm.write(0x0D);
  // delay(10);
  if (developers_mode)
  {
    Serial.println("Computer: Line Feed");
  }
  gsm.write(0x1A);
  gsm_reply_checking_integer=1;
  while(gsm_reply_checking_integer)
  {
    gsm_check();
  }
  if (developers_mode)
  {
    Serial.println("Completed Sending SMS");
  }
  digitalWrite(gsm_led_green,HIGH);
  gsm_reply_checking_integer=1;
}

void serial_gsm_command()
{
  if (Serial.available())
  {


    char c= Serial.read();
    switch(c)
    {
    case '1': 
      Serial.println("Computer: AT");
      gsm.println("AT");
      break;



    case '2': 
      Serial.print("Computer: ATD");
      gsm.print("ATD");



      if (phone_no[0]==0)
      {
        for (int kk=0;kk<11;kk++)
        {
          Serial.print(phone_no[kk]);
        }
        Serial.println(";");

        for (int kk=0;kk<11;kk++)
        {
          gsm.print(phone_no[kk]);
        }
        gsm.println(";");
      }



      else
      {
        Serial.print("0");
        for (int kk=0;kk<10;kk++)
        {
          Serial.print(phone_no[kk]);
        }
        Serial.println(";");

        gsm.print("0");
        for (int kk=0;kk<10;kk++)
        {
          gsm.print(phone_no[kk]);
        }
        gsm.println(";");
      }

      break;




    case '3': 
      Serial.println("Computer: ATH");
      gsm.println("ATH");
      break;
    case '4': 
      Serial.println("Computer: ATA");
      gsm.println("ATA");

      break;
    case '5': 
      Serial.println("Computer: AT+CMGF=1");
      gsm.println("AT+CMGF=1");
      break;
    case '6': 
      Serial.println("Computer: AT+CSCA=\"GSM\""); // Dont Use !
      gsm.println("AT+CSCA=\"GSM\"");
      break;
    case '7': 
      Serial.println("Computer: AT+CMGS=\"0976926409\"");
      gsm.println("AT+CMGS=\"09769626409\"");             
      break;
    case '8': 
      for(;;)
      {
        miss_call(); 
      }
      break;
    case '9': 
      Serial.println("Computer: BOW DOWN TO ME, I AM GSM MODULE MWAHA HA HA HA ! and also KTB IS KING");
      gsm.print("BOW DOWN TO ME, I AM GSM MODULE MWAHA HA HA HA ! and also KTB IS KING");
      Serial.println();  
      break;
    case 'a': 
      Serial.println("Computer: Ctrl + z");
      gsm.write(0x1A);
      break;
    case 'b': 
      Serial.println("Computer: Carriage Return");
      gsm.write(0x0D);
      break;
    case 'c': 
      Serial.println("Computer: Line Feed");
      gsm.write(0x1A);
      break;
    case 't':
      for(int y=0;y<30;y++)
        Serial.print(gsm_stored_data[y]);
      Serial.println();
      break;
    case 's':
      sending_sms();
      break;
    case 'k':
      gsm.println("AT+IPR=9600");
      break;
    default:  
      Serial.println("Switch Case Defaulted");

    }
  }

}

void developing_state()
{
  Serial.println("            DEVELOPING STATE"); 
  Serial.println(); 
  Serial.println(); 
  Serial.println(); 
  Serial.println(); 

  Serial.print("Developers Mode BOOLEAN= ");
  Serial.println(developers_mode); 

  Serial.print("Checkpoint BOOLEAN= ");
  Serial.println(checkpoint); 


  Serial.print("Storage Fault Locator BOOLEAN= ");
  Serial.println(storage_fault_locator); 
}


void receiver_check()
{
  Serial.println("       RECEIVER CHECK");
  Serial.println();
  Serial.print("Receiver Data 1 = ");
  Serial.println(digitalRead(receiver_1)); 
  Serial.print("Receiver Data 2 = ");
  Serial.println(digitalRead(receiver_2)); 
  Serial.print("Receiver Data 3 = ");
  Serial.println(digitalRead(receiver_3)); 
  Serial.println();
  Serial.println();
  delay(500);
}


int glass_detect(long glass_times)
{
  if (glass_range(glass_times)==1)
  {
    digitalWrite(glass_led_red,LOW);
    digitalWrite(glass_led_green,HIGH); 
    return(1);   
  }
  if (glass_range(glass_times)==0)
  {
    digitalWrite(glass_led_green,LOW);
    digitalWrite(glass_led_red,HIGH);
    return(0);    
  }

}

int glass_range(long times)
{

  if (digitalRead(receiver_2))
  {
    for (long check_multiple=0;check_multiple<times;check_multiple++)
    {

      if (digitalRead(receiver_2)==0)
      {

        return(1);
      }

    }
    return(0);
  }
  if (digitalRead(receiver_2)==0)
  {

    for (long check_multiple=0;check_multiple<times;check_multiple++)

    {
      if (digitalRead(receiver_2)==1)
      {
        return(1);
      }

    }

    return(0);
  } 
}

void receiver_data_print()
{
  if (developers_mode)
  {
    receiver_check();
    delay(100); 
  }

}


void splash_screen()
{
  lcd.clear();
  if (mode != 2)
  {
    lcd.setCursor(4,0);
    lcd.print("TEXTING");
    lcd.setCursor(4,1);
    lcd.print("TUMBLER"); 
  }

}


/*
 0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
 * * * * T E X T I N G * * * * *
 * * * * T U M B L E R * * * * *
 
 */


int button=0;

void just_gsm()
{
  if (gsm.available())
  {
    gsm_reply=gsm.read();
    //   Serial.println(gsm_didnt_reply);
    if (lcd_column==16)
    {
      lcd_row++;
      lcd_column=0;
      if(lcd_row==2)
      {
        lcd_row=0;
      }  
    }

    lcd.setCursor(lcd_column,lcd_row);

    if ( int(gsm_reply) != -1 && int(gsm_reply) != 13 && int(gsm_reply) != 10 )
    {
      lcd.write(gsm_reply);
      lcd_column++;
    }  

    else if (int (gsm_reply==-1))
    {
      lcd.clear();
      lcd_row=0;
      lcd_column=0;
    }
  }
  button=button_press();
  if (button != 12)
  {
    numpad_gsm_command(button);
  }

}


void numpad_gsm_command(int gsm_command_button)
{
  lcd.clear();
  lcd_column=0;
  lcd_row=0;
  switch(gsm_command_button)
  {
  case 1: 
    gsm.println("AT");
    break;



  case 2: 
    gsm.print("ATD");



    if (phone_no[0]==0)
    {
      for (int kk=0;kk<11;kk++)
      {
        gsm.print(phone_no[kk]);
      }
      gsm.println(";");
    }



    else
    {
      gsm.print("0");
      for (int kk=0;kk<10;kk++)
      {
        gsm.print(phone_no[kk]);
      }
      gsm.println(";");
    }

    break;




  case 3: 
    gsm.println("ATH");
    break;
  case 4: 
    gsm.println("ATA");

    break;
  case 5: 
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Missed Call");
    lcd.setCursor(2,1);    
    lcd.print("Mode Entered");
    while(button_press()!=10)
    {
      miss_call(); 
    }
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Exiting");
    lcd.setCursor(0,1);    
    lcd.print("Missed Call Mode");
    delay(1000);
    break;
  case 6: 
    sending_sms();
    break;
  case 7: 
    gsm.println("ATZ");
    break;
  default:  
    if (developers_mode)
    {
      Serial.println("Switch Case Defaulted");
    }
  }

}




void gsm_direct_communication()
{
  if (gsm.available())
  {
    Serial.write(gsm.read()); 
  }
  if (Serial.available())
  {
    gsm.write(Serial.read());
  }

}


void gsm_direct_communication_initialiser()
{
  digitalWrite(power_led_red,LOW);
  digitalWrite(gsm_led_red,LOW);
  digitalWrite(glass_led_red,LOW);
  digitalWrite(power_led_green,HIGH);
  digitalWrite(gsm_led_green,HIGH);
  digitalWrite(glass_led_green,HIGH);
  /*
     0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
   G S M   D I R E C T   C O M M *
   * * * A C T I V A T E D * * * *
   
   */
  lcd.clear();
  lcd.print("GSM DIRECT COMM");
  lcd.setCursor(3,1);
  lcd.print("ACTIVATED");
}

void drunk_detected()
{
  if (network_check(2) && digitalRead(receiver_3)==0 && glass_detect(100000))
  {
    sending_sms();
  } 
}

