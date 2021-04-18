 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohamed Taha
 *
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_OFF)

/* Pre-compile option for presence of Port_SetPinDirection API */
#define PORT_SET_PIN_DIRECTION_API           (STD_ON)

/* Pre-compile option for presence of Port_SetPinMode API */
#define PORT_SET_PIN_MODE_API                (STD_ON)

/* Number of the configured Port Pins */
#define PORT_CONFIGURED_PINS              (39U)
   
/* Number of the configured Modes */
#define PORT_CONFIGURED_MODES              (18U)

/*******************************************************************************
 *          Channel Index in the array of structures in Port_PBcfg.c           *
 *******************************************************************************/
   
                          /* PORT A (PA0 -> PA7) */
#define PortConf_PA0_CHANNEL_ID_INDEX            (uint8)0x00
#define PortConf_PA1_CHANNEL_ID_INDEX            (uint8)0x01
#define PortConf_PA2_CHANNEL_ID_INDEX            (uint8)0x02
#define PortConf_PA3_CHANNEL_ID_INDEX            (uint8)0x03
#define PortConf_PA4_CHANNEL_ID_INDEX            (uint8)0x04
#define PortConf_PA5_CHANNEL_ID_INDEX            (uint8)0x05
#define PortConf_PA6_CHANNEL_ID_INDEX            (uint8)0x06
#define PortConf_PA7_CHANNEL_ID_INDEX            (uint8)0x07
   
                          /* PORT B (PB0 -> PB7) */      
#define PortConf_PB0_CHANNEL_ID_INDEX            (uint8)0x08
#define PortConf_PB1_CHANNEL_ID_INDEX            (uint8)0x09
#define PortConf_PB2_CHANNEL_ID_INDEX            (uint8)0x10
#define PortConf_PB3_CHANNEL_ID_INDEX            (uint8)0x11
#define PortConf_PB4_CHANNEL_ID_INDEX            (uint8)0x12
#define PortConf_PB5_CHANNEL_ID_INDEX            (uint8)0x13
#define PortConf_PB6_CHANNEL_ID_INDEX            (uint8)0x14
#define PortConf_PB7_CHANNEL_ID_INDEX            (uint8)0x15

                          /* PORT C (PC4 -> PC7) */
    /* NOTE : PC0 -> PC3 are used in JTAG connections so they aren't configured */
#define PortConf_PC4_CHANNEL_ID_INDEX            (uint8)0x16
#define PortConf_PC5_CHANNEL_ID_INDEX            (uint8)0x17
#define PortConf_PC6_CHANNEL_ID_INDEX            (uint8)0x18
#define PortConf_PC7_CHANNEL_ID_INDEX            (uint8)0x19
   
                          /* PORT D (PD0 -> PD7) */      
#define PortConf_PD0_CHANNEL_ID_INDEX            (uint8)0x20
#define PortConf_PD1_CHANNEL_ID_INDEX            (uint8)0x21
#define PortConf_PD2_CHANNEL_ID_INDEX            (uint8)0x22
#define PortConf_PD3_CHANNEL_ID_INDEX            (uint8)0x23
#define PortConf_PD4_CHANNEL_ID_INDEX            (uint8)0x24
#define PortConf_PD5_CHANNEL_ID_INDEX            (uint8)0x25
#define PortConf_PD6_CHANNEL_ID_INDEX            (uint8)0x26
#define PortConf_PD7_CHANNEL_ID_INDEX            (uint8)0x27
   
                          /* PORT E (PE0 -> PE5) */      
#define PortConf_PE0_CHANNEL_ID_INDEX            (uint8)0x28
#define PortConf_PE1_CHANNEL_ID_INDEX            (uint8)0x29
#define PortConf_PE2_CHANNEL_ID_INDEX            (uint8)0x30
#define PortConf_PE3_CHANNEL_ID_INDEX            (uint8)0x31
#define PortConf_PE4_CHANNEL_ID_INDEX            (uint8)0x32
#define PortConf_PE5_CHANNEL_ID_INDEX            (uint8)0x33

                          /* PORT F (PF0 -> PF4) */
#define PortConf_PF0_CHANNEL_ID_INDEX            (uint8)0x34
#define PortConf_PF1_CHANNEL_ID_INDEX            (uint8)0x35
#define PortConf_PF2_CHANNEL_ID_INDEX            (uint8)0x36
#define PortConf_PF3_CHANNEL_ID_INDEX            (uint8)0x37
#define PortConf_PF4_CHANNEL_ID_INDEX            (uint8)0x38

/*******************************************************************************
 *                          PORT Configured Port ID's                          *
 *******************************************************************************/

                          /* PORT A (PA0 -> PA7) */
