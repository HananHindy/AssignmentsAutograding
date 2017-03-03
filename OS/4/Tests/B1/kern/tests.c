#include <kern/tests.h>
#include <kern/memory_manager.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

extern int execute_command(char *command_string);
extern uint32 CalculateRequiredSpace(char** arguments);
extern void CopyUserMemory(char** arguments);
extern void DeleteUserStack(char** arguments);

void TestAssignment4(uint32 cntOfRuns)
{
	if (cntOfRuns == 1)
	{
		cprintf("\n=====================================================\n");
		cprintf("[CAUTION] MAKE SURE TO TEST EACH QUESTION SEPARATELY.\n(Comment all tests except the one you need to check!)");
		cprintf("\n=====================================================\n");

	}
	//TestAss4Q1(cntOfRuns);
//	TestAss4Q2();
//	TestAss4Q3();
	TestAss4QB(cntOfRuns);
}

int TestAss4Q1(uint32 cntOfRuns)
{
	cprintf("[EV@LU@TI0N] 0\n");
	
	if (cntOfRuns == 1 && calculate_free_frames() < (212 + 2013 + 4015))
	{
		panic("\n insufficient frames to run the three test programs consecutively.\nTry to run each program separately\n");
		while(1==1){}
	}
	if (cntOfRuns == 1)
	{
		char cr1[100] = "run tst1 200";
		execute_command(cr1);
	}
	if (cntOfRuns == 2)
	{
		char cr2[100] = "run tst2 2000";
		execute_command(cr2);
	}
	if (cntOfRuns == 3)
	{
		char cr3[100] = "run tst3 4000";
		execute_command(cr3);
	}
	
	cprintf("[EV@LU@TI0N] 100\n");
	cprintf("[EV@LU@TI0N] Finished\n");
	cprintf("Q1 Automatic Test: FINISHED. Evaluation = 100%\n");

	return 0;
}


