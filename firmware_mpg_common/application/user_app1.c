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
/* Existing variables (defined in other files)      */
extern u8 G_au8DebugScanfBuffer[];  /* From debug.c */
extern u8 G_u8DebugScanfCharCount;  /* From debug.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static bool bState2Mark = FALSE;
static bool bState1Mark = FALSE;

/*mean the state is state2*/
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/



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
   /*make sure the leds in right statement*/
    LedOff(BLUE);
    LedOff(WHITE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(ORANGE);
    LedOff(RED);
    LedOff(YELLOW);
    LedOff(PURPLE);
    /*clear the lcd*/
    LCDCommand(LCD_CLEAR_CMD); 
    

    /*If good initialization, set state to Idle*/ 
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
     /*The task isn't properly initialized, so shut it down and don't run */
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

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1_State1

Description:
the first statement 
*/
static void UserApp1_State1(void)
{
  u8 au8Message[] = "STATE 1";
  static bool bMessageon = TRUE;
  static u16 u16Counter = 0;
 
  
  static bool bState1Mark = TRUE;
    LedOff(RED);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
  /*prepare to choose*/
  if(u16Counter++ >= 10);
  {
     u16Counter = 0;
     UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /*to show the information*/
  LCDMessage(LINE1_START_ADDR, au8Message);
  
  /*lit the leds and lcd*/
  LedOn(WHITE);
  LedOn(BLUE);
  LedOn(PURPLE);
  LedOn(CYAN);
  /*purple*/
  LedOn(LCD_RED);
  LedOff(LCD_GREEN);
  LedOn(LCD_BLUE);
 /*operation in tera term*/ 
  if(bMessageon)
  {
    DebugPrintf("Entering state 1\n\r ");
    bMessageon=FALSE;
  }

}/*end UserApp1State1()*/


/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1State2

Description:
the sec statement
*/
static void UserApp1_State2(void)
{
  u8 au8Message[] = "STATE 2";
  static bool bMessageon = TRUE;
  static u16 u16Counter = 0;
  
  
  bState2Mark = TRUE;
  /*prepare to choose*/
  if(u16Counter++ >= 10);
  {
     u16Counter = 0;
     UserApp1_StateMachine = UserApp1SM_Idle;
  }
  /*Lcd and leds working*/ 
  if(bMessageon)
  {
    DebugPrintf("Entering state 2\n\r ");
    bMessageon=FALSE;
    LedBlink(RED,LED_8HZ);
    LedBlink(ORANGE,LED_4HZ);
    LedBlink(YELLOW,LED_2HZ);
    LedBlink(GREEN,LED_1HZ);  
    LedOff(WHITE);
    LedOff(BLUE);
    LedOff(PURPLE);
    LedOff(CYAN); 
  }
   
    LCDMessage(LINE1_START_ADDR, au8Message);     
 
    LedPWM(LCD_RED,LED_PWM_100);
    LedPWM(LCD_GREEN,LED_PWM_40);
    LedPWM(LCD_BLUE,LED_PWM_10);

}/*end UserApp1State2()*/
                                                                                              
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */

static void UserApp1SM_Idle(void)
{     
  static u16 u16BuzzerCounter = 0;
  static bool bBuzzer = TRUE;
  
 
  /*state2 is running*/
  if(bState2Mark)
  {
    /*control the buzzer*/
    if(bBuzzer)
    {
      if(u16BuzzerCounter>100)
      {
        PWMAudioOff(BUZZER1);
      }
      if(u16BuzzerCounter>1000)
      {
        u16BuzzerCounter = 0;
        PWMAudioOn(BUZZER1);
      }
    }/* if(bBuzzer)*/
  }
  if(WasButtonPressed(BUTTON1))
  {
    ButtonAcknowledge(BUTTON1);
    LCDCommand(LCD_CLEAR_CMD);
    bBuzzer = FALSE;
    UserApp1_StateMachine = UserApp1_State1;
  }
    if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    LCDCommand(LCD_CLEAR_CMD);
    UserApp1_StateMachine = UserApp1_State2;
  }
  /*choose state in tera term*/ 
  if(G_u8DebugScanfCharCount == 2)
  {
    if(G_au8DebugScanfBuffer[0]=='1'&&G_au8DebugScanfBuffer[1]=='\r')
    {  
      G_au8DebugScanfBuffer[0]='\0';
      G_au8DebugScanfBuffer[1]='\0';
      G_u8DebugScanfCharCount=0; 
      LCDCommand(LCD_CLEAR_CMD);
      bBuzzer = FALSE;
      UserApp1_StateMachine = UserApp1_State1;
      
    }
    else if(G_au8DebugScanfBuffer[0]=='2'&&G_au8DebugScanfBuffer[1]=='\r')
    {
       LCDCommand(LCD_CLEAR_CMD);
       G_au8DebugScanfBuffer[0]='\0';
       G_au8DebugScanfBuffer[1]='\0';
       G_u8DebugScanfCharCount=0;
       UserApp1_StateMachine = UserApp1_State2;
    }
    else
    {
       G_au8DebugScanfBuffer[0]='\0';
       G_au8DebugScanfBuffer[1]='\0';
       G_u8DebugScanfCharCount=0;
       DebugPrintf("Invalid command");
    }
  }/*end if(G_u8DebugScanfCharCount == 2)*/
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