#define PortConf_PA0_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA1_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA2_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA3_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA4_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA5_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA6_PORT_NUM                    (Port_PinType)0 /* PORTA */
#define PortConf_PA7_PORT_NUM                    (Port_PinType)0 /* PORTA */

                          /* PORT B (PB0 -> PB7) */
#define PortConf_PB0_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB1_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB2_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB3_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB4_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB5_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB6_PORT_NUM        		 (Port_PinType)1 /* PORTB */
#define PortConf_PB7_PORT_NUM        		 (Port_PinType)1 /* PORTB */

                          /* PORT C (PC0 -> PC7) */
#define PortConf_PC0_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC1_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC2_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC3_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC4_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC5_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC6_PORT_NUM       		 (Port_PinType)2 /* PORTC */
#define PortConf_PC7_PORT_NUM       		 (Port_PinType)2 /* PORTC */
										 
                          /* PORT D (PD0 -> PD7) */  
#define PortConf_PD0_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD1_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD2_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD3_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD4_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD5_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD6_PORT_NUM        		 (Port_PinType)3 /* PORTD */
#define PortConf_PD7_PORT_NUM        		 (Port_PinType)3 /* PORTD */
										 
                          /* PORT E (PE0 -> PE5) */  
#define PortConf_PE0_PORT_NUM        		 (Port_PinType)4 /* PORTE */
#define PortConf_PE1_PORT_NUM        		 (Port_PinType)4 /* PORTE */
#define PortConf_PE2_PORT_NUM        		 (Port_PinType)4 /* PORTE */
#define PortConf_PE3_PORT_NUM        		 (Port_PinType)4 /* PORTE */
#define PortConf_PE4_PORT_NUM        		 (Port_PinType)4 /* PORTE */
#define PortConf_PE5_PORT_NUM        		 (Port_PinType)4 /* PORTE */
											
                          /* PORT F (PF0 -> PF4) */  
#define PortConf_PF0_PORT_NUM        		 (Port_PinType)5 /* PORTF */
#define PortConf_PF1_PORT_NUM        		 (Port_PinType)5 /* PORTF */
#define PortConf_PF2_PORT_NUM        		 (Port_PinType)5 /* PORTF */
#define PortConf_PF3_PORT_NUM        		 (Port_PinType)5 /* PORTF */
#define PortConf_PF4_PORT_NUM        		 (Port_PinType)5 /* PORTF */

/*******************************************************************************
 *                          PORT Configured Pins ID's                          *
 *******************************************************************************/

                          /* PORT A (PA0 -> PA7) */
#define PortConf_PA0_PIN_NUM                 (Port_PinType)0 /* Pin 0 in PORTA */
#define PortConf_PA1_PIN_NUM                 (Port_PinType)1 /* Pin 1 in PORTA */
#define PortConf_PA2_PIN_NUM                 (Port_PinType)2 /* Pin 2 in PORTA */
#define PortConf_PA3_PIN_NUM                 (Port_PinType)3 /* Pin 3 in PORTA */
#define PortConf_PA4_PIN_NUM                 (Port_PinType)4 /* Pin 4 in PORTA */
#define PortConf_PA5_PIN_NUM                 (Port_PinType)5 /* Pin 5 in PORTA */
#define PortConf_PA6_PIN_NUM                 (Port_PinType)6 /* Pin 6 in PORTA */
#define PortConf_PA7_PIN_NUM                 (Port_PinType)7 /* Pin 7 in PORTA */

                          /* PORT B (PB0 -> PB7) */
#define PortConf_PB0_PIN_NUM        	     (Port_PinType)0 /* Pin 0 in PORTB */
#define PortConf_PB1_PIN_NUM        	     (Port_PinType)1 /* Pin 1 in PORTB */
#define PortConf_PB2_PIN_NUM        	     (Port_PinType)2 /* Pin 2 in PORTB */
#define PortConf_PB3_PIN_NUM        	     (Port_PinType)3 /* Pin 3 in PORTB */
#define PortConf_PB4_PIN_NUM        	     (Port_PinType)4 /* Pin 4 in PORTB */
#define PortConf_PB5_PIN_NUM        	     (Port_PinType)5 /* Pin 5 in PORTB */
#define PortConf_PB6_PIN_NUM        	     (Port_PinType)6 /* Pin 6 in PORTB */
#define PortConf_PB7_PIN_NUM        	     (Port_PinType)7 /* Pin 7 in PORTB */

                          /* PORT C (PC0 -> PC7) */
