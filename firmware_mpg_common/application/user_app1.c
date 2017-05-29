/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 u8Number = 2;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
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
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1FirstChooseModeState;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1ChooseModeState()

Description:
To help user choose the following mode.
*/
static void UserApp1FirstChooseModeState(void) 
{
  static bool bMessageon = TRUE;
  /*contain some help information*/
  u8 au8UserApp1Start1[] = "********************************************************\n\rLed Programing Interface\n\rPress 1 to program Led command sequence\n\rPress 2 to show current USER program\n\r********************************************************\n\r";
  static u8 au8Choose[128];
  
  if(bMessageon)
  {
    DebugPrintf(au8UserApp1Start1);
    bMessageon=FALSE;
  }
  
  if(G_u8DebugScanfCharCount == 1)
  {
    DebugScanf(au8Choose);
    if(au8Choose[0] == '1')
    {
      //G_u8DebugScanfCharCount=0;
      //G_au8DebugScanfBuffer[0] = '\0';     
      DebugLineFeed();
      /*clear the current data*/
      LedDisplayStartList();
      UserApp1_StateMachine=UserApp1Mode1;
    }
    else if(au8Choose[0] == '2')
    {
      //G_u8DebugScanfCharCount=0;
      //G_au8DebugScanfBuffer[0] = '\0'; 
      DebugLineFeed();
      UserApp1_StateMachine=UserApp1Mode2;
    }
    else
    {
      //G_u8DebugScanfCharCount=0;
      //G_au8DebugScanfBuffer[0] = '\0';
      DebugLineFeed();
      DebugPrintf("Invalid command:please press 1 or 2");
      DebugLineFeed();
    }
  }
  
}/*end UserApp1ChooseModeState()*/
  

/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1Mode1()

