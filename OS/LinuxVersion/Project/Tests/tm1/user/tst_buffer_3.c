/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 10 */
/* MAKE SURE MAX_MODIFIED_LIST_COUNT = 1000 */
/* *********************************************************** */

#include <inc/lib.h>

void _main(void)
{
	int envID = sys_getenvid();
	//	cprintf("envID = %d\n",envID);

	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	//("STEP 0: checking Initial WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->pageWorkingSet[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[6].virtual_address,PAGE_SIZE) !=   0x800000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[7].virtual_address,PAGE_SIZE) !=   0x801000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[8].virtual_address,PAGE_SIZE) !=   0x802000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[9].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("InitialWSError2: INITIAL PAGE WS last index checking failed! Review size of the WS..!!");
	}
	int kilo = 1024;
	int Mega = 1024*1024;

	{
		int freeFrames = sys_calculate_free_frames() ;
		int origFreeFrames = freeFrames ;

		uint32 size = 10*Mega;
		unsigned char *x = malloc(sizeof(unsigned char)*size) ;
		freeFrames = sys_calculate_free_frames() ;
		int modFrames = sys_calculate_modified_frames();

		cprintf("all frames AFTER malloc = %d\n", freeFrames + modFrames);
		x[1]=-1;

		x[1*Mega] = -1;

		int i = x[2*Mega];

		int j = x[3*Mega];

		x[4*Mega] = -1;

		x[5*Mega] = -1;

		x[6*Mega] = -1;

		x[7*Mega] = -1;

		x[8*Mega] = -1;

		x[9*Mega] = -1;

		free(x);

		int numOFEmptyLocInWS = 0;
		for (i = 0 ; i < PAGE_WS_MAX_SIZE; i++)
		{
			if (myEnv->pageWorkingSet[i].empty)
			{
				numOFEmptyLocInWS++;
			}
			else
			{
				uint32 va = ROUNDDOWN(myEnv->pageWorkingSet[i].virtual_address,PAGE_SIZE) ;
				if (va >= USER_HEAP_START && va < (USER_HEAP_START + size))
					panic("freeMem didn't remove its page(s) from the WS");
			}
		}
		if ((sys_calculate_modified_frames() + sys_calculate_free_frames() - numOFEmptyLocInWS) - (modFrames + freeFrames) != 0 ) panic("FreeMem didn't remove all modified frames in the given range from the modified list");

		//if (sys_calculate_modified_frames() != 0 ) panic("FreeMem didn't remove all modified frames in the given range from the modified list");
		//if (sys_calculate_notmod_frames() != 7) panic("FreeMem didn't remove all un-modified frames in the given range from the free frame list");

		//if (sys_calculate_free_frames() - freeFrames != 3) panic("FreeMem didn't UN-BUFFER the removed BUFFERED frames in the given range.. (check updating of isBuffered");

		cprintf("Congratulations!! test of removing BUFFERED pages in freeHeap is completed successfully.\n");

		//Try to access any of the removed buffered pages in the Heap [It's ILLEGAL ACCESS now]
		{
			cprintf("\nNow, trying to access the removed BUFFERED pages, you should make the kernel PANIC with ILLEGAL MEMORY ACCESS in page_fault_handler() since we have illegal access to page that is NOT EXIST in PF and NOT BELONGS to STACK.\n\n\n");

			x[1]=-1;

			x[1*Mega] = -1;

			int i = x[2*Mega];

			int j = x[3*Mega];
		}
		panic("ERROR: FOS SHOULD NOT panic here, it should panic earlier in page_fault_handler(), since we have illegal access to page that is NOT EXIST in PF and NOT BELONGS to STACK. REMEMBER: creating new page in page file shouldn't be allowed except ONLY for stack pages\n");
	}

	return;
}

