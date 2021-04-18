 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohamed Taha
 *
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigChannel * Port_Channels = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port Driver module.
************************************************************************************/
void Port_Init( const Port_ConfigType * ConfigPtr )
{
  	volatile uint32 * Port_Ptr = NULL_PTR;
	boolean error = FALSE; /* Used in case a DET error is reported */
        volatile uint32 delay = 0; /* Used for the clock register */
        uint8 INDEX_COUNTER = 0 ; /* Used for looping inside the PB configuration structure members */

#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,  PORT_E_PARAM_CONFIG);
          error = TRUE;
	}
	else    
#endif
        /* In-case there are no errors */
	if(FALSE == error)
	{
         /*
          * Set the module state to initialized and point to the PB configuration structure using a global pointer.
          * This global pointer is global to be used by other functions to read the PB configuration structures
          */
          Port_Channels = ConfigPtr->Pins; /* address of the first Port structure --> Port[0] */
         /* A loop is used to loop around all the structure members to get configured */
         for( INDEX_COUNTER = 0 ; INDEX_COUNTER < PORT_CONFIGURED_PINS ; INDEX_COUNTER++)
         { 
            /**************************************************************************************************************************************
            *               1- Assign Pointer Port_Ptr to be pointing at the Port Number in the PB configuration structure member                *
            **************************************************************************************************************************************/
            switch(Port_Channels[INDEX_COUNTER].Port_Num)
            {
                case  0  : Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                            break;
                            
                case  1  : Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                            break;
                            
                case  2  : Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                            break;
                            
                case  3  : Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                            break;
                            
                case  4  : Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                            break;
                            
                case  5  : Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                            break;
            }
            
            /* Enable clock for PORT and allow time for clock to start for all pins */
            SYSCTL_REGCGC2_REG |= (1<<(Port_Channels[INDEX_COUNTER].Port_Num));
            delay = SYSCTL_REGCGC2_REG;
                       
            /* For Pins PC3-0 and PD7 and PF0 , unlocking is needed */
            
            /* PD7 or PF0 */
            if((((Port_Channels[INDEX_COUNTER].Port_Num) == PortConf_PD7_PORT_NUM) && ((Port_Channels[INDEX_COUNTER].Pin_Num) == PortConf_PD7_PIN_NUM)) || (((Port_Channels[INDEX_COUNTER].Port_Num) == PortConf_PF0_PORT_NUM) && ((Port_Channels[INDEX_COUNTER].Pin_Num) == PortConf_PF0_PIN_NUM))) 
            {	
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = GPIO_LOCK_NUM;                                      /* Unlock the GPIOCR register */   
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            /* PC0 to PC3 */ /* If those pins where configured */ /* In my configuration those pins aren't configured */
            else if( ((Port_Channels[INDEX_COUNTER].Port_Num) == PortConf_PC0_PORT_NUM) && ((Port_Channels[INDEX_COUNTER].Pin_Num) <= PortConf_PC3_PIN_NUM) ) 
            {
              /* Do Nothing ...  this is the JTAG pins */
              continue;
            }
            else
            {
                /* Do Nothing ... No need to unlock the commit register for this pin */
            }
                       
           /**************************************************************************************************************************************
            *                                       2- Check on the Pin mode ( DIO or AF or AIO )                                                *
            **************************************************************************************************************************************/
           /*************************************************************
            *                           MODE 0                          *
            *************************************************************/   
            if( (Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE0_DIO ) 
            {	
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~( MODE0_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4)); /* Clear the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
              
           /*************************************************************
            *                           MODE 1                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE1)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE1_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 2                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE2)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE2_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 3                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE3)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE3_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 4                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE4)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE4_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 5                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE5)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE5_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 6                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE6)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE6_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 7                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE7)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE7_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 8                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE8)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE8_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 9                          *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE9)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE9_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
           /*************************************************************
            *                           MODE 14                         *
            *************************************************************/                            
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE14)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE14_PMC_MASK << ((Port_Channels[INDEX_COUNTER].Pin_Num)* 4));  /* Set the PMCx bits for this pin */
              /* Check for direction */
              if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                
                if((Port_Channels[INDEX_COUNTER].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }                                                                                                                                        
              }                                                                                                                                            
              else if((Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
              {                                                                                                                                            
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                  
                /* Check for the internal resistor */                                                                                                    
                if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_UP)                                                                           
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }                                                                                                                                        
                else if((Port_Channels[INDEX_COUNTER].Pin_Int_Resistor) == PULL_DOWN)                                                                    
                {                                                                                                                                        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }                                                                                                                                        
                else /* No internal Resistor */                                                                                                          
                {                                                                                                                                        
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
              }
              else
              {
                  /* Do Nothing */
              }
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }
            
           /*******************************************************************
            *                           MODE Ain(15)                          *
            *******************************************************************/
            else if((Port_Channels[INDEX_COUNTER].Pin_Mode) == MODE15_ANALOG)
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));       /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num));        /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */              
            }   							
            
            else 
            {
              /* No Action Required */                            
            }    
                         
         } /* End of For Loop */
         
        Port_Status   = PORT_INITIALIZED;
	}
	else
	{
          /* No Action Required */
	}
}