Description:
To help user program Led command sequence.
*/
static void UserApp1Mode1(void)
{
    static bool bLedCommand=FALSE;
    static bool bOnInput=TRUE;
    static bool bOffInput=FALSE;
    static u8 index=2;
    static u8 indexCheck=2;
    
    
    static bool bMessageon = TRUE;
    static u32 u32OnTime=0;
    static u32 u32OffTime=0;
    LedCommandType eUserCommand;
    /*contain some help information*/
    u8 au8UserApp1Start1[] = "Enter commands as LED-ONTIME-OFFTIME and press Enter\n\rTime is in milliseconds,max 100 commands\n\rLED colors:R,O,Y,G,C,B,P,W\n\rExample:R-100-200(Red on at 100ms and off at 200ms)\n\rPress Enter on blank line to end.\n\r1: ";
    
    /*print the help information*/
    if(bMessageon)
    {
    DebugLineFeed();
    DebugPrintf(au8UserApp1Start1);
    bMessageon=FALSE;
    }
      /*set the led*/
if(G_u8DebugScanfCharCount == 1)
{
      if(G_au8DebugScanfBuffer[0]=='R'||G_au8DebugScanfBuffer[0]=='r')
      {
        eUserCommand.eLED = RED; 
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='O'||G_au8DebugScanfBuffer[0]=='o')
      {
        eUserCommand.eLED = ORANGE;
        bLedCommand=TRUE;        
      }
      if(G_au8DebugScanfBuffer[0]=='Y'||G_au8DebugScanfBuffer[0]=='y')
      {
        eUserCommand.eLED = YELLOW; 
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='G'||G_au8DebugScanfBuffer[0]=='g')
      {
        eUserCommand.eLED = GREEN;   
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='B'||G_au8DebugScanfBuffer[0]=='b')
      {
        eUserCommand.eLED = BLUE;  
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='P'||G_au8DebugScanfBuffer[0]=='p')
      {
        eUserCommand.eLED = PURPLE;   
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='W'||G_au8DebugScanfBuffer[0]=='w')
      {
        eUserCommand.eLED = WHITE;    
        bLedCommand=TRUE;
      }
      if(G_au8DebugScanfBuffer[0]=='\r')
      {
        bLedCommand=TRUE;
      }
      /*to give some debug information*/

      if(!bLedCommand)
      {
        DebugLineFeed();
        DebugPrintf("The led must be one of the eight kinds\n\r");
        DebugLineFeed();
        /*the initial value of u8Number is 2,it will only add when a whole command is input*/
        DebugPrintNumber(u8Number-1);
        DebugPrintf(": "); 
        /*reset the Led input condition*/
        G_u8DebugScanfCharCount=0;
        G_au8DebugScanfBuffer[0]='\0';
      }


}

  /*set on time*/
 if(bOnInput)
 {
#if 0
   if(G_au8DebugScanfBuffer[index]=='\b')
   {
     u32OnTime=(u32OnTime-G_au8DebugScanfBuffer[index-1])/10;
     index++;
     break;
     
   }
#endif
  if(G_u8DebugScanfCharCount >= 3)
  { 
    if(G_au8DebugScanfBuffer[index]!='-')
    {   
      /*to add when you have scanf a char*/
      if(indexCheck==index)
      {
        /*if user type the wrong time,they can backspace*/
         if(G_au8DebugScanfBuffer[index]=='\b')
         {
           
           u32OnTime=(u32OnTime-(G_au8DebugScanfBuffer[index-1]-'0'))/10;
           index++;       
         }
         else
         {
          /*get the number you have scanfed*/
          u32OnTime = u32OnTime*10 + (G_au8DebugScanfBuffer[index]-'0');
          index++;
          /*check if the input is right,30-39 is the Ascll of 0-9*/
         }


               if((G_au8DebugScanfBuffer[index-1]<0x30||G_au8DebugScanfBuffer[index-1]>0x39)&&G_au8DebugScanfBuffer[index-1]!='\b')
               {
                           /*reset code*/
                           bLedCommand=FALSE;
                           u32OnTime=0;
                           u32OffTime=0;
                           index = 2;
                           indexCheck = 2;
                           for(u8 i = 0; i < G_u8DebugScanfCharCount; i++)
                               {
                                 G_au8DebugScanfBuffer[i] = '\0';
                               }
                           G_u8DebugScanfCharCount = 0;
                           bOnInput = TRUE;
                           bOffInput = FALSE;
                           /*print the current serial number and helping information*/       
                           DebugLineFeed();
                           DebugPrintf("invalid command please check carefully and command again");
                           DebugLineFeed();
                           DebugPrintNumber(u8Number-1);
                           DebugPrintf(": "); 
                           u8Number++;
               }

         }
      indexCheck=G_u8DebugScanfCharCount-1;
    }
    else
    {
         /*add the command to the list*/
         eUserCommand.bOn = TRUE;
         eUserCommand.u32Time = u32OnTime;
         eUserCommand.eCurrentRate = LED_PWM_0;
         LedDisplayAddCommand(USER_LIST, &eUserCommand);
         bOnInput = FALSE;
         bOffInput = TRUE;
         indexCheck=index;
        
    }
  }
 }
  /*set on time*/
 if(bOffInput)
 {
   if(G_au8DebugScanfBuffer[index]!='\r')
   {
      if(G_au8DebugScanfBuffer[index]!='-')
      {
       if(indexCheck==index)
       {
                  if(G_au8DebugScanfBuffer[index]=='\b')
                   {
                     u32OnTime=(u32OnTime-(G_au8DebugScanfBuffer[index-1]-'0'))/10;
                     index++;       
                   }
                  else
                  {
                  /*get the number you have scanfed*/
                  u32OffTime = u32OffTime*10 + (G_au8DebugScanfBuffer[index]-'0');
                  index++;
                  }
              /*check if the input is right,30-39 is the Ascll of 0-9*/
               if(G_au8DebugScanfBuffer[index-1]<0x30||G_au8DebugScanfBuffer[index-1]>0x39&&G_au8DebugScanfBuffer[index-1]!='\b')
               {
                           /*reset code*/
                           bLedCommand=FALSE;
                           u32OnTime=0;
                           u32OffTime=0;
                           index = 2;
                           indexCheck = 2;
                           for(u8 i = 0; i < G_u8DebugScanfCharCount; i++)
                               {
                                 G_au8DebugScanfBuffer[i] = '\0';
                               }
                           G_u8DebugScanfCharCount = 0;
                           bOnInput = TRUE;
                           bOffInput = FALSE;
                           /*print the current serial number and helping information*/       
                           DebugLineFeed();
                           DebugPrintf("invalid command please check carefully and command again");
                           DebugLineFeed();
                           DebugPrintNumber(u8Number-1);
                           DebugPrintf(": "); 
                           u8Number++;
               }
       }
            indexCheck=G_u8DebugScanfCharCount-1;
      
    }/* if(G_au8DebugScanfBuffer[index]!='-')*/
      else
      {
        index++;
      }
   }/*if(G_au8DebugScanfBuffer[index]=='\r')*/
     /*press enter and check if the command is right*/
   else 
          {
               /*add the command to the list*/
               eUserCommand.bOn = FALSE;
               eUserCommand.u32Time = u32OffTime;
               eUserCommand.eCurrentRate = LED_PWM_100;
               LedDisplayAddCommand(USER_LIST, &eUserCommand);
               /*reset*/
               bLedCommand=FALSE;
               u32OnTime=0;
               u32OffTime=0;
               index = 2;
               indexCheck = 2;
               for(u8 i = 0; i < G_u8DebugScanfCharCount; i++)
                   {
                     G_au8DebugScanfBuffer[i] = '\0';
                   }
               G_u8DebugScanfCharCount = 0;
               bOnInput = TRUE;
               bOffInput = FALSE;
               /*print the next serial number*/       
               DebugLineFeed();
               DebugPrintNumber(u8Number);
               DebugPrintf(": "); 
               u8Number++;
          }
             
            
     

  }
  /*to show users the recent program*/
  if(G_au8DebugScanfBuffer[0]=='\r')
  {
    UserApp1_StateMachine=UserApp1Mode3;
    //DebugLineFeed();
    //DebugPrintf("Command entry complete.\n\rCommands entered: ");
   // DebugPrintNumber(u8Number-1);
  }
  else
  {
    G_au8DebugScanfBuffer[0]='\0';
  }
 
 

}/*end UserApp1Mode1()*/


