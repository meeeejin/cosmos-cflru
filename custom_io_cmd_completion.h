#ifndef SRC_CUSTOM_IO_CMD_COMPLETION_H_
#define SRC_CUSTOM_IO_CMD_COMPLETION_H_

#define PRINT_DEBUG_MSG		0
//#define PRINT_READ_STALL_LATENCY 0
#define MAX_CMD_SLOT_TAG	1024

typedef struct _CUSTOM_IO_CMD_COMPLETE_ENTRY
{
	unsigned int notCompletedRxCount: 8;
	unsigned int notIssuedTxCount: 8;
	unsigned int notCompletedTxCount: 8;
	unsigned int notCompletedNANDCount: 8;
} CUSTOM_IO_CMD_COMPLETE_ENTRY, *P_CUSTOM_IO_CMD_COMPLETE_ENTRY;

typedef struct _CUSTOM_IO_CMD_COMPLETE_TABLE
{
	CUSTOM_IO_CMD_COMPLETE_ENTRY customIoCmdCompleteEntry[MAX_CMD_SLOT_TAG];
} CUSTOM_IO_CMD_COMPLETE_TABLE, *P_CUSTOM_IO_CMD_COMPLETE_TABLE;

void InitCustomIoCmdCompleteTable();
void AllocateNotCompletedRxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr);
void AllocateNotIssuedTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr);
void AllocateNotCompletedTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr);
void AllocateNotCompletedNandReqforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int devAddr);
void CompleteRxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode);
void CompleteTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode);
void CompleteNandReqforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode);

#endif /* SRC_CUSTOM_IO_CMD_COMPLETION_H_ */