/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin       - Port Pin ID number.
*                  Direction - Port Pin Direction.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction.
*              the user should enter pin number from 0 -> 43(decimal) 
*              PORT A : 0  -> 7  (PortConf_PA0_PIN_ID_INDEX -> PortConf_PA7_PIN_ID_INDEX)
*              PORT B : 8  -> 15 (PortConf_PB0_PIN_ID_INDEX -> PortConf_PB7_PIN_ID_INDEX)
*              PORT C : 16 -> 19 (PortConf_PC4_PIN_ID_INDEX -> PortConf_PC7_PIN_ID_INDEX)
*              PORT D : 20 -> 27 (PortConf_PD0_PIN_ID_INDEX -> PortConf_PD7_PIN_ID_INDEX)
*              PORT E : 28 -> 33 (PortConf_PE0_PIN_ID_INDEX -> PortConf_PE5_PIN_ID_INDEX)
*              PORT F : 34 -> 38 (PortConf_PF0_PIN_ID_INDEX -> PortConf_PF4_PIN_ID_INDEX)
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction )
{
  	volatile uint32 * Port_Ptr = NULL_PTR;
	boolean error = FALSE; /* Used in case a DET error is reported */
        uint8 INDEX = 0 ; /* Used for Pointing on PB configuration structure members */
        
        /* Variables used for Adjesting the PIN numbers */
        uint8 PORTA_ADJ = 0 ; 
        uint8 PORTB_ADJ = 8 ; 
        uint8 PORTC_ADJ = 12 ; 
        uint8 PORTD_ADJ = 20 ; 
        uint8 PORTE_ADJ = 28 ; 
        uint8 PORTF_ADJ = 34 ;
        
        /* Choosing the right PIN */
        if(Pin >= PortConf_PA0_PIN_ID_INDEX && Pin <= PortConf_PA7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTA_ADJ ;
        }
        else if(Pin >= PortConf_PB0_PIN_ID_INDEX && Pin <= PortConf_PB7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTB_ADJ ;
        }
        else if(Pin >= PortConf_PC4_PIN_ID_INDEX && Pin <= PortConf_PC7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTC_ADJ ;
        }
        else if(Pin >= PortConf_PD0_PIN_ID_INDEX && Pin <= PortConf_PD7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTD_ADJ ;
        }
        else if(Pin >= PortConf_PE0_PIN_ID_INDEX && Pin <= PortConf_PE5_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTE_ADJ ;
        }
        else if(Pin >= PortConf_PF0_PIN_ID_INDEX && Pin <= PortConf_PF4_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTF_ADJ ;
        }
        else
        {
          /* No Action Required */
        }
        
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_UNINIT);
          error = TRUE;
	}
	else
	{
	 /* No Action Required */
	}
        /* Check if that Pin Direction is changeable or no */
        if ((Port_Channels[INDEX].Pin_Dir_Changeable) == PORT_PIN_DIRECTION_NOT_CHANGEABLE)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_DIRECTION_UNCHANGEABLE);
          error = TRUE;
	}
	else
	{
          /* No Action Required */
	}
	/* Check if the used Pin is within the valid range */
	if (PORT_CONFIGURED_PINS <= Pin)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_PARAM_PIN);
          error = TRUE;
	}
	else
	{
          /* No Action Required */
	}
