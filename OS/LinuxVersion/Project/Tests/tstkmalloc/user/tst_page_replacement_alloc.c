/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 9 */
/* *********************************************************** */

#include <inc/lib.h>

char arr[PAGE_SIZE*12];
uint8* ptr = (uint8* )0x0801000 ;
uint8* ptr2 = (uint8* )0x0804000 ;

void _main(void)
{
	int envID = sys_getenvid();
//	cprintf("envID = %d\n",envID);

	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	//("STEP 0: checking Initial WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->__uptr_pws[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[6].virtual_address,PAGE_SIZE) !=   0x800000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[7].virtual_address,PAGE_SIZE) !=   0x801000)  panic("InitialWSError1! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[8].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError1! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("InitialWSError1INITIAL PAGE WS last index checking failed! Review size of the WS..!!");
	}

	int freePages = sys_calculate_free_frames();
	//cprintf("[BEFORE] free frames = %d\n", freePages) ;
	int usedDiskPages = sys_pf_calculate_allocated_pages();

	//Reading (Not Modified)
	char garbage1 = arr[PAGE_SIZE*11-1] ;
	char garbage2 = arr[PAGE_SIZE*12-1] ;

	//Writing (Modified)
	int i ;
	for (i = 0 ; i < PAGE_SIZE*10 ; i+=PAGE_SIZE/2)
	{
		arr[i] = -1 ;
		*ptr = *ptr2 ;
		ptr++ ; ptr2++ ;
	}

	//===================

	//cprintf("Checking Allocation in Mem & Page File... \n");
	{
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Unexpected extra/less pages have been added to page file.. NOT Expected to add new pages to the page file");

		uint32 freePagesAfter = (sys_calculate_free_frames() + sys_calculate_modified_frames());
		if( (freePages - freePagesAfter) != 0 )
			panic("Extra memory are wrongly allocated... It's REplacement: expected that no extra frames are allocated");

	}

	cprintf("[AUTO_GR@DING]Congratulations!! test PAGE replacement [ALLOCATION] is completed successfully.\n");
	return;
}
