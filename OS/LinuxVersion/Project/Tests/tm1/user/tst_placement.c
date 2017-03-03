/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 20 */
/* *********************************************************** */

#include <inc/lib.h>

void _main(void)
{
	int envID = sys_getenvid();
//	cprintf("envID = %d\n",envID);
	volatile struct Env* myEnv;
	myEnv = &(envs[envID]);

	char arr[PAGE_SIZE*1024*4];

	//("STEP 0: checking InitialWSError2: INITIAL WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->pageWorkingSet[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[6].virtual_address,PAGE_SIZE) !=   0x206000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[7].virtual_address,PAGE_SIZE) !=   0x800000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[8].virtual_address,PAGE_SIZE) !=   0x801000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[9].virtual_address,PAGE_SIZE) !=   0x802000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[10].virtual_address,PAGE_SIZE) !=   0x803000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[11].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->pageWorkingSet[12].virtual_address,PAGE_SIZE) !=   0xedbfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->pageWorkingSet[13].empty !=  1)  										panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  13)  											panic("InitialWSError2: INITIAL PAGE last index checking failed! Review size of the WS..!!");

	}

	int usedDiskPages = sys_pf_calculate_allocated_pages() ;
	int freePages = sys_calculate_free_frames();

	int i=0;
	for(;i<=PAGE_SIZE;i++)
	{
		arr[i] = -1;
	}

	i=PAGE_SIZE*1024;
	for(;i<=(PAGE_SIZE*1024 + PAGE_SIZE);i++)
	{
		arr[i] = -1;
	}

	i=PAGE_SIZE*1024*2;
	for(;i<=(PAGE_SIZE*1024*2 + PAGE_SIZE);i++)
	{
		arr[i] = -1;
	}



	cprintf("STEP A: checking PLACEMENT fault handling ... \n");
	{
		if( arr[0] !=  -1)  panic("PLACEMENT of stack page failed");
		if( arr[PAGE_SIZE] !=  -1)  panic("PLACEMENT of stack page failed");

		if( arr[PAGE_SIZE*1024] !=  -1)  panic("PLACEMENT of stack page failed");
		if( arr[PAGE_SIZE*1025] !=  -1)  panic("PLACEMENT of stack page failed");

		if( arr[PAGE_SIZE*1024*2] !=  -1)  panic("PLACEMENT of stack page failed");
		if( arr[PAGE_SIZE*1024*2 + PAGE_SIZE] !=  -1)  panic("PLACEMENT of stack page failed");


		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  5) panic("new stack pages are not written to Page File");

		if( (freePages - sys_calculate_free_frames() ) != 9 ) panic("allocated memory size incorrect");
	}
	cprintf("STEP A passed: PLACEMENT fault handling works!\n\n\n");



	cprintf("STEP B: checking WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->pageWorkingSet[0].virtual_address,PAGE_SIZE) !=   0x200000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[6].virtual_address,PAGE_SIZE) !=   0x206000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[7].virtual_address,PAGE_SIZE) !=   0x800000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[8].virtual_address,PAGE_SIZE) !=   0x801000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[9].virtual_address,PAGE_SIZE) !=   0x802000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[10].virtual_address,PAGE_SIZE) !=   0x803000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[11].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[12].virtual_address,PAGE_SIZE) !=  0xedbfd000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[13].virtual_address,PAGE_SIZE) !=  0xedbfe000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[14].virtual_address,PAGE_SIZE) !=  0xedffd000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[15].virtual_address,PAGE_SIZE) !=  0xedffe000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[16].virtual_address,PAGE_SIZE) !=  0xee3fd000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
		if( ROUNDDOWN(myEnv->pageWorkingSet[17].virtual_address,PAGE_SIZE) !=  0xee3fe000)  panic("PAGE WS entry checking failed... trace it by printing page WS before & after fault");
	}
	cprintf("STEP B passed: WS entries test are correct\n\n\n");

	cprintf("STEP C: checking working sets pointer locations...\n");
	{
		if(myEnv->page_last_WS_index != 18) panic("wrong PAGE WS pointer location... trace it by printing page WS before & after fault");

		i=PAGE_SIZE*1024*3;
		for(;i<=(PAGE_SIZE*1024*3+PAGE_SIZE);i++)
		{
			arr[i] = -1;
		}

		if( arr[PAGE_SIZE*1024*3] !=  -1)  panic("PLACEMENT of stack page failed");
		if( arr[PAGE_SIZE*1024*3 + PAGE_SIZE] !=  -1)  panic("PLACEMENT of stack page failed");

		if( ROUNDDOWN(myEnv->pageWorkingSet[19].virtual_address,PAGE_SIZE) !=  0xee7fe000)  panic("LAST PAGE WS entry checking failed");

		if(myEnv->page_last_WS_index != 0) panic("wrong PAGE WS pointer location... trace it by printing page WS before & after fault");

	}
	cprintf("STEP C passed: pointers reached zero\n\n\n");

	cprintf("Congratulations!! Test of PAGE PLACEMENT completed successfully!!\n\n\n");
	return;
}