#endif

	/* In-case there are no errors */
	if(FALSE == error)
	{
          if( Direction == PORT_PIN_OUT ) /* If the Pin is Output Pin */
          {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin );                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */                                                                                                                                        
          }
          else if( Direction == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
          {                                                                                                                                            
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin );              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
          }
          else
          {
               /* No Action Required */
          }
	}
	else
	{
		/* No Action Required */
	}
}
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection( void )
{
  	volatile uint32 * Port_Ptr = NULL_PTR;
	boolean error = FALSE; /* Used in case a DET error is reported */
        uint8 INDEX_COUNTER = 0 ; /* Used for looping inside the PB configuration structure members */

#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REF_PORT_DIR_SID, PORT_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif
        
	/* In-case there are no errors */
	if(FALSE == error)
	{
         for( INDEX_COUNTER = 0 ; INDEX_COUNTER < PORT_CONFIGURED_PINS ; INDEX_COUNTER++)
         { 
            switch(Port_Channels[INDEX_COUNTER].Port_Num)
            {
                case  0  : Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                            break;
                            
                case  1  : Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                            break;
                            
                case  2  : Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                            break;
                            
                case  3  : Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                            break;
                            
                case  4  : Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                            break;
                            
                case  5  : Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                            break;
            }
            /* Refreshing Each Pin Direction */
            if( (Port_Channels[INDEX_COUNTER].Pin_Dir_Changeable) == PORT_PIN_DIRECTION_CHANGEABLE ) /* If the Pin is Direction Changeable */
            {
              /* Do nothing */
              /* Any pin with Direction Changeable should be excluded */
              continue;
            }
    
            else if( (Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_OUT ) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num) );                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */                                                                                                                                        
            }
          
            else if( (Port_Channels[INDEX_COUNTER].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , (Port_Channels[INDEX_COUNTER].Pin_Num) );              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            }
         }  
	}
	else
	{
		/* No Action Required */
	}
}

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): VersionInfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Function to get the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
  	boolean error = FALSE; /* Used in case a DET error is reported */
        
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == versioninfo)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
          error = TRUE;
	}
      	else
	{
		/* No Action Required */
	}
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
          error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
#endif
        
	/* In-case there are no errors */
	if(FALSE == error)
	{
          /* Copy the vendor Id */
          versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
          /* Copy the module Id */
          versioninfo->moduleID = (uint16)PORT_MODULE_ID;
          /* Copy Software Major Version */
          versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
          /* Copy Software Minor Version */
          versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
          /* Copy Software Patch Version */
          versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
	else
	{
		/* No Action Required */
	}
}
#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin       - Port Pin ID number.
*                  Mode      - New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
*              the user should enter pin number from 0 -> 43 (decimal)
*              PORT A : 0  -> 7  (PortConf_PA0_PIN_ID_INDEX -> PortConf_PA7_PIN_ID_INDEX)
*              PORT B : 8  -> 15 (PortConf_PB0_PIN_ID_INDEX -> PortConf_PB7_PIN_ID_INDEX)
*              PORT C : 16 -> 19 (PortConf_PC4_PIN_ID_INDEX -> PortConf_PC7_PIN_ID_INDEX)
*              PORT D : 20 -> 27 (PortConf_PD0_PIN_ID_INDEX -> PortConf_PD7_PIN_ID_INDEX)
*              PORT E : 28 -> 33 (PortConf_PE0_PIN_ID_INDEX -> PortConf_PE5_PIN_ID_INDEX)
*              PORT F : 34 -> 38 (PortConf_PF0_PIN_ID_INDEX -> PortConf_PF4_PIN_ID_INDEX)
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
  	volatile uint32 * Port_Ptr = NULL_PTR;
	boolean error = FALSE; /* Used in case a DET error is reported */
        uint8 INDEX = 0 ; /* Used for Pointing on PB configuration structure members */
        
        /* Variables used for Adjesting the PIN numbers */
        uint8 PORTA_ADJ = 0 ; 
        uint8 PORTB_ADJ = 8 ; 
        uint8 PORTC_ADJ = 12 ; 
        uint8 PORTD_ADJ = 20 ; 
        uint8 PORTE_ADJ = 28 ; 
        uint8 PORTF_ADJ = 34 ;
        
        /* Choosing the right PIN */
        if(Pin >= PortConf_PA0_PIN_ID_INDEX && Pin <= PortConf_PA7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTA_ADJ ;
        }
        else if(Pin >= PortConf_PB0_PIN_ID_INDEX && Pin <= PortConf_PB7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTB_ADJ ;
        }
        else if(Pin >= PortConf_PC4_PIN_ID_INDEX && Pin <= PortConf_PC7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTC_ADJ ;
        }
        else if(Pin >= PortConf_PD0_PIN_ID_INDEX && Pin <= PortConf_PD7_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTD_ADJ ;
        }
        else if(Pin >= PortConf_PE0_PIN_ID_INDEX && Pin <= PortConf_PE5_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTE_ADJ ;
        }
        else if(Pin >= PortConf_PF0_PIN_ID_INDEX && Pin <= PortConf_PF4_PIN_ID_INDEX)
        {
          Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
          INDEX = Pin ;
          Pin = Pin - PORTF_ADJ ;            
        }
        else
        {
          /* No Action Required */
        }

