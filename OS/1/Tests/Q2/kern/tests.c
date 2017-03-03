#include <kern/tests.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

void TestAssignment1()
{
	//TestAss1Q1();
	TestAss1Q2();
	//TestAss1Q3();
}

int TestAss1Q1()
{
	cprintf("Q1 Test: manually try the test cases in the doc. \n..."
			"OR, you can do it automatically by uncommenting the code below, it should print the description of add, help, is_pal commands and execute fact 5 command\n");

	int retValue = 0;
	int i = 0;

	//CASE1: should print ONLY the description of "add" command
	char cr1[100] = "add ?";
	execute_command(cr1) ;

	//CASE2: should print ONLY the description of "help" command
	char cr2[100] = "help ?";
	execute_command(cr2) ;

	//CASE3: should print ONLY the description of "ip" command
	char cr3[100] = "is_pal ?";
	execute_command(cr3) ;

	//CASE4: should execute fact 5 and print 120
	char cr4[100] = "fact 5";
	execute_command(cr4) ;

	return 0;
}


int TestAss1Q2()
{
	int retValue = 0;
	int i = 0;
	//CASE1: palindrome word
	char cr1[100] = "is_pal acaramanamaraca";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	uint32 ret = CheckIsPalindrome(args) ;
	if (ret != 1)
	{
		cprintf("[EVAL#] CheckIsPalindrome: Failed\n");
		return retValue;
	}

	//CASE2: not palindrome
	char cr2[100] = "is_pal amanaplancanalpanama";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	ret = CheckIsPalindrome(args) ;
	if (ret != 0)
	{
		cprintf("[EVAL#] CheckIsPalindrome: Failed\n");
		return retValue;
	}

	//CASE3: palindrome
	char cr3[100] = "is_pal aa";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	ret = CheckIsPalindrome(args) ;
	if (ret != 1)
	{
		cprintf("[EVAL#] CheckIsPalindrome: Failed\n");
		return retValue;
	}

	cprintf("[EVAL#] CheckIsPalindrome: Succeeded\n");

	return 0;
}
int TestAss1Q3()
{
	int retValue = 0;
	int i = 0;

	//CASE1: invalid number of args
	char cr1[100] = "ver 1";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;
	uint32 ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 0)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	//CASE2: invalid number of args
	char cr2[100] = "add 12";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;
	ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 0)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	//CASE3: invalid number of args
	char cr3[100] = "fact";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;
	ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 0)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	//CASE4: invalid number of args
	char cr4[100] = "add 12 13 25";
	numOfArgs = 0;
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;
	ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 0)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	//CASE5: valid number of args
	char cr5[100] = "add 12 13";
	numOfArgs = 0;
	strsplit(cr5, WHITESPACE, args, &numOfArgs) ;
	ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 1)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	//CASE6: valid number of args
	char cr6[100] = "help";
	numOfArgs = 0;
	strsplit(cr6, WHITESPACE, args, &numOfArgs) ;
	ret = CheckNumberOfArgs(args, numOfArgs) ;
	if (ret != 1)
	{
		cprintf("[EVAL#] CheckNumberOfArgs: Failed\n");
		return retValue;
	}

	cprintf("[EVAL#] CheckNumberOfArgs: Succeeded\n");

	return 0;
}