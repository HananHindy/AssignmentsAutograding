//tests pages are buffered correctly
/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 9 */
/* MAKE SURE MAX_MODIFIED_LIST_COUNT = 1000 */
/* *********************************************************** */

#include <inc/lib.h>

#define arrSize PAGE_SIZE*6 / 4
int src[arrSize];
int dst[arrSize];

void _main(void)
{	
	int envID = sys_getenvid();
	//	cprintf("envID = %d\n",envID);

	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	//("STEP 0: checking InitialWSError2: INITIAL WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->pageWorkingSet[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[6].virtual_address,PAGE_SIZE) !=   0x800000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[7].virtual_address,PAGE_SIZE) !=   0x801000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
	//	if( ROUNDDOWN(myEnv->pageWorkingSet[8].virtual_address,PAGE_SIZE) !=   0x802000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[8].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review sizes of the two WS's..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("InitialWSError2: INITIAL PAGE WS last index checking failed! Review sizes of the two WS's..!!");
	}

	int initModBufCnt = sys_calculate_modified_frames();
	int initFreeBufCnt = sys_calculate_notmod_frames();
	int usedDiskPages = sys_pf_calculate_allocated_pages();

	//[1]Bring 6 pages and modify them (6 unmodified will be buffered)
	int i=0;
	int dstSum1 = 0;
	for(i=0;i<arrSize;i+=PAGE_SIZE/4)
	{
		dst[i] = i;
		dstSum1 += i;
	}


	if (sys_calculate_notmod_frames()  - initFreeBufCnt != 6)  panic("Error in BUFFERING/RESTORING of free frames... WRONG number of buffered pages in FREE frame list");
	if (sys_calculate_modified_frames() - initModBufCnt  != 0)  panic("Error in BUFFERING/RESTORING of modified frames... WRONG number of buffered pages in MODIFIED frame list");

	initFreeBufCnt = sys_calculate_notmod_frames();
	initModBufCnt = sys_calculate_modified_frames();

	//[2]Bring 6 unmodified pages (6 modified will be buffered)
	int srcSum1 = 0 ;
	i = 0;
	for(;i<arrSize;i+=PAGE_SIZE/4)
	{
		srcSum1 += src[i];
	}

	//cprintf("sys_calculate_notmod_frames()  - initFreeBufCnt = %d\n", sys_calculate_notmod_frames()  - initFreeBufCnt);
	if (sys_calculate_notmod_frames()  - initFreeBufCnt != 0)  panic("Error in BUFFERING/RESTORING of free frames... WRONG number of buffered pages in FREE frame list");
	if (sys_calculate_modified_frames() - initModBufCnt  != 6)  panic("Error in BUFFERING/RESTORING of modified frames... WRONG number of buffered pages in MODIFIED frame list");
	initFreeBufCnt = sys_calculate_notmod_frames();
	initModBufCnt = sys_calculate_modified_frames();

	//[3]Bring the 6 modified pages again and ensure their values are correct (6 unmodified will be buffered)
	i = 0;
	int dstSum2 = 0 ;
	for(i=0;i<arrSize;i+=PAGE_SIZE/4)
	{
		dstSum2 += dst[i];
	}

	if (sys_calculate_notmod_frames()  - initFreeBufCnt != 6)  panic("Error in BUFFERING/RESTORING of free frames... WRONG number of buffered pages in FREE frame list");
	if (sys_calculate_modified_frames() - initModBufCnt  != -6)  panic("Error in BUFFERING/RESTORING of modified frames... WRONG number of buffered pages in MODIFIED frame list");

	initFreeBufCnt = sys_calculate_notmod_frames();
	initModBufCnt = sys_calculate_modified_frames();

	//[4]Bring the 6 unmodified pages again and ensure their values are correct (6 modified will be buffered)
	i = 0;
	int srcSum2 = 0 ;
	for(i=0;i<arrSize;i+=PAGE_SIZE/4)
	{
		srcSum2 += src[i];
	}

	if (sys_calculate_notmod_frames()  - initFreeBufCnt != -6)  panic("Error in BUFFERING/RESTORING of free frames... WRONG number of buffered pages in FREE frame list");
	if (sys_calculate_modified_frames() - initModBufCnt  != 6)  panic("Error in BUFFERING/RESTORING of modified frames... WRONG number of buffered pages in MODIFIED frame list");

	if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Unexpected extra/less pages have been added to page file.. NOT Expected to add any new page to the page file");

	if (srcSum1 != srcSum2 || dstSum1 != dstSum2) 	panic("Error in buffering/restoring modified/not modified pages") ;

	cprintf("Congratulations!! test buffered pages inside REPLACEMENT is completed successfully.\n");
	return;

}