int TestAss4Q2()
{
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	uint32 res =0;
	uint32 totalScorePercent = 0;
	/*=======================================*/
	/*Ex1: ALL pages and tables are not exist*/
	/*=======================================*/
	{
		//Test1
		char cr1[100] = "crs 0x0 8 K";
		strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 3)
		{
			cprintf("Q2 Automatic Test1: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}

		//Test2
		char cr2[100] = "crs 0x0 4 M";
		strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 1025)
		{
			cprintf("Q2 Automatic Test2: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
		//Test3
		char cr3[100] = "crs 0x0 8 M";
		strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 2050)
		{
			cprintf("Q2 Automatic Test3: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
	}

	{
		//Test4
		char cr4[100] = "crs 0x1000 6 K";
		strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 3)
		{
			cprintf("Q2 Automatic Test4: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}

		//Test5
		char cr5[100] = "crs 0x1800 3 K";
		strsplit(cr5, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 3)
		{
			cprintf("Q2 Automatic Test5: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}

		//Test6
		char cr6[100] = "crs 0x400000 10 M";
		strsplit(cr6, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 2563)
		{
			cprintf("Q2 Automatic Test6: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
	}

	{
		//Test7
		char cr7[100] = "crs 0x700000 2 M";
		strsplit(cr7, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 514)
		{
			cprintf("Q2 Automatic Test7: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}

		//Test8
		char cr8[100] = "crs 0x3FFFFF 1 K";
		strsplit(cr8, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 4)
		{
			cprintf("Q2 Automatic Test8: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}
	}

	/*=======================================*/
	/*Ex2: SOME pages and/or tables are exist*/
	/*=======================================*/
	{
		//Test1
		char c1[100] = "ap 0x0";
		execute_command(c1);

		char c2[100] = "crs 0x0 8 K";
		strsplit(c2, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 1)
		{
			cprintf("Q2 Automatic Test9: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}

		char c3[100] = "ap 0x100000";
		execute_command(c3);

		//Test2
		char c4[100] = "crs 0x0 4 M";
		strsplit(c4, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 1022)
		{
			cprintf("Q2 Automatic Test10: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
		//Test3
		char c5[100] = "crs 0x0 8 M";
		strsplit(c5, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 2047)
		{
			cprintf("Q2 Automatic Test11: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
	}

	{
		char c6[100] = "ap 0x2000";
		execute_command(c6);

		//Test4
		char c7[100] = "crs 0x1800 3 K";
		strsplit(c7, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 1)
		{
			cprintf("Q2 Automatic Test12: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}

		char c8[100] = "ap 0x800000";
		execute_command(c8);

		//Test5
		char c9[100] = "crs 0x400000 10 M";
		strsplit(c9, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 2561)
		{
			cprintf("Q2 Automatic Test13: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}
	}

	{
		char c10[100] = "ap 0x801000";
		execute_command(c10);

		char c11[100] = "ap 0x810000";
		execute_command(c11);

		//Test7
		char c12[100] = "crs 0x700000 2 M";
		strsplit(c12, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 510)
		{
			cprintf("Q2 Automatic Test14: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}

		//Test8
		char c13[100] = "crs 0x3FFFFF 1 K";
		strsplit(c13, WHITESPACE, args, &numOfArgs) ;

		res = CalculateRequiredSpace(args) ;
		if (res  != 3)
		{
			cprintf("Q2 Automatic Test15: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}
	}

	cprintf("[EV@LU@TI0N] %d\n", totalScorePercent);
	cprintf("[EV@LU@TI0N] Finished\n");
	
	cprintf("Q2 Automatic Test: FINISHED. Evaluation = %d%\n", totalScorePercent);

	return 0;
}

int TestAss4Q3()
{
	cprintf("[EV@LU@TI0N] 0\n");

	//cprintf("Q3 Automatic Test: To be prepared...\n");
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	uint32 res =0;
	uint32 totalScorePercent = 0;
	uint32 numOfFreeFramesBefore, numOfFreeFramesAfter ;
	char *ch1, *ch2, *ch3, *ch4, *ch5, *ch6, *ch7 ;
	/*=======================================*/
	/*Ex1: Destination page(s) exist		 */
	/*=======================================*/
	{
		char c1[100] = "ap 0x0";
		execute_command(c1);
		char c2[100] = "wum 0x000000 a";
		execute_command(c2);
		char c3[100] = "wum 0x0007FF b";
		execute_command(c3);
		char c4[100] = "wum 0x000FFF c";
		execute_command(c4);
		char c5[100] = "ap 0x400000";
		execute_command(c5);
		char c6[100] = "wum 0x400000 x";
		execute_command(c6);
		char c7[100] = "wum 0x4007FF y";
		execute_command(c7);
		char c8[100] = "wum 0x400FFF z";
		execute_command(c8);

		//Test1
		numOfFreeFramesBefore = calculate_free_frames();

		char cr1[100] = "cum 0x0 0x400000 4 K";
		strsplit(cr1, WHITESPACE, args, &numOfArgs) ;
		CopyUserMemory(args) ;

		numOfFreeFramesAfter = calculate_free_frames();

		if (numOfFreeFramesBefore != numOfFreeFramesAfter)
		{
			cprintf("Q3 Automatic Test1: FINISHED. Evaluation = 0%\n");
			cprintf("[EV@LU@TI0N] Finished\n");
			return 0;
		}

		ch1 = (char*)0x000000; ch2 = (char*)0x400000;
		char c9[100] = "wum 0x0007FF y";
		execute_command(c9);
		ch3 = (char*)0x0007FF; ch4 = (char*)0x4007FF;
		char c10[100] = "wum 0x400FFF z";
		execute_command(c10);
		ch5 = (char*)0x000FFF; ch6 = (char*)0x400FFF;

		if (*ch1 != 'a' || *ch2 != 'a' || *ch3 != 'y' || *ch4 != 'b' || *ch5 != 'c' || *ch6 != 'z')
		{
			cprintf("Q3 Automatic Test2: FINISHED. Evaluation = 0%\n");
			cprintf("[EV@LU@TI0N] Finished\n");
			return 0;
		}

		totalScorePercent += 25 ;

		//Test2
		char cr2[100] = "cum 0x400000 0x0 2 K";
		strsplit(cr2, WHITESPACE, args, &numOfArgs) ;
		CopyUserMemory(args) ;

		numOfFreeFramesAfter = calculate_free_frames();
		if (numOfFreeFramesBefore != numOfFreeFramesAfter)
		{
			cprintf("Q3 Automatic Test3: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 5 ;
		}

		ch1 = (char*)0x000000; ch2 = (char*)0x0007FF; ch3 = (char*)0x000FFF;
		if (*ch1 != 'a' || *ch2 != 'b' || *ch3 != 'c')
		{
			cprintf("Q3 Automatic Test4: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}
	}

	/*=======================================*/
	/*Ex2: Destination page(s) doesn't exist */
	/*=======================================*/
	{
		char c11[100] = "ap 0x800000";
		execute_command(c11);
		char c12[100] = "ap 0x801000";
		execute_command(c12);
		char c13[100] = "ap 0x802000";
		execute_command(c13);
		char c14[100] = "wum 0x800000 a";
		execute_command(c14);
		char c15[100] = "wum 0x8017FF b";
		execute_command(c15);
		char c16[100] = "wum 0x802FFF c";
		execute_command(c16);

		//Test3
		numOfFreeFramesBefore = calculate_free_frames();

		char cr3[100] = "cum 0x800000 0x900000 12 K";
		strsplit(cr3, WHITESPACE, args, &numOfArgs) ;
		CopyUserMemory(args) ;

		numOfFreeFramesAfter = calculate_free_frames();

		if ((numOfFreeFramesBefore - numOfFreeFramesAfter) != 3)
		{
			cprintf("Q3 Automatic Test5: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 20 ;
		}

		ch1 = (char*)0x800000; ch2 = (char*)0x900000;
		char c17[100] = "wum 0x8017FF y";
		execute_command(c17);
		ch3 = (char*)0x8017FF; ch4 = (char*)0x9017FF;
		char c18[100] = "wum 0x902FFF z";
		execute_command(c18);
		ch5 = (char*)0x802FFF; ch6 = (char*)0x902FFF;

		if (*ch1 != 'a' || *ch2 != 'a' || *ch3 != 'y' || *ch4 != 'b' || *ch5 != 'c' || *ch6 != 'z')
		{
			cprintf("Q3 Automatic Test6: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}

		//Test4
		numOfFreeFramesBefore = calculate_free_frames();

		char cr4[100] = "cum 0x901000 0xBFF000 8 K";
		strsplit(cr4, WHITESPACE, args, &numOfArgs) ;
		CopyUserMemory(args) ;

		numOfFreeFramesAfter = calculate_free_frames();
		if ((numOfFreeFramesBefore - numOfFreeFramesAfter) != 3)
		{
			cprintf("Q3 Automatic Test7: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 20 ;
		}

		ch1 = (char*)0x9017FF; ch2 = (char*)0xBFF7FF; ch3 = (char*)0x902FFF;ch4 = (char*)0xC00FFF;
		if (*ch1 != 'b' || *ch2 != 'b' || *ch3 != 'z' || *ch4 != 'z')
		{
			cprintf("Q3 Automatic Test8: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercent += 10 ;
		}
	}
	
	cprintf("[EV@LU@TI0N] %d\n", totalScorePercent);
	cprintf("[EV@LU@TI0N] Finished\n");;

		
	cprintf("Q3 Automatic Test: FINISHED. Evaluation = %d%\n", totalScorePercent);

	return 0;
}

static uint32 totalScorePercentBonus = 0;
int TestAss4QB(uint32 cntOfRuns)
{
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	uint32 res =0;
	uint32 numOfFreeFramesBefore, numOfFreeFramesAfter ;

	if (cntOfRuns == 1)
	{
		char cr1[100] = "run tst1 200";
		execute_command(cr1);
	}
	else if (cntOfRuns == 2)
	{
		//TEST1
		numOfFreeFramesBefore = calculate_free_frames();

		char cr2[100] = "dus tst1";
		strsplit(cr2, WHITESPACE, args, &numOfArgs) ;
		DeleteUserStack(args) ;

		numOfFreeFramesAfter = calculate_free_frames();

		if ((numOfFreeFramesAfter - numOfFreeFramesBefore) != 201)
		{
			cprintf("QB Automatic Test1: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercentBonus += 30 ;
		}

		char cr3[100] = "run tst2 2000";
		execute_command(cr3);
	}
	else if (cntOfRuns == 3)
	{
		//TEST2
		numOfFreeFramesBefore = calculate_free_frames();

		char cr4[100] = "dus tst2";
		strsplit(cr4, WHITESPACE, args, &numOfArgs) ;
		DeleteUserStack(args) ;

		numOfFreeFramesAfter = calculate_free_frames();

		if ((numOfFreeFramesAfter - numOfFreeFramesBefore) != 2002)
		{
			cprintf("QB Automatic Test2: Failed. diff = %d\n", numOfFreeFramesAfter - numOfFreeFramesBefore);
			//return 0;
		}
		else
		{
			totalScorePercentBonus += 30 ;
		}

		char cr5[100] = "run tst3 4000";
		execute_command(cr5);
	}
	else if (cntOfRuns == 4)
	{
		//TEST3
		numOfFreeFramesBefore = calculate_free_frames();

		char cr6[100] = "dus tst3";
		strsplit(cr6, WHITESPACE, args, &numOfArgs) ;
		DeleteUserStack(args) ;

		numOfFreeFramesAfter = calculate_free_frames();

		if ((numOfFreeFramesAfter - numOfFreeFramesBefore) != 4004)
		{
			cprintf("QB Automatic Test3: Failed.\n");
			//return 0;
		}
		else
		{
			totalScorePercentBonus += 40 ;
		}

		cprintf("[EV@LU@TI0N] %d\n", totalScorePercentBonus);
		cprintf("[EV@LU@TI0N] Finished\n");
	
		cprintf("QB Automatic Test: FINISHED. Evaluation = %d%\n", totalScorePercentBonus);
	}

	return 0;
}

