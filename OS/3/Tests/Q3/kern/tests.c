#include <kern/tests.h>
#include <kern/memory_manager.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

void TestAssignment3()
{
	//TestAss3Q1();
	//TestAss3Q2();
	TestAss3Q3();
	//TestAss3Q4();
	//TestAss3B1();
	//TestAss3B2();
}

int TestAss3Q1()
{
	// Case 1 : KERNEL_BASE and above
	char cr1[100] = "gfn 0xF0000000";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int fn = GetFrameNumber(args) ;
	if (fn != 0)
	{
		cprintf("Q1 Automatic Test1: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	char cr2[100] = "gfn 0xF0000100";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	fn = GetFrameNumber(args) ;
	if (fn != 0)
	{
		cprintf("Q1 Automatic Test2: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	char cr3[100] = "gfn 0xF0001100";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	fn = GetFrameNumber(args) ;
	if (fn != 1)
	{
		cprintf("Q1 Automatic Test3: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	char cr4[100] = "gfn 0xF0400000";
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	fn = GetFrameNumber(args) ;
	if (fn != 1024)
	{
		cprintf("Q1 Automatic Test4: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	// Case 2: Not mapped
	char cr5[100] = "gfn 0x100";
	strsplit(cr5, WHITESPACE, args, &numOfArgs) ;

	fn = GetFrameNumber(args) ;
	if (fn != -1)
	{
		cprintf("Q1 Automatic Test5: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	// Case 2: Not mapped
	char cr6[100] = "gfn 0x0F000000";
	strsplit(cr6, WHITESPACE, args, &numOfArgs) ;

	fn = GetFrameNumber(args) ;
	if (fn != -1)
	{
		cprintf("Q1 Automatic Test6: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	cprintf("Q1 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	return 0;
}

int TestAss3Q2()
{
	int i = 0;
	//Table #961 (Exists)
	char cr1[100] = "cum 961";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int cnt = CountUsedModifiedInTable(args) ;
	if (cnt != 0)
	{
		cprintf("Q2 Automatic Test1: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}
	char *ptr1, *ptr2, *ptr3;
	ptr1 = (char*)0xF0400000; *ptr1 = 'A' ;
	ptr2 = (char*)0xF0403000; *ptr2 = 'B' ;
	cnt = CountUsedModifiedInTable(args) ;
	if (cnt != 2)
	{
		cprintf("Q2 Automatic Test2: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}
	ptr3 = (char*)0xF0409000; *ptr2 = *ptr3 ;
	cnt = CountUsedModifiedInTable(args) ;
	if (cnt != 2)
	{
		cprintf("Q2 Automatic Test3: Failed. Evaluation = 25%\n");
		cprintf("[EV@LU@TI0N] 0.25\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Table #10 (not exist)
	char cr2[100] = "cum 10";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;
	cnt = CountUsedModifiedInTable(args) ;
	if (cnt != -1)
	{
		cprintf("Q2 Automatic Test4: Failed. Evaluation = 50%\n");
		cprintf("[EV@LU@TI0N] 0.5\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Table #2 (Create it! totally free)
	uint32 *ptr_table;
	get_page_table(ptr_page_directory, (void*)0x800000, 1, &ptr_table);
	char cr3[100] = "cfp 2";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;
	cnt = CountUsedModifiedInTable(args) ;
	if (cnt != 0)
	{
		cprintf("Q2 Automatic Test5: Failed. Evaluation = 75%\n");
		cprintf("[EV@LU@TI0N] 0.75\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Table #2 (map 3 pages inside it)
	map_frame(ptr_page_directory, &(frames_info[0]), (void*)0x800000, PERM_WRITEABLE);
	map_frame(ptr_page_directory, &(frames_info[256]), (void*)0x804000, PERM_WRITEABLE);
	map_frame(ptr_page_directory, &(frames_info[257]), (void*)0x80F000, PERM_WRITEABLE);
	char cr4[100] = "cfp 2";
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;
	cnt = CountUsedModifiedInTable(args) ;
	if (cnt != 0)
	{
		cprintf("Q2 Automatic Test6: Failed. Evaluation = 75%\n");
		cprintf("[EV@LU@TI0N] 0.75\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	cprintf("Q2 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	return 0;
}

int TestAss3Q3()
{
	int i = 0;
	//Not modified range
	char cr1[100] = "sm2nu 0xF0000000 0xF0005000";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int cnt = SetModifiedPagesInRangeToNotUsed(args) ;
	if (cnt != 0)
	{
		cprintf("Q3 Automatic Test1: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Modify 3 pages in the range
	char *ptr, *ptr2 ;
	ptr = (char*)0xF0000000 ; *ptr = 'A';
	ptr = (char*)0xF0000005 ; *ptr = 'B';
	ptr2 = (char*)0xF0003000 ; *ptr = *ptr2;
	ptr = (char*)0xF0004FFF ; *ptr = 'C';

	char cr2[100] = "sm2nu 0xF0000000 0xF0005000";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	cnt = SetModifiedPagesInRangeToNotUsed(args) ;
	if (cnt != 2)
	{
		cprintf("Q3 Automatic Test2: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Modify 1 page outside the range
	ptr = (char*)0xF0005000 ; *ptr = 'X';

	char cr3[100] = "sm2nu 0xF0000000 0xF0005000";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	cnt = SetModifiedPagesInRangeToNotUsed(args) ;
	if (cnt != 2)
	{
		cprintf("Q3 Automatic Test3: Failed. Evaluation = 25%\n");
		cprintf("[EV@LU@TI0N] 0.25\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Check used status
	if (CheckBit(0xF0000000, 5) || !CheckBit(0xF0003000, 5) || CheckBit(0xF0004000, 5))
	{
		cprintf("Q3 Automatic Test4: Failed. Evaluation = 50%\n");
		cprintf("[EV@LU@TI0N] 0.5\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	int x = 0;
	//range across multiple tables
	ptr = (char*) 0xF07FF000 ; *ptr = 'A';
	ptr = (char*) 0xF0805000 ; *ptr = 'B';
	ptr2 = (char*)0xF0906000 ; x += (int)(*ptr2);
	ptr2 = (char*)0xF0A00000 ; x += (int)(*ptr2);
	ptr2 = (char*)0xF0B00000 ; x += (int)(*ptr2); *ptr = x;
	ptr = (char*) 0xF0E00000 ; *ptr = 'C';
	ptr = (char*) 0xF0E03000 ; *ptr = 'D';
	ptr = (char*) 0xF0E04000 ; *ptr = 'E';
	ptr = (char*) 0xF0E05000 ; *ptr = 'X';

	char cr4[100] = "sm2nu 0xF07FF000 0xF0E05000";
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	cnt = SetModifiedPagesInRangeToNotUsed(args) ;

	if (cnt != 5|| CheckBit(0xF07FF000, 5) || CheckBit(0xF0805000, 5) || !CheckBit(0xF0906000, 5)
				|| !CheckBit(0xF0A00000, 5) || !CheckBit(0xF0B00000, 5) || CheckBit(0xF0E00000, 5)
				|| CheckBit(0xF0E03000, 5) || CheckBit(0xF0E04000, 5) || !CheckBit(0xF0E05000, 5))
	{
		cprintf("Q3 Automatic Test5: Failed. Evaluation = 75%\n");
		cprintf("[EV@LU@TI0N] 0.75\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}


	cprintf("Q3 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	return 0;
}

int TestAss3Q4()
{
	//Connect with write permission
	char cr1[100] = "cvp 0x0 0x00300000 w";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int ref1 = frames_info[0x00300000 / PAGE_SIZE].references;
	uint32 entry = ConnectVirtualToPhysical(args) ;
	char *ptr1, *ptr2, *ptr3;
	ptr1 = (char*)0x0; *ptr1 = 'A' ;
	int ref2 = frames_info[0x00300000 / PAGE_SIZE].references;

	if ((ref2 - ref1) != 0)
	{
		cprintf("Q4 Automatic Test1: Failed. You should manually implement the connection logic using paging data structures ONLY. [DON'T update the references]. Evaluation = 0\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	uint32 f = entry & 0xFFFFF000 ;
	if (*ptr1 != 'A' || (f != 0x00300000) || ((entry & PERM_WRITEABLE) == 0))
	{
		cprintf("Q4 Automatic Test2: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Connect with read permission on same pa
	char cr2[100] = "cvp 0x00004000 0x00300000 r";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	entry = ConnectVirtualToPhysical(args) ;
	ptr2 = (char*)0x00004000;

	int ref3 = frames_info[0x00300000 / PAGE_SIZE].references;

	if ((ref3 - ref2) != 0)
	{
		cprintf("Q4 Automatic Test3: Failed. You should manually implement the connection logic using paging data structures ONLY. [DON'T update the references]. Evaluation = 0.25\n");
		cprintf("[EV@LU@TI0N] 0.25\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	f = entry & 0xFFFFF000 ;
	if (*ptr1 != 'A' || *ptr2 != 'A' || (f != 0x00300000) || ((entry & PERM_WRITEABLE) != 0))
	{
		cprintf("Q4 Automatic Test4: Failed. Evaluation = 50%\n");
		cprintf("[EV@LU@TI0N] 0.5\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Connect with write permission on already connected page
	char cr3[100] = "cvp 0x00004000 0x00400000 w";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;
	int r1 = frames_info[0x00400000 / PAGE_SIZE].references;

	entry = ConnectVirtualToPhysical(args) ;

	ptr2 = (char*)0x00004000; *ptr2 = 'B';

	int ref4 = frames_info[0x00300000 / PAGE_SIZE].references;
	int r2 = frames_info[0x00400000 / PAGE_SIZE].references;

	if (*ptr1 != 'A' || *ptr2 != 'B' || (ref4 - ref1) != 0 || (r2 - r1) != 0)
	{
		cprintf("Q4 Automatic Test5: Failed [DON'T USE MAP_FRAME()! Implement the connection by yourself]. Evaluation = 50%\n");
		cprintf("[EV@LU@TI0N] 0.5\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	cprintf("Q4 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	return 0;
}

// BONUS
int TestAss3B1()
{
	cprintf("MAKE SURE TO RUN THIS TEST CASE SEPARATELY !!");
	//Case1: pa = 1 MB
	char cr1[100] = "fv 0x00100000";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	uint32 va = FindVirtualOfPhysical(args) ;
	int kilo = 1024 ;
	int mega = 1024*1024 ;
	if (va  != (KERNEL_BASE + 1*mega))
	{
		cprintf("B1 Automatic Test1: Failed. Evaluation = 0%\n");
		cprintf("[EV@LU@TI0N] 0\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}


	//Case2: pa = 1 MB after connecting va 8 MB to it
	map_frame(ptr_page_directory, &(frames_info[((1*mega)>>12)]), (void*)0x00800000, PERM_WRITEABLE);

	char cr2[100] = "fv 0x00100000";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	va = FindVirtualOfPhysical(args) ;
	if (va  != 8*mega)
	{
		cprintf("B1 Automatic Test2: Failed. Evaluation = 50%\n");
		cprintf("[EV@LU@TI0N] 0.5\n");
		cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Case3: pa = 1 MB after connecting va 0 MB to it
	map_frame(ptr_page_directory, &(frames_info[((1*mega)>>12)]), (void*)0x0, PERM_WRITEABLE);

	char cr3[100] = "fv 0x00100000";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	va = FindVirtualOfPhysical(args) ;
	if (va  != 0)
	{
		cprintf("B1 Automatic Test3: Failed. Evaluation = 75%\n");
				cprintf("[EV@LU@TI0N] 0.75\n");
				cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	cprintf("B1 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	return 0;
}

int TestAss3B2()
{
	//Case1: Source and desitnation exist:
	//====================================
	//Connect va = 0 with pa = 4 MB
	char cr1[100] = "cvp 0x0 0x00400000 w";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	uint32 entry = ConnectVirtualToPhysical(args) ;
	char *ptr1, *ptr2, *ptr3;
	ptr1 = (char*)0x0; 		*ptr1 = 'A' ;
	ptr2 = (char*)0x100; 	*ptr2 = 'B' ;
	ptr3 = (char*)0xfff; 	*ptr3 = 'C' ;

	//Connect va = 8 KB with pa = 5 MB
	char cr2[100] = "cvp 0x2000 0x00500000 w";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	entry = ConnectVirtualToPhysical(args) ;

	//Copy page
	char cr3[100] = "cp 0x0 0x2000";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int ret = CopyPage(args) ;

	//Read from dest page
	ptr1 = (char*)0x2000;
	ptr2 = (char*)0x2100;
	ptr3 = (char*)0x2fff;

	if (ret != 0 || *ptr1 != 'A' || *ptr2 != 'B' || *ptr3 != 'C')
	{
		cprintf("B2 Automatic Test1: Failed. Evaluation = 0%\n");
				cprintf("[EV@LU@TI0N] 0\n");
				cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	ptr1 = (char*)0x0; 		*ptr1 = 'X' ;
	ptr2 = (char*)0x2000;
	if (*ptr1 != 'X' || *ptr2 != 'A')
	{
		cprintf("B2 Automatic Test2: Failed. Evaluation = 25%\n");
				cprintf("[EV@LU@TI0N] 0.25\n");
				cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}


	//Case2: Source page doesn't exist:
	//====================================
	//Connect va = 8 KB with pa = 5 MB
	char cr4[100] = "cvp 0x2000 0x00500000 w";
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	entry = ConnectVirtualToPhysical(args) ;

	ptr1 = (char*)0x2000;	*ptr1 = 'A' ;

	//Copy page
	char cr5[100] = "cp 0x1000 0x2000";
	strsplit(cr5, WHITESPACE, args, &numOfArgs) ;

	ret = CopyPage(args) ;

	//Read from dest page
	ptr1 = (char*)0x2000;

	if (ret != -1 || *ptr1 != 'A' || CheckBit(0x1000, 0) )
	{
		cprintf("B2 Automatic Test3: Failed. Evaluation = 50%\n");
				cprintf("[EV@LU@TI0N] 0.5\n");
				cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	//Case3: Desination page doesn't exist:
	//====================================
	//Connect va = 0 KB with pa = 4 MB
	char cr6[100] = "cvp 0x0 0x00400000 w";
	strsplit(cr6, WHITESPACE, args, &numOfArgs) ;

	entry = ConnectVirtualToPhysical(args) ;

	ptr1 = (char*)0x0;	*ptr1 = 'A' ;
	ptr1 = (char*)0x100;*ptr1 = 'B' ;
	ptr1 = (char*)0xfff;*ptr1 = 'C' ;

	//Copy page
	char cr7[100] = "cp 0x0 0x1000";
	strsplit(cr7, WHITESPACE, args, &numOfArgs) ;

	ret = CopyPage(args) ;

	//Read from source page
	ptr1 = (char*)0x0;

	if (ret != -1 || *ptr1 != 'A' || CheckBit(0x1000, 0) )
	{
		cprintf("B2 Automatic Test4: Failed. Evaluation = 75%\n");
				cprintf("[EV@LU@TI0N] 0.75\n");
				cprintf("[EV@LU@TI0N] Finished\n");
		return 0;
	}

	cprintf("B2 Automatic Test: Succeeded. Evaluation = 100%\n");
	cprintf("[EV@LU@TI0N] 1\n");
	cprintf("[EV@LU@TI0N] Finished\n");

	return 0;
}

int CheckBit(uint32 va, int bitNum)
{
	uint32 *ptr_table = NULL;
	uint32 mask = 1<<bitNum;
	get_page_table(ptr_page_directory, (void*)va, 0, &ptr_table);
	return (ptr_table[PTX(va)] & mask) == mask ? 1 : 0 ;
}
