#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H


/* Error Interrupts */
#define CANDRIVER_CAN_IT_EWG                  ((unsigned int)0x00000100) /*!< Error warning Interrupt*/
#define CANDRIVER_CAN_IT_EPV                  ((unsigned int)0x00000200) /*!< Error passive Interrupt*/
#define CANDRIVER_CAN_IT_BOF                  ((unsigned int)0x00000400) /*!< Bus-off Interrupt*/
#define CANDRIVER_CAN_IT_LEC                  ((unsigned int)0x00000800) /*!< Last error code Interrupt*/
#define CANDRIVER_CAN_IT_ERR                  ((unsigned int)0x00008000) /*!< Error Interrupt*/

/* Receive Interrupts */
#define CANDRIVER_CAN_IT_FMP0                 ((unsigned int)0x00000002) /*!< FIFO 0 message pending Interrupt*/
#define CANDRIVER_CAN_IT_FF0                  ((unsigned int)0x00000004) /*!< FIFO 0 full Interrupt*/
#define CANDRIVER_CAN_IT_FOV0                 ((unsigned int)0x00000008) /*!< FIFO 0 overrun Interrupt*/
#define CANDRIVER_CAN_IT_FMP1                 ((unsigned int)0x00000010) /*!< FIFO 1 message pending Interrupt*/
#define CANDRIVER_CAN_IT_FF1                  ((unsigned int)0x00000020) /*!< FIFO 1 full Interrupt*/
#define CANDRIVER_CAN_IT_FOV1                 ((unsigned int)0x00000040) /*!< FIFO 1 overrun Interrupt*/

#define CanDriver_CAN_TxStatus_NoMailBox      ((unsigned char)0x04) /*!< CAN cell did not provide */

#define CanDriver_CAN_Id_Standard             ((unsigned int)0x00000000)  /*!< Standard Id */
#define CanDriver_CAN_Id_Extended             ((unsigned int)0x00000004)  /*!< Extended Id */

#define CanDriver_CAN_RTR_Data                ((unsigned int)0x00000000)  /*!< Data frame */

typedef struct
{
	unsigned int StdId;  /*!< Specifies the standard identifier.This parameter can be a value between 0 to 0x7FF. */

	unsigned int ExtId;  /*!< Specifies the extended identifier.This parameter can be a value between 0 to 0x1FFFFFFF. */

	unsigned char IDE;     /*!< Specifies the type of identifier for the message that will be transmitted. This parameter can be a value of @ref CAN_identifier_type */

	unsigned char RTR;     /*!< Specifies the type of frame for the message that will be transmitted. This parameter can be a value of @ref CAN_remote_transmission_request */

	unsigned char DLC;     /*!< Specifies the length of the frame that will be transmitted. This parameter can be a value between 0 to 8 */

	unsigned char Data[8]; /*!< Contains the data to be transmitted. It ranges from 0 to 0xFF. */
} CanDriver_CanTxMsg;

typedef struct
{
	unsigned int StdId;  /*!< Specifies the standard identifier.This parameter can be a value between 0 to 0x7FF. */

	unsigned int ExtId;  /*!< Specifies the extended identifier.This parameter can be a value between 0 to 0x1FFFFFFF. */

	unsigned char IDE;     /*!< Specifies the type of identifier for the message that will be received. This parameter can be a value of @ref CAN_identifier_type */

	unsigned char RTR;     /*!< Specifies the type of frame for the received message.This parameter can be a value of @ref CAN_remote_transmission_request */

	unsigned char DLC;     /*!< Specifies the length of the frame that will be received.This parameter can be a value between 0 to 8 */

	unsigned char Data[8]; /*!< Contains the data to be received. It ranges from 0 to 0xFF. */

	unsigned char FMI;     /*!< Specifies the index of the filter the message stored in the mailbox passes through. This parameter can be a value between 0 to 0xFF */
} CanDriver_CanRxMsg;


unsigned int CanDriver_Can1Init(unsigned char localId, unsigned int baudrate, unsigned char broadcastId);

unsigned char CanDriver_CAN_Transmit(void* canx, CanDriver_CanTxMsg *pTxMsg);

void CanDriver_CAN_Receive(void *canx, unsigned char FIFOx, void *pRxMsg);

void CanDriver_AddFilter(unsigned char id);

void CanDriver_ClearITPendingBit(void *canx, unsigned int it_flag);

unsigned int CanDriver_CAN_GetITStatus(void *canx, unsigned int it_flag);


#endif
