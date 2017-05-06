/**********************************************************************************************************************
File: user_app1.c                                                                
zeyun wang
----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 UserApp1_u8FinalCode[10]=0;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
static u8 u8ButtonValue(void);
static u8 u8ConfirmValue(void);
void UserApp1_ChooseMode(void);
void UserApp1_SetCode(void);
static u8 u8ButtonValue(void);
static u8 u8ConfirmValue(void);

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
Function: u8ButtonValue

Description:To get the button return value(0,1,2)*/

static u8 u8ButtonValue(void)
{
  u8 u8GetValue=4;
  if(IsButtonPressed(BUTTON0))
  {
    u8GetValue=0;
  }
  if(IsButtonPressed(BUTTON1))
  {
     u8GetValue=1;
  }
  if(IsButtonPressed(BUTTON2))
  {
     u8GetValue=2;
  }
  return u8GetValue;
}/*end u8ButtonValue()*/


/*----------------------------------------------------------------------------------------------------------------------
Function: u8ConfirmValue

Description:To check if confirm button(button3) is pressed*/
static u8 u8ConfirmValue(void)
{
  u8 u8ConfirmValue=5;
  if(IsButtonPressed(BUTTON3))
  {
    u8ConfirmValue=3;
  }
  return u8ConfirmValue;
}/*end u8ConfirmValue()*/


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_ChangeMode()
Description:To choose whether to reset code*/
void UserApp1_ChooseMode(void)
{
  /*check the mode*/
  if(IsButtonHeld(BUTTON3,2000))
  {
    LedBlink(GREEN,500);
    LedBlink(RED,500);
    /*start to reset the code*/
    UserApp1_StateMachine=UserApp1_SetCode;
  }
}/*end UserApp1_ChooseMode()*/


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_SetCode()
Description:To set a new code*/
void UserApp1_SetCode(void)
{
  static u8 u8CodeValue=0;
  static u8 u8Confirm=0;
  static u8 u8ArryNum=0;
  static bool bWhiteOn=TRUE;
  static u32 u32TimeCounter=0;
  
    /*set the new code*/
    u8CodeValue=u8ButtonValue();
    u8Confirm=u8ConfirmValue();
    
    if(u8CodeValue!=4)
    {
      UserApp1_u8FinalCode[u8ArryNum]=u8CodeValue;
      u8ArryNum++;
      bWhiteOn=TRUE;
    }
    /*when enter a code,white led will blink*/
    if(bWhiteOn)
    {
      LedOn(WHITE);
      u32TimeCounter++;
      if(u32TimeCounter>500)
      {
         LedOff(WHITE);
         u32TimeCounter=0;
         bWhiteOn=FALSE;
      }
    }
    /*all the code have been scanfed*/
    if(u8Confirm!=5||u8ArryNum>8)
    {
      LedOff(RED);
      LedOff(GREEN);
      LedOff(WHITE); 
      u8ArryNum=0;
      UserApp1_StateMachine=UserApp1SM_Idle;
    }
    
  
}/*end UserApp1_SetCode()*/


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
    /*check if need to set new code*/
   
   
    LedOff(BLUE);
    LedOff(WHITE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(ORANGE);
    LedOff(RED);
    LedOff(YELLOW);
    LedOff(PURPLE);
    
 
    UserApp1_StateMachine=UserApp1_ChooseMode;
    
}   
   /* If good initialization, set state to Idle 
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
     The task isn't properly initialized, so shut it down and don't run 
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }   
      
      
} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();
} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions    
                                                                                              */

/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */

static void UserApp1SM_Idle(void)
{
    static u8 u8CodeValue=0;
    static u8 u8Confirm=0;
    static u32 u32EnterCode[]=0;
    static u8  u8ArryNum=0;
    static bool bWhiteOn=TRUE;
    static u8 u8index=0;
    static u8 u8CodeCheck=0;
    static u32 u32TimeCounter=0;
    
    /*locked*/
    LedOn(RED);
    
    u8CodeValue=u8ButtonValue();
    u8Confirm=u8ConfirmValue();
    /*enter the code */
    if(u8CodeValue!=4)
    {
      u32EnterCode[u8ArryNum]=u8CodeValue;
      u8ArryNum++;
      bWhiteOn=TRUE;
    }
    /*when enter a code,white led will blink*/
    if(bWhiteOn)
    {
      LedOn(WHITE);
      u32TimeCounter++;
      if(u32TimeCounter>500)
      {
         LedOff(WHITE);
         u32TimeCounter=0;
         bWhiteOn=FALSE;
      }
    }
  /*check the code*/
  if(u8Confirm!=5||u8ArryNum>8)
  {
    for(u8index=0;u8index<=u8ArryNum;u8ArryNum++)
    {
     if(u32EnterCode[u8index]!=UserApp1_u8FinalCode[u8index])
     {
       /*the code is wrong*/
       u8CodeCheck=1;
        break;
     }
    }
    if(u8CodeCheck!=1)
    {
      /*the code is right*/
      u8CodeCheck=2;
    }
  }
  
  /*set which led blink*/
  if(u8CodeCheck==1)
  {
    LedOff(WHITE);
    LedOn(RED);
  }
  if(u8CodeCheck==2)
  {
    LedOff(WHITE);
    LedOn(GREEN);
    LedOff(RED); 
  }
        
} /* end UserApp1SM_Idle() */
    
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