/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1Mode2()

Description:
To help user view current USER program.
*/
static void UserApp1Mode2(void)
{
  static u8 au8Choose[128];
  static bool bMessageon = TRUE; 
  /*contain some help information*/
  u8 u8ListEntry = 0;
    
  /*show user the user list*/
    if(bMessageon)
  {
    DebugLineFeed();
    DebugPrintf("Current User program:\n\r");
    DebugLineFeed();
    DebugPrintf("LED  ON TIME   OFF TIME\n\r------------------------\n\r");
    while( LedDisplayPrintListLine(u8ListEntry++) );
    DebugPrintf("\n\r------------------------\n\r");
    DebugPrintf("Press 1 to program");
    
    bMessageon=FALSE;
  }
  /*back to menu*/
   if(G_u8DebugScanfCharCount == 1)
  {
    DebugScanf(au8Choose);
    if(au8Choose[0] == '1')
    {
      UserApp1_StateMachine=UserApp1Mode1;
    }
  }
  
}/*end UserApp1Mode2()*/


/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1Mode3()

Description:
To help user view USER program have input.
*/
static void UserApp1Mode3(void)
{
  static bool bMessageon = TRUE;
  u8 u8ListEntry = 0;
  static u8 au8Choose[128];
  
  /*show user the proram they input*/
  if(bMessageon)
  {
    DebugLineFeed();
    DebugPrintf("Command entry complete.\n\rCommands entered: ");
    DebugPrintNumber(u8Number-1);
    DebugLineFeed();
    DebugLineFeed();
    DebugPrintf("New USER program:\n\r");
    DebugLineFeed();
    DebugPrintf("LED  ON TIME   OFF TIME\n\r------------------------\n\r");
    while( LedDisplayPrintListLine(u8ListEntry++) );
    DebugPrintf("\n\r------------------------\n\r");
    DebugPrintf("Press 1 to view the user list");
    /*clear the buffer*/
    for(u8 i = 0; i < G_u8DebugScanfCharCount; i++)
        {
          G_au8DebugScanfBuffer[i] = '\0';
        }
    G_u8DebugScanfCharCount=0;
    bMessageon=FALSE;
  }
  /*back to menu*/
   if(G_u8DebugScanfCharCount == 1)
  {
    DebugScanf(au8Choose);
    if(au8Choose[0] == '1')
    {
      UserApp1_StateMachine=UserApp1Mode2;
    }
  }
}/*end UserApp1Mode3()*/


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
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  
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
