#include <Arduino.h>

// #include "Protocol.h"
#include "MoreOrLessProtocol.h"
#include "UserInterface.h"
#include "TimeoutEvent.h"

#include "TM1638.h"


#define SERIAL_BAUD 115200
#define LED 2

#define I2C_SLAVE_ADDR_POTENTIAL_SCORE  0x2b 
#define SLAVE_ADDR_ROBOTPIG  0x2d 
#define I2C_BROADCAST  0x0 

CTimeoutEvent m_evtHeartbeat;
CMoreOrLessProtocol m_protocol;

#define Pin_Data 7
#define Pin_Clk 8
#define Pin_Strobe 9
// intensity 0 - 7
#define DisplayIntensity 0
TM1638 m_tm1638(Pin_Data, Pin_Clk, Pin_Strobe, true, DisplayIntensity);

CTimeoutEvent m_evtCheckUI;
// To avoid bounce, buttons must go through 0 before repeat
bool m_bButtonsClear = true;

CUserInterface m_UI;


void PulsePin(int nPin, int msDelay)
{
  digitalWrite(nPin, HIGH);   
  delay(msDelay);             
  digitalWrite(nPin, LOW);   
}

/*
void SendData()
{
  int nSlaveAddr = I2C_SLAVE_ADDR;
  int nPacketSize = 4;
  int data[nPacketSize];
  data[0] = 12;
  data[1] = 12;
  data[2] = 12;
  data[3] = 12;
  m_protocol.SendDataToSlave(nSlaveAddr, data, nPacketSize);
}*/

void SendVizData()
{
  m_tm1638.setLED(TM1638_COLOR_RED, 7);
 // m_protocol.SendVizDataToSlave(I2C_SLAVE_ADDR_POTENTIAL_SCORE, m_UI.GetVizData());
 // m_protocol.SendVizDataToSlave(SLAVE_ADDR_ROBOTPIG, m_UI.GetVizData()); 
  m_protocol.SendVizDataToSlave(I2C_BROADCAST, m_UI.GetVizData());
   
  delay(100);
  m_tm1638.setLED(TM1638_COLOR_NONE, 7);  
}


void setup() 
{ 
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  while(!Serial) {} // Wait
  Serial.println("Program begins.");

  pinMode(LED, OUTPUT);

  PulsePin(LED, 500);
     
  m_protocol.Setup(0, false);
  m_evtHeartbeat.SetTimeout(10000);
  m_evtCheckUI.SetTimeout(200);
  m_UI.DisplayUI(m_tm1638);
} 

void CheckTM1638() 
{
  byte btButtons = m_tm1638.getButtons();

  if(btButtons == 0)
    m_bButtonsClear = true;
  else
  {
    if(m_bButtonsClear == true)
    {
      Serial.print("Button: ");Serial.println(btButtons);

      if(btButtons == 1)
        m_UI.ChangeMode(-1);
      if(btButtons == 2)
        m_UI.ChangeMode(1);

      if(btButtons == 4)
        m_UI.ChangeValue(-1);
      if(btButtons == 8)
        m_UI.ChangeValue(1);  

      if(btButtons == 128)
        SendVizData();             

      m_UI.DisplayUI(m_tm1638);
    }

    m_bButtonsClear = false;
  }
  

}
  

void loop() 
{  
 if(m_evtHeartbeat.HasTimedout())
  {
    m_evtHeartbeat.ResetTimer();
    PulsePin(LED, 20);   
    // SendVizData(); 
  }  

  if(m_evtCheckUI.HasTimedout())
    CheckTM1638();
} 