#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
          error = TRUE;
	}
	else
	{
	 /* No Action Required */
	}
	/* Check if that Pin Mode is changeable or no */
        if ((Port_Channels[INDEX].Pin_Mode_Changeable) == PORT_PIN_MODE_NOT_CHANGEABLE)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
          error = TRUE;
        }
	else
	{
          /* No Action Required */
	}
        if (PORT_CONFIGURED_PINS <= Pin)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
          error = TRUE;
	}
	else
	{
          /* No Action Required */
	}
	/* Check if the used Mode is within the valid range */
	if (PORT_CONFIGURED_MODES <= Mode)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
          error = TRUE;
	}
	else
	{
          /* No Action Required */
	}
#endif
	/* In-case there are no errors */
	if(FALSE == error)
	{
         /*************************************************************
          *                           MODE 0                          *
          *************************************************************/   
          if( Mode == MODE0_DIO ) 
          {	
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~( MODE0_PMC_MASK << (Pin* 4)); /* Clear the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
                     
         /* MODE 1 -> 9 --> AF and MODE Ain(15) --> Analog */
         /*************************************************************
          *                           MODE 1                          *
          *************************************************************/                            
          else if( Mode == MODE1)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE1_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 2                          *
          *************************************************************/                            
          else if( Mode == MODE2)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE2_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 3                          *
          *************************************************************/                            
          else if( Mode == MODE3)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE3_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 4                          *
          *************************************************************/                            
          else if( Mode == MODE4)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE4_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 5                          *
          *************************************************************/                            
          else if( Mode == MODE5)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE5_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 6                          *
          *************************************************************/                            
          else if( Mode == MODE6)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE6_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 7                          *
          *************************************************************/                            
          else if( Mode == MODE7)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE7_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 8                          *
          *************************************************************/                            
          else if( Mode == MODE8)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE8_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 9                          *
          *************************************************************/                            
          else if( Mode == MODE9)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE9_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
         /*************************************************************
          *                           MODE 14                         *
          *************************************************************/                            
          else if( Mode == MODE14)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= ( MODE14_PMC_MASK << (Pin* 4));  /* Set the PMCx bits for this pin */
            /* Check for direction */
            if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_OUT) /* If the Pin is Output Pin */
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
              
              if((Port_Channels[INDEX].Pin_Initial_Value) == PORT_PIN_LEVEL_HIGH) /* Check for the initial value for the pin */
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);           /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
              }                                                                                                                                        
              else                                                                                                                                     
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET) , Pin);         /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
              }                                                                                                                                        
            }                                                                                                                                            
            else if((Port_Channels[INDEX].Pin_Direction) == PORT_PIN_IN) /* If the Pin is Input Pin */                                           
            {                                                                                                                                            
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET) , Pin);              /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                                                                                                                                                                
              /* Check for the internal resistor */                                                                                                    
              if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_UP)                                                                           
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }                                                                                                                                        
              else if((Port_Channels[INDEX].Pin_Int_Resistor) == PULL_DOWN)                                                                    
              {                                                                                                                                        
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);      /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
              }                                                                                                                                        
              else /* No internal Resistor */                                                                                                          
              {                                                                                                                                        
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Pin);    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
              }
            }
            else
            {
                /* Do Nothing */
            }
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
          }
          
         /*******************************************************************
          *                           MODE Ain(15)                          *
          *******************************************************************/
          else if( Mode == MODE15_ANALOG)
          {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);       /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */              
          }   							
          
          else 
          {
            /* No Action Required */                            
          }    
	}
	else
	{
		/* No Action Required */
	}   
}
#endif