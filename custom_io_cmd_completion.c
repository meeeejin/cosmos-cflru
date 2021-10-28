#include "xil_printf.h"
#include "memory_map.h"
#include "nvme/host_lld.h"
#include "request_schedule.h"

P_CUSTOM_IO_CMD_COMPLETE_TABLE customIoCmdCompleteTablePtr;

void InitCustomIoCmdCompleteTable()
{
	unsigned int cmdSlotTag;
	customIoCmdCompleteTablePtr = (P_CUSTOM_IO_CMD_COMPLETE_TABLE) CUSTOM_IO_CMD_COMPLETE_TABLE_ADDR;

	for(cmdSlotTag=0; cmdSlotTag < MAX_CMD_SLOT_TAG; cmdSlotTag++)
	{
		customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount = 0;
		customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notIssuedTxCount =0;
		customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount = 0;
		customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedNANDCount = 0;
	}
}

void AllocateNotCompletedRxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount++;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Allo Rx Custom Io CmdSlotTag:%d Cnt:%d devAddr:%08x\r\n", cmdSlotTag,
			customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount, devAddr);
#endif
}

void AllocateNotIssuedTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notIssuedTxCount++;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Allo Tx Custom Io (Issue) CmdSlotTag:%d Cnt:%d devAddr:%08x\r\n", cmdSlotTag,
			customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notIssuedTxCount, devAddr);
#endif
}

void AllocateNotCompletedTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int devAddr)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount++;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Allo Tx Custom Io (Complete) CmdSlotTag:%d Cnt:%d devAddr:%08x\r\n", cmdSlotTag,
			customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount, devAddr);
#endif
}

void AllocateNotCompletedNandReqforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int devAddr)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedNANDCount++;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Allo NandReq Custom Io CmdSlotTag:%d Cnt:%d devAddr:%08x\r\n", cmdSlotTag,
			customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedNANDCount, devAddr);
#endif
}

void CompleteRxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount--;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Discount notCompletedRxCount Cnt:%d\r\n", customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount);
	xil_printf("Complete Rx Custom Io CmdSlotTag:%d reqSlotTag:%d\r\n", cmdSlotTag, reqSlotTag);
	PrintDataBuffer(reqSlotTag, 1);
#endif
}

void IssuedTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notIssuedTxCount--;
	#if (PRINT_DEBUG_MSG == 1)
		xil_printf("Discount notIssuedTxCount Cnt:%d\r\n", customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notIssuedTxCount);
		//xil_printf("Issue Tx Custom Io CmdSlotTag:%d reqSlotTag:%d\r\n", cmdSlotTag, reqSlotTag);
		PrintDataBuffer(reqSlotTag, 1);
	#endif
}

void CompleteTxforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode)
{
	NVME_COMPLETION nvmeCPL;
	nvmeCPL.statusFieldWord = 0;
	nvmeCPL.statusField.SCT = SCT_GENERIC_COMMAND_STATUS;
	nvmeCPL.statusField.SC = SC_SUCCESSFUL_COMPLETION;
	nvmeCPL.statusField.DNR = 1;
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount--;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Discount notCompletedTxCount Cnt:%d\r\n", customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount);
#endif
	if(customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedTxCount == 0
			&& customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedRxCount == 0)
	{
		set_auto_nvme_cpl(cmdSlotTag, 0, nvmeCPL.statusFieldWord);
#if (PRINT_DEBUG_MSG == 1)
		xil_printf("Complete Tx Custom Io CmdSlotTag:%d reqSlotTag:%d NvmeBlockCnt:%d\r\n", cmdSlotTag, reqSlotTag,
				reqPoolPtr->reqPool[reqSlotTag].nvmeDmaInfo.numOfNvmeBlock);
		PrintDataBuffer(reqSlotTag, reqPoolPtr->reqPool[reqSlotTag].nvmeDmaInfo.numOfNvmeBlock);
#endif
	}
}

void CompleteNandReqforCustomIoCmd(unsigned int cmdSlotTag, unsigned int reqSlotTag, unsigned int reqCode)
{
	customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedNANDCount--;
#if (PRINT_DEBUG_MSG == 1)
	xil_printf("Complete NandReq Custom Io CmdSlotTag:%d Cnt:%d devAddr:%08x reqCode:%x\r\n", cmdSlotTag,
			customIoCmdCompleteTablePtr->customIoCmdCompleteEntry[cmdSlotTag].notCompletedNANDCount, GenerateDataBufAddr(reqSlotTag), reqCode);
#endif
}
