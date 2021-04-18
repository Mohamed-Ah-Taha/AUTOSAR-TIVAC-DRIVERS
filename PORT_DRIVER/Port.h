 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohamed Taha
 *
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Mohamed Taha's ID = 1000 */
#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for PORT Init */
#define PORT_INIT_SID                   (uint8)0x00

/* Service ID for PORT Set Pin Direction */
#define PORT_SET_PIN_DIR_SID            (uint8)0x01

/* Service ID for PORT Refresh Port Direction */
#define PORT_REF_PORT_DIR_SID           (uint8)0x02
   
/* Service ID for PORT GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID       (uint8)0x03
   
/* Service ID for PORT Set Pin Mode */
#define PORT_SET_PIN_MODE_SID           (uint8)0x04

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port/Pin ID */
#define PORT_E_PARAM_PIN                (uint8)0x0A

/* DET code to report Port/Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE   (uint8)0x0B

/* Port_Init API service called with wrong parameter */
#define PORT_E_PARAM_CONFIG             (uint8)0x0C

/* DET code to report Port/Pin Mode passed not valid */
#define PORT_E_PARAM_INVALID_MODE       (uint8)0x0D
   
/* Port_SetPinMode API service called when mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE        (uint8)0x0E

/* API service called without module initialization */
#define PORT_E_UNINIT                   (uint8)0x0F

/* Api called with a NULL Pointer Parameter */
#define PORT_E_PARAM_POINTER            (uint8)0x10

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Description: Type definition for Port_PinType used by the PORT APIs , Data type for the symbolic name of a port pin */
typedef uint8 Port_PinType;

/* Description: Enum to hold PIN direction , Possible directions of a port pin */
typedef enum
{
    PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Type definition for Port_PinModeType used by the PORT APIs , Different port pin modes */
typedef uint8 Port_PinModeType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_PinInternalResistor;

/* Description: Enum to hold Initial Value for PIN */
typedef enum
{
    PORT_PIN_LEVEL_LOW,PORT_PIN_LEVEL_HIGH
}Port_PinInitialValue;

/* Description: Enum to hold if PORT/PIN Mode can be changed */
typedef enum
{
    PORT_PIN_MODE_NOT_CHANGEABLE,PORT_PIN_MODE_CHANGEABLE
}Port_PinModeChangeableType;

/* Description: Enum to hold if PORT/PIN Direction can be changed */
typedef enum
{
    PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_DIRECTION_CHANGEABLE
}Port_PinDirectionChangeableType;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the mode of the pin (e.g. DIO, ADC, SPI …) --> 0 -> 14
 *      4. the direction of pin --> INPUT or OUTPUT 
 *      5. the internal resistor --> Disable, Pull up or Pull down
 *      6. the initial value of the pin --> 0 , 1
 *      7. if the mode of the pin can be changed in the runtime or no --> 0 , 1
 *      8. if the direction of the pin can be changed in the runtime or no --> 0 , 1
 */
typedef struct 
{
        Port_PinType                    Port_Num;
        Port_PinType                    Pin_Num;
        Port_PinModeType                Pin_Mode;
        Port_PinDirectionType           Pin_Direction;
        Port_PinInternalResistor        Pin_Int_Resistor;
        Port_PinInitialValue            Pin_Initial_Value;
        Port_PinModeChangeableType      Pin_Mode_Changeable;
        Port_PinDirectionChangeableType Pin_Dir_Changeable;
}Port_ConfigChannel;


/* Data Structure required for initializing the Port Driver */
typedef struct Port_ConfigType
{
	Port_ConfigChannel Pins[PORT_CONFIGURED_PINS];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for PORT Initialization API */
void Port_Init(const Port_ConfigType* ConfigPtr);

/* Function for PORT Set Pin Direction API */
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif

/* Function for PORT Refresh Direction API */
void Port_RefreshPortDirection(void);

/* Function for PORT Get Version Info API */
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/* Function for PORT Set Pin Mode API */
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
