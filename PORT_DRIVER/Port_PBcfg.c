 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohamed Taha
 *
 ******************************************************************************/

#include "Port.h"

/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/*  */

/* PB structure used with Port_Init API */
/* All Pins by default will be :
 *                              1- Direction : Output Pins
 *                              2- Internal Resistor : OFF
 *                              3- Initial Value : HIGH
 *                              4- Mode : MODE0 --> GPIO Pins
 *                              5- Pin Mode Changeable : Not Changeable
 *                              6- Pin Direction Changeable : Not Changeable
 */  
/* In this Project only 2 pins are used :
 *                                       1- PF1 : LED1
 *                                       2- PF4 : SW1
 * 
 * For checking on the modes :
 *                            1- PB4 : MODE8
 *                            2- PD2 : MODE9
 *                            3- PE0 : MODE15_ANALOG
 * For checking on changing pin mode during runtime : PA0 , PB7 -> PORT_PIN_MODE_CHANGEABLE
 * For checking on changing pin direction during runtime : PE3 -> PORT_PIN_DIRECTION_CHANGEABLE
 */  
   
const Port_ConfigType Port_Configuration = {
                                             /*******************************************************************************
                                             *                                  PORT A                                      *
                                             *******************************************************************************/
                                             PortConf_PA0_PORT_NUM   ,   PortConf_PA0_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_CHANGEABLE       ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA0 */ /* Index 00 Pin 00 */ /* Pin mode can be changed during runtime */
                                             PortConf_PA1_PORT_NUM   ,   PortConf_PA1_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA1 */ /* Index 01 Pin 01 */
                                             PortConf_PA2_PORT_NUM   ,   PortConf_PA2_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA2 */ /* Index 02 Pin 02 */
                                             PortConf_PA3_PORT_NUM   ,   PortConf_PA3_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA3 */ /* Index 03 Pin 03 */
                                             PortConf_PA4_PORT_NUM   ,   PortConf_PA4_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA4 */ /* Index 04 Pin 04 */
                                             PortConf_PA5_PORT_NUM   ,   PortConf_PA5_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_CHANGEABLE      , /* PA5 */ /* Index 05 Pin 05 */ 
                                             PortConf_PA6_PORT_NUM   ,   PortConf_PA6_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA6 */ /* Index 06 Pin 06 */
                                             PortConf_PA7_PORT_NUM   ,   PortConf_PA7_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PA7 */ /* Index 07 Pin 07 */
                                                                                                                                                                                                                                                           
                                             /*******************************************************************************                                                                                                                              
                                             *                                  PORT B                                      *                                                                                                                              
                                             *******************************************************************************/                                                                                                                              
                                             PortConf_PB0_PORT_NUM   ,   PortConf_PB0_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB0 */ /* Index 08 Pin 08 */
                                             PortConf_PB1_PORT_NUM   ,   PortConf_PB1_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB1 */ /* Index 09 Pin 09 */
                                             PortConf_PB2_PORT_NUM   ,   PortConf_PB2_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB2 */ /* Index 10 Pin 10 */
                                             PortConf_PB3_PORT_NUM   ,   PortConf_PB3_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB3 */ /* Index 11 Pin 11 */
                                             PortConf_PB4_PORT_NUM   ,   PortConf_PB4_PIN_NUM   ,  MODE8       ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB4 */ /* Index 12 Pin 12 */ /* MODE CHANGED TO MODE 8 Rx */
                                             PortConf_PB5_PORT_NUM   ,   PortConf_PB5_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB5 */ /* Index 13 Pin 13 */
                                             PortConf_PB6_PORT_NUM   ,   PortConf_PB6_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB6 */ /* Index 14 Pin 14 */
                                             PortConf_PB7_PORT_NUM   ,   PortConf_PB7_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_CHANGEABLE       ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PB7 */ /* Index 15 Pin 15 */ /* Pin mode can be changed during runtime */
                                                                                                                                                                                                                                                            
                                             /*******************************************************************************                                                                                                                               
                                             *                                  PORT C                                      *                                                                                                                               
                                             *******************************************************************************/                                                                                                                               
                                             PortConf_PC4_PORT_NUM   ,   PortConf_PC4_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PC4 */ /* Index 16 Pin 16 */
                                             PortConf_PC5_PORT_NUM   ,   PortConf_PC5_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PC5 */ /* Index 17 Pin 17 */
                                             PortConf_PC6_PORT_NUM   ,   PortConf_PC6_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PC6 */ /* Index 18 Pin 18 */
                                             PortConf_PC7_PORT_NUM   ,   PortConf_PC7_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PC7 */ /* Index 19 Pin 19 */
                                                                                                                                                                                                                                                                              
                                             /*******************************************************************************                                                                                                                              
                                             *                                  PORT D                                      *                                                                                                                              
                                             *******************************************************************************/                                                                                                                                                                                                                     
                                             PortConf_PD0_PORT_NUM   ,   PortConf_PD0_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD0 */ /* Index 20 Pin 20 */
                                             PortConf_PD1_PORT_NUM   ,   PortConf_PD1_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD1 */ /* Index 21 Pin 21 */
                                             PortConf_PD2_PORT_NUM   ,   PortConf_PD2_PIN_NUM   ,  MODE9       ,  PORT_PIN_IN   ,  PULL_DOWN ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD2 */ /* Index 22 Pin 22 */ /* MODE CHANGED TO MODE 9 C0o C1o */
                                             PortConf_PD3_PORT_NUM   ,   PortConf_PD3_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD3 */ /* Index 23 Pin 23 */
                                             PortConf_PD4_PORT_NUM   ,   PortConf_PD4_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD4 */ /* Index 24 Pin 24 */
                                             PortConf_PD5_PORT_NUM   ,   PortConf_PD5_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD5 */ /* Index 25 Pin 25 */
                                             PortConf_PD6_PORT_NUM   ,   PortConf_PD6_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD6 */ /* Index 26 Pin 26 */
                                             PortConf_PD7_PORT_NUM   ,   PortConf_PD7_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PD7 */ /* Index 27 Pin 27 */
                                                                                                                                                                                                                                                           
                                             /*******************************************************************************                                                                                                                              
                                             *                                  PORT E                                      *                                                                                                                              
                                             *******************************************************************************/                                                                                                                              
                                             PortConf_PE0_PORT_NUM   ,   PortConf_PE0_PIN_NUM   ,MODE15_ANALOG ,  PORT_PIN_IN   ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PE0 */ /* Index 28 Pin 28 */ /* MODE CHANGED TO MODE 15 ANALOG */
                                             PortConf_PE1_PORT_NUM   ,   PortConf_PE1_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PE1 */ /* Index 29 Pin 29 */
                                             PortConf_PE2_PORT_NUM   ,   PortConf_PE2_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_CHANGEABLE      , /* PE2 */ /* Index 30 Pin 30 */
                                             PortConf_PE3_PORT_NUM   ,   PortConf_PE3_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_CHANGEABLE      , /* PE3 */ /* Index 31 Pin 31 */ /* Pin Direction can be changed during runtime */
                                             PortConf_PE4_PORT_NUM   ,   PortConf_PE4_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PE4 */ /* Index 32 Pin 32 */
                                             PortConf_PE5_PORT_NUM   ,   PortConf_PE5_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PE5 */ /* Index 33 Pin 33 */
                                                                                                                                                                                                                                                                              
                                             /*******************************************************************************                                                                                                                              
                                             *                                  PORT F                                      *                                                                                                                              
                                             *******************************************************************************/                                                                                                                                                                                                                    
                                             PortConf_PF0_PORT_NUM   ,   PortConf_PF0_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PF0 */ /* Index 34 Pin 34 */
                                             PortConf_PF1_PORT_NUM   ,   PortConf_PF1_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PF1 */ /* Index 35 Pin 35 */ /* LED1 */
                                             PortConf_PF2_PORT_NUM   ,   PortConf_PF2_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PF2 */ /* Index 36 Pin 36 */
                                             PortConf_PF3_PORT_NUM   ,   PortConf_PF3_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_OUT  ,    OFF     ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE  , /* PF3 */ /* Index 37 Pin 37 */
                                             PortConf_PF4_PORT_NUM   ,   PortConf_PF4_PIN_NUM   ,  MODE0_DIO   ,  PORT_PIN_IN   ,  PULL_UP   ,   PORT_PIN_LEVEL_HIGH   ,   PORT_PIN_MODE_NOT_CHANGEABLE   ,   PORT_PIN_DIRECTION_NOT_CHANGEABLE    /* PF4 */ /* Index 38 Pin 38 */ /* SW1 */
                                                                                                                                                                                                                                                    
				         };