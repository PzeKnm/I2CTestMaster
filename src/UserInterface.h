

#ifndef __CUserInterface_H__
#define __CUserInterface_H__

#include "Arduino.h"
// #include "Protocol.h"
#include "MoreOrLessProtocol.h"
#include "TM1638.h"

class CUserInterface
{

public:
	CUserInterface();
	~CUserInterface();

  void DisplayUI(TM1638& tm);

  SVizData GetVizData();

  void ChangeMode(int dir);

  void ChangeValue(int dir); 

  enum ui_mode
  {
    GameState = 0,
    InternalState,
    PotentialScore,
    Score,
    TotalGameSecs,
    RemainingSecs,
    RemainingQuestionSecs,
    Env_Demo,
    Env_Light,
    Env_Movement
  };

private:

  int GetCurrentValue();

  int Confine(int val, int min, int max);

  int GetEnvironmentStatus();

  ui_mode m_mode;

  SVizData m_vizData;
  int m_Env_Demo = 0;
  int m_Env_Light = 1;
  int m_Env_Movement = 0;
};

#endif //__CUserInterface_H__