#define PortConf_PC0_PIN_NUM       	     (Port_PinType)0 /* Pin 0 in PORTC */
#define PortConf_PC1_PIN_NUM       	     (Port_PinType)1 /* Pin 1 in PORTC */
#define PortConf_PC2_PIN_NUM       	     (Port_PinType)2 /* Pin 2 in PORTC */
#define PortConf_PC3_PIN_NUM       	     (Port_PinType)3 /* Pin 3 in PORTC */
#define PortConf_PC4_PIN_NUM       	     (Port_PinType)4 /* Pin 4 in PORTC */
#define PortConf_PC5_PIN_NUM       	     (Port_PinType)5 /* Pin 5 in PORTC */
#define PortConf_PC6_PIN_NUM       	     (Port_PinType)6 /* Pin 6 in PORTC */
#define PortConf_PC7_PIN_NUM       	     (Port_PinType)7 /* Pin 7 in PORTC */
										 
                          /* PORT D (PD0 -> PD7) */  
#define PortConf_PD0_PIN_NUM        	     (Port_PinType)0 /* Pin 0 in PORTD */
#define PortConf_PD1_PIN_NUM        	     (Port_PinType)1 /* Pin 1 in PORTD */
#define PortConf_PD2_PIN_NUM        	     (Port_PinType)2 /* Pin 2 in PORTD */
#define PortConf_PD3_PIN_NUM        	     (Port_PinType)3 /* Pin 3 in PORTD */
#define PortConf_PD4_PIN_NUM        	     (Port_PinType)4 /* Pin 4 in PORTD */
#define PortConf_PD5_PIN_NUM        	     (Port_PinType)5 /* Pin 5 in PORTD */
#define PortConf_PD6_PIN_NUM        	     (Port_PinType)6 /* Pin 6 in PORTD */
#define PortConf_PD7_PIN_NUM        	     (Port_PinType)7 /* Pin 7 in PORTD */
										 
                          /* PORT E (PE0 -> PE5) */  
#define PortConf_PE0_PIN_NUM        	     (Port_PinType)0 /* Pin 0 in PORTE */
#define PortConf_PE1_PIN_NUM        	     (Port_PinType)1 /* Pin 1 in PORTE */
#define PortConf_PE2_PIN_NUM        	     (Port_PinType)2 /* Pin 2 in PORTE */
#define PortConf_PE3_PIN_NUM        	     (Port_PinType)3 /* Pin 3 in PORTE */
#define PortConf_PE4_PIN_NUM        	     (Port_PinType)4 /* Pin 4 in PORTE */
#define PortConf_PE5_PIN_NUM        	     (Port_PinType)5 /* Pin 5 in PORTE */
											
                          /* PORT F (PF0 -> PF4) */  
#define PortConf_PF0_PIN_NUM        	     (Port_PinType)0 /* Pin 0 in PORTF */
#define PortConf_PF1_PIN_NUM        	     (Port_PinType)1 /* Pin 1 in PORTF */
#define PortConf_PF2_PIN_NUM        	     (Port_PinType)2 /* Pin 2 in PORTF */
#define PortConf_PF3_PIN_NUM        	     (Port_PinType)3 /* Pin 3 in PORTF */
#define PortConf_PF4_PIN_NUM        	     (Port_PinType)4 /* Pin 4 in PORTF */

/*******************************************************************************
 *                                PIN Modes ID's                               *
 *******************************************************************************/
/* Description:
 *	       MODE 0        : All Pins are used as Digital IO in this mode.
 *	       MODE 1  -> 9  : Alternative Function , differs from Pin to another Pin.
 *	       MODE 10 -> 13 : No Alternative Functions.
 *	       MODE 14       : Alternative Function , differs from Pin to another Pin.
 *             MODE Ain(15)  : All Pins are used as Analog IO in this mode.
 */
#define MODE0_DIO             		         (Port_PinModeType)0 
#define MODE1           		         (Port_PinModeType)1  
#define MODE2           		         (Port_PinModeType)2
#define MODE3       		                 (Port_PinModeType)3 
#define MODE4           		         (Port_PinModeType)4  
#define MODE5           		         (Port_PinModeType)5
#define MODE6       		                 (Port_PinModeType)6 
#define MODE7           		         (Port_PinModeType)7  
#define MODE8           		         (Port_PinModeType)8
#define MODE9       		                 (Port_PinModeType)9 
#define MODE14           		         (Port_PinModeType)10  
#define MODE15_ANALOG          		         (Port_PinModeType)11

/*******************************************************************************
 *                    PIN Modes Masks for PMC Register                         *
 *******************************************************************************/
