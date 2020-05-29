


#include "UserInterface.h"

CUserInterface::CUserInterface()
{
  m_mode = GameState;

  m_vizData.EnvironmentStatus = 0;
  m_vizData.GameState = GamePlaying;
  m_vizData.InternalState = DisplayQuestion; 
  m_vizData.PotentialScore = 5;
  m_vizData.Score = 25; 
  m_vizData.TotalGameSecs = 60;
  m_vizData.RemainingSecs = 5; 
  m_vizData.RemainingQuestionSecs = 5;
} 


CUserInterface::~CUserInterface()
{
} 


SVizData CUserInterface::GetVizData()
{
  return m_vizData;
}


void CUserInterface::DisplayUI(TM1638& tm)
{
  tm.clearDisplay();

  // First write the value, right aligned
  int val = GetCurrentValue();
  if(val > 0)
    tm.setDisplayToDecNumber(GetCurrentValue(), 0, false);
  else
    tm.setDisplayDigit(0, 7, false);
    

  // Then the mode text
  switch(m_mode)
  {
    case GameState: tm.setDisplayToString("GaSt"); break;
    case InternalState: tm.setDisplayToString("InSt"); break;
    case PotentialScore: tm.setDisplayToString("PtSc"); break;
    case Score: tm.setDisplayToString("Scor"); break;
    case TotalGameSecs: tm.setDisplayToString("tSEC"); break;
    case RemainingSecs: tm.setDisplayToString("rSEC"); break;
    case RemainingQuestionSecs: tm.setDisplayToString("qSEC"); break;
    case Env_Demo: tm.setDisplayToString("E-D"); break;
    case Env_Light: tm.setDisplayToString("E-L"); break;
    case Env_Movement: tm.setDisplayToString("E-PP"); break;    
  }

}



void CUserInterface::ChangeMode(int dir)
{
  int tmp = m_mode;
  tmp += dir;
  if(tmp < GameState)
    tmp = Env_Movement;
  if(tmp > Env_Movement)
    tmp = GameState;
  m_mode = (ui_mode)tmp;
}


void CUserInterface::ChangeValue(int dir)
{
  if(m_mode == GameState)
    m_vizData.GameState += dir;
  if(m_mode == InternalState)
    m_vizData.InternalState += dir;
  if(m_mode == PotentialScore)
    m_vizData.PotentialScore += dir;
  if(m_mode == Score)
    m_vizData.Score += dir;
  if(m_mode == TotalGameSecs)
    m_vizData.TotalGameSecs += dir;
  if(m_mode == RemainingSecs)
    m_vizData.RemainingSecs += dir;
  if(m_mode == RemainingQuestionSecs)
    m_vizData.RemainingQuestionSecs += dir;
  if(m_mode == Env_Demo)
    m_Env_Demo += dir;
  if(m_mode == Env_Light)
    m_Env_Light += dir;
  if(m_mode == Env_Movement)
    m_Env_Movement += dir;

  m_vizData.GameState = Confine(m_vizData.GameState, 0, 7);
  m_vizData.InternalState = Confine(m_vizData.InternalState, 0, 3);
  m_vizData.PotentialScore = Confine(m_vizData.PotentialScore, 0, 10);
  m_vizData.Score = Confine(m_vizData.Score, 0, 100);
  m_vizData.TotalGameSecs = Confine(m_vizData.TotalGameSecs, 0, 120);
  m_vizData.RemainingSecs = Confine(m_vizData.RemainingSecs, 0, 50);
  m_vizData.RemainingQuestionSecs = Confine(m_vizData.RemainingQuestionSecs, 0, 20);
  m_Env_Demo = Confine(m_Env_Demo, 0, 4);
  m_Env_Light = Confine(m_Env_Light, 0, 1);
  m_Env_Movement = Confine(m_Env_Movement, 0, 1);
  m_vizData.EnvironmentStatus = GetEnvironmentStatus();
}

int CUserInterface::GetCurrentValue()
{
  switch(m_mode)
  {
    case GameState: return m_vizData.GameState;
    case InternalState: return m_vizData.InternalState;
    case PotentialScore: return m_vizData.PotentialScore;
    case Score: return m_vizData.Score;
    case TotalGameSecs: return m_vizData.TotalGameSecs;
    case RemainingSecs: return m_vizData.RemainingSecs;
    case RemainingQuestionSecs: return m_vizData.RemainingQuestionSecs;
    case Env_Demo: return m_Env_Demo;
    case Env_Light: return m_Env_Light;
    case Env_Movement: return m_Env_Movement;    
  }

  return 0;
}


int CUserInterface::Confine(int val, int min, int max)
{
  if(val < min)
    return min;
  if(val > max)
    return max;
  return val;
}

  
int CUserInterface::GetEnvironmentStatus()
{
  int nStatus = 0;

  if(m_Env_Demo == 1)
    nStatus += 0b00000001;
  if(m_Env_Demo == 1)
    nStatus += 0b00000011;
  if(m_Env_Demo == 1)
    nStatus += 0b00000101;
  if(m_Env_Demo == 1)
    nStatus += 0b00000111;  
  if(m_Env_Light == 1)
    nStatus += 0b00001000; 
  if(m_Env_Movement == 1)
    nStatus += 0b00010000; 

  return nStatus;
}