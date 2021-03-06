/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.0
*
* Description: This project demonstrates connectivity between the PSoC 6 BLE and 
*              CySmart BLE host Emulation tool or mobile device running the CySmart
*              mobile application, and also demonstrates four independent slaves 
*              that talks to four independent masters.
*
* Related Document: CE223508_PSoC6_BLE_MultiSlave_RTOS.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
*******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/

#include "board_adapter.h"
#include "uart_debug.h"
#include "tickless_idle.h"
#include "tuya_ble_app_demo.h"

/* Function that returns the Tickless Idle readiness of the system */
bool IsSystemReadyForTicklessIdle(void);

int main(void) {   
    /* Configure switch SW2 as hibernate wake up source */
    //Cy_SysPm_SetHibWakeupSource(CY_SYSPM_HIBPIN1_LOW);

    board_init();
    
    /* Create ble task. */    
    ble_pdl_task_init();
    
    tuya_ble_app_task_init();
    
    /** 
     * Initialize thread-safe debug message printing. 
     * See uart_debug.h header file to enable / disable this feature 
     */
    Task_DebugInit();

    /* Initialize Tickless Idle mode */                    
    TicklessIdleInit(IsSystemReadyForTicklessIdle);
    
    /* Start the RTOS scheduler. This function should never return */
    DebugPrintf("RTOS - scheduler\r\n");
    vTaskStartScheduler();
    
    /* Should never get here! */ 
    DebugPrintf("Error!   : RTOS - scheduler crashed \r\n");
    
    /* Halt the CPU if scheduler exits */
    CY_ASSERT(0);
    
    for(;;)
    {
    }	
}

/*******************************************************************************
* Function Name: bool IsSystemReadyForTicklessIdle(void)
********************************************************************************
*
* Summary:
*  This function returns the Tickless Idle readiness of the system.
*    
* Parameters:
*  None
*
* Return:
*  bool: Tickless Idle readiness of the system.
*
*******************************************************************************/
bool IsSystemReadyForTicklessIdle(void)
{
    return  ((Task_Ble_Tickless_Idle_Readiness())           &&
             (Task_Debug_Tickless_Idle_Readiness())         &&                            
             (Task_Factory_Test_Tickless_Idle_Readiness())        
    );
}

/*******************************************************************************
* Function Name: void vApplicationIdleHook(void)
********************************************************************************
*
* Summary:
*  This function is called when the RTOS in idle mode
*    
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void vApplicationIdleHook(void)
{
    /* Enter sleep-mode */
    Cy_SysPm_Sleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
}

/*******************************************************************************
* Function Name: void vApplicationStackOverflowHook(TaskHandle_t *pxTask, 
*                                                   signed char *pcTaskName)
********************************************************************************
*
* Summary:
*  This function is called when a stack overflow has been detected by the RTOS
*    
* Parameters:
*  TaskHandle_t  : Handle to the task
*  signed char   : Name of the task
*
* Return:
*  None
*
*******************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t *pxTask, 
                                   signed char *pcTaskName)
{
    /* Remove warning for unused parameters */
    (void)pxTask;
    
    /** 
     * Print the error message with task name if debug is enabled in 
     * uart_debug.h file 
     */
    DebugPrintf("Error!   : RTOS - stack overflow in %s \r\n", pcTaskName);
    
    /* Halt the CPU */
    CY_ASSERT(0);
}

/*******************************************************************************
* Function Name: void vApplicationMallocFailedHook(void)
********************************************************************************
*
* Summary:
*  This function is called when a memory allocation operation by the RTOS
*  has failed
*    
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void vApplicationMallocFailedHook(void)
{
    /* Print the error message if debug is enabled in uart_debug.h file */
    DebugPrintf("Error!   : RTOS - Memory allocation failed \r\n");
    
    /* Halt the CPU */
    CY_ASSERT(0);
}

/* [] END OF FILE */