#define MODE0_PMC_MASK                           (uint32)0x0000000F
#define MODE1_PMC_MASK                           (uint32)0x00000001
#define MODE2_PMC_MASK                           (uint32)0x00000002
#define MODE3_PMC_MASK                           (uint32)0x00000003
#define MODE4_PMC_MASK                           (uint32)0x00000004
#define MODE5_PMC_MASK                           (uint32)0x00000005
#define MODE6_PMC_MASK                           (uint32)0x00000006
#define MODE7_PMC_MASK                           (uint32)0x00000007
#define MODE8_PMC_MASK                           (uint32)0x00000008
#define MODE9_PMC_MASK                           (uint32)0x00000009
#define MODE14_PMC_MASK                          (uint32)0x0000000E
/*Note Mode Ain(15) has no PMC Mask */

/* Magic number used for unlocking Commit Register GPIOCR */
#define GPIO_LOCK_NUM                            (uint32)0x4C4F434B

/*******************************************************************************
 *          Channel Index used for SetDirection and SetMode Functions          *
 *******************************************************************************/
   
                          /* PORT A (PA0 -> PA7) */
#define PortConf_PA0_PIN_ID_INDEX                (Port_PinType)0
#define PortConf_PA1_PIN_ID_INDEX                (Port_PinType)1
#define PortConf_PA2_PIN_ID_INDEX                (Port_PinType)2
#define PortConf_PA3_PIN_ID_INDEX                (Port_PinType)3
#define PortConf_PA4_PIN_ID_INDEX                (Port_PinType)4
#define PortConf_PA5_PIN_ID_INDEX                (Port_PinType)5
#define PortConf_PA6_PIN_ID_INDEX                (Port_PinType)6
#define PortConf_PA7_PIN_ID_INDEX                (Port_PinType)7
   
                          /* PORT B (PB0 -> PB7) */      
#define PortConf_PB0_PIN_ID_INDEX                (Port_PinType)8
#define PortConf_PB1_PIN_ID_INDEX                (Port_PinType)9
#define PortConf_PB2_PIN_ID_INDEX                (Port_PinType)10
#define PortConf_PB3_PIN_ID_INDEX                (Port_PinType)11
#define PortConf_PB4_PIN_ID_INDEX                (Port_PinType)12
#define PortConf_PB5_PIN_ID_INDEX                (Port_PinType)13
#define PortConf_PB6_PIN_ID_INDEX                (Port_PinType)14
#define PortConf_PB7_PIN_ID_INDEX                (Port_PinType)15

                          /* PORT C (PC4 -> PC7) */
    /* NOTE : PC0 -> PC3 are used in JTAG connections so they aren't configured */
#define PortConf_PC4_PIN_ID_INDEX                (Port_PinType)16
#define PortConf_PC5_PIN_ID_INDEX                (Port_PinType)17
#define PortConf_PC6_PIN_ID_INDEX                (Port_PinType)18
#define PortConf_PC7_PIN_ID_INDEX                (Port_PinType)19
   
                          /* PORT D (PD0 -> PD7) */      
#define PortConf_PD0_PIN_ID_INDEX                (Port_PinType)20
#define PortConf_PD1_PIN_ID_INDEX                (Port_PinType)21
#define PortConf_PD2_PIN_ID_INDEX                (Port_PinType)22
#define PortConf_PD3_PIN_ID_INDEX                (Port_PinType)23
#define PortConf_PD4_PIN_ID_INDEX                (Port_PinType)24
#define PortConf_PD5_PIN_ID_INDEX                (Port_PinType)25
#define PortConf_PD6_PIN_ID_INDEX                (Port_PinType)26
#define PortConf_PD7_PIN_ID_INDEX                (Port_PinType)27
   
                          /* PORT E (PE0 -> PE5) */      
#define PortConf_PE0_PIN_ID_INDEX                (Port_PinType)28
#define PortConf_PE1_PIN_ID_INDEX                (Port_PinType)29
#define PortConf_PE2_PIN_ID_INDEX                (Port_PinType)30
#define PortConf_PE3_PIN_ID_INDEX                (Port_PinType)31
#define PortConf_PE4_PIN_ID_INDEX                (Port_PinType)32
#define PortConf_PE5_PIN_ID_INDEX                (Port_PinType)33

                          /* PORT F (PF0 -> PF4) */
#define PortConf_PF0_PIN_ID_INDEX                (Port_PinType)34
#define PortConf_PF1_PIN_ID_INDEX                (Port_PinType)35
#define PortConf_PF2_PIN_ID_INDEX                (Port_PinType)36
#define PortConf_PF3_PIN_ID_INDEX                (Port_PinType)37
#define PortConf_PF4_PIN_ID_INDEX                (Port_PinType)38


#endif /* PORT_CFG_H */
