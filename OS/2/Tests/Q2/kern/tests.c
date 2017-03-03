#include <kern/tests.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

void TestAssignment2()
{
	//TestAss2Q1();
	TestAss2Q2();
	//TestAss2Q3();
	//TestAss2Q4();
	//TestAss2BONUS();
}

int TestAss2Q1()
{
	//Clear mem locations
	char* ptr = (char*)(0x200000 + KERNEL_BASE);
	memset(ptr, 0, 100);

	//Write single word
	char wdc1[100] = "ws 200000 FCIS";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(wdc1, WHITESPACE, args, &numOfArgs) ;
	WriteString(args, numOfArgs) ;

	if (strcmp(ptr, "FCIS") != 0)
	{
		cprintf("written chars = %s\n", ptr);
		cprintf("[EVAL] #1 WriteString: Failed\n");
		return 0;
	}

	//Clear mem locations
	ptr = (char*)(0x400000 + KERNEL_BASE);
	memset(ptr, 0, 100);

	//Write multi words
	char wdc2[100] = "ws 400000 FCIS OS'16";
	strsplit(wdc2, WHITESPACE, args, &numOfArgs) ;
	WriteString(args, numOfArgs) ;

	if (strcmp(ptr, "FCIS OS'16") != 0)
	{
		cprintf("written chars = %s\n", ptr);
		cprintf("[EVAL] #2 WriteString: Failed\n");
		return 0;
	}

	//Clear mem locations
	ptr = (char*)(0x300000 + KERNEL_BASE);
	memset(ptr, 0, 100);

	//Write multi words
	char wdc3[100] = "ws 300000 Ain Shams Univ. - FCIS";
	strsplit(wdc3, WHITESPACE, args, &numOfArgs) ;
	WriteString(args, numOfArgs) ;

	if (strcmp(ptr, "Ain Shams Univ. - FCIS") != 0)
	{
		cprintf("written chars = %s\n", ptr);
		cprintf("[EVAL] #3 WriteString: Failed\n");
		return 0;
	}

	cprintf("[EVAL2] WriteString: Succeeded\n");
	return 1;
}

int TestAss2Q2()
{
	int retValue = 1;
	int i = 0;
	//Create first array
	char cr1[100] = "cnia x1 3 10 20 30";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateIntArray(args) ;

	//Check elements of 1st array
	for (i=0 ; i<3; i++)
	{
		if (ptr1[i] != (i+1)*10)
		{
			cprintf("[EVAL] #1 CreateIntArray: Failed\n");
			return retValue;
		}
	}

	//Create second array
	char cr2[100] = "cnia y1 4 1 2 3 4";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateIntArray(args) ;
	//Check elements of 2nd array
	for (i=0 ; i<4; i++)
	{
		if (ptr2[i] != (i+1))
		{
			cprintf("[EVAL] #2 CreateIntArray: Failed\n");
			return retValue;
		}
	}
	//Check elements of 1st array
	for (i=0 ; i<3; i++)
	{
		if (ptr1[i] != (i+1)*10)
		{
			cprintf("[EVAL] #3 CreateIntArray: Failed\n");
			return retValue;
		}
	}

	//Create third array
	char cr3[100] = "cnia z1 1 100";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateIntArray(args) ;
	//Check elements of 3rd array
	for (i=0 ; i<1; i++)
	{
		if (ptr3[i] != (i+1)*100)
		{
			cprintf("[EVAL] #4 CreateIntArray: Failed\n");
			return retValue;
		}
	}
	//Check elements of 2nd array
	for (i=0 ; i<4; i++)
	{
		if (ptr2[i] != (i+1))
		{
			cprintf("[EVAL] #5 CreateIntArray: Failed\n");
			return retValue;
		}
	}
	//Check elements of 1st array
	for (i=0 ; i<3; i++)
	{
		if (ptr1[i] != (i+1)*10)
		{
			cprintf("[EVAL] #6 CreateIntArray: Failed\n");
			return retValue;
		}
	}

	cprintf("[EVAL2] CreateIntArray: Succeeded\n");

	return retValue;
}

int TestAss2Q3()
{
	//Create first array
	char cr1[100] = "cnia x2 3 10 20 30";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	CreateIntArray(args) ;

	//Get (Exist)
	char f1[100] = "gea x2 2";
	strsplit(f1, WHITESPACE, args, &numOfArgs) ;
	int ret = GetElementFromArray(args) ;
	if (ret != 30)
	{
		cprintf("[EVAL] #1 GetElementFromArray: Failed\n");
		return 1;
	}
	//Get (Not Exist)
	strcpy(f1, "gea x2 3");
	strsplit(f1, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #2 GetElementFromArray: Failed\n");
		return 1;
	}

	//Create second array
	char cr2[100] = "cnia y2 4 4 3 2 1";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	CreateIntArray(args) ;

	//Get (Exist)
	char f2[100] = "gea y2 0";
	strsplit(f2, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 4)
	{
		cprintf("[EVAL] #3 GetElementFromArray: Failed\n");
		return 1;
	}
	//Get (Not Exist)
	strcpy(f2, "gea y2 6");
	strsplit(f2, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #4 GetElementFromArray: Failed\n");
		return 1;
	}

	//Create third array
	char cr3[100] = "cnia z2 1 100";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	CreateIntArray(args) ;

	//Get (Exist)
	char f3[100] = "gea z2 0";
	strsplit(f3, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 100)
	{
		cprintf("[EVAL] #5 GetElementFromArray: Failed\n");
		return 1;
	}
	//Get (Not Exist)
	strcpy(f3, "gea z2 1");
	strsplit(f3, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #6 GetElementFromArray: Failed\n");
		return 1;
	}

	//Get from the three arrays
	char f4[100] = "gea x2 1";
	strsplit(f4, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 20)
	{
		cprintf("[EVAL] #7 GetElementFromArray: Failed\n");
		return 1;
	}

	char f5[100] = "gea y2 2";
	strsplit(f5, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 2)
	{
		cprintf("[EVAL] #8 GetElementFromArray: Failed\n");
		return 1;
	}
	char f6[100] = "gea y2 4";
	strsplit(f6, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #9 GetElementFromArray: Failed\n");
		return 1;
	}

	cprintf("[EVAL2] GetElementFromArray: Succeeded\n");

	return 1;
}


int TestAss2Q4()
{
	int retValue = 1;
	int i = 0;
	//Create first array
	char cr1[100] = "cnia arr1 3 30 20 10";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateIntArray(args) ;

	//Sort the array in ascending order
	char sa1[100] = "sa arr1 a";
	strsplit(sa1, WHITESPACE, args, &numOfArgs) ;

	SortTheArray(args) ;

	//Check elements of 1st array
	for (i=0 ; i<2; i++)
	{
		if (ptr1[i] > ptr1[i+1])
		{
			cprintf("[EVAL] #1 SortTheArray: Failed\n");
			return retValue;
		}
	}

	//Create second array
	char cr2[100] = "cnia arr2 4 1 3 2 4";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateIntArray(args) ;

	//Sort the array in descending order
	char sa2[100] = "sa arr2 d";
	strsplit(sa2, WHITESPACE, args, &numOfArgs) ;

	SortTheArray(args) ;

	//Check elements of 2nd array
	for (i=0 ; i<3; i++)
	{
		if (ptr2[i] < ptr2[i+1])
		{
			cprintf("[EVAL] #2 SortTheArray: Failed\n");
			return retValue;
		}
	}
	//Check elements of 1st array
	for (i=0 ; i<2; i++)
	{
		if (ptr1[i] > ptr1[i+1])
		{
			cprintf("[EVAL] #3 SortTheArray: Failed\n");
			return retValue;
		}
	}

	//Create third array
	char cr3[100] = "cnia arr3 2 100 100";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateIntArray(args) ;

	//Sort the 3rd array in descending order
	char sa3[100] = "sa arr3 d";
	strsplit(sa3, WHITESPACE, args, &numOfArgs) ;

	SortTheArray(args) ;

	//Check elements of 3rd array
	for (i=0 ; i<1; i++)
	{
		if (ptr3[i] != ptr3[i+1])
		{
			cprintf("[EVAL] #4 SortTheArray: Failed\n");
			return retValue;
		}
	}

	//Sort the 1st array in descending order
	char sa4[100] = "sa arr1 d";
	strsplit(sa4, WHITESPACE, args, &numOfArgs) ;

	SortTheArray(args) ;

	//Check elements of 1st array
	for (i=0 ; i<2; i++)
	{
		if (ptr1[i] < ptr1[i+1])
		{
			cprintf("[EVAL] #5 SortTheArray: Failed\n");
			return retValue;
		}
	}

	//Check elements of 2nd array
	for (i=0 ; i<3; i++)
	{
		if (ptr2[i] < ptr2[i+1])
		{
			cprintf("[EVAL] #6 SortTheArray: Failed\n");
			return retValue;
		}
	}

	cprintf("[EVAL2] SortTheArray: Succeeded\n");


	return 1;
}

int TestAss2BONUS()
{
	int retValue = 1;
	int i = 0;
	/*CASE1: Create 1 array then delete it*/
	//Create first array
	char cr1[100] = "cnia  x3 1 2 3";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateIntArray(args) ;

	//Delete it
	char dl1[100] = "dnia x3";
	strsplit(dl1, WHITESPACE, args, &numOfArgs) ;
	DeleteIntArray(args);

	//Create another array (should overwrite the 1st one!
	char cr2[100] = "cnia y3 3 10 20 30";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateIntArray(args) ;

	//Access 1st element of 1st array (it should retrieve the one of the 2nd array)
	if (ptr1[0] != 10 || ptr1 != ptr2)
	{
		cprintf("[EVAL] #1 DeleteIntArray: Failed\n");
		return retValue;
	}

	//Get element from 1st array and from 2nd array
	char f1[100] = "gea x3 1";
	strsplit(f1, WHITESPACE, args, &numOfArgs) ;
	int ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #2 DeleteIntArray: Failed\n");
		return 1;
	}
	char f2[100] = "gea y3 1";
	strsplit(f2, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 20)
	{
		cprintf("[EVAL] #3 DeleteIntArray: Failed\n");
		return 1;
	}


	/*CASE2: Create three arrays then delete one of them*/
	//Create second array
	char cr3[100] = "cnia z3 3 100 200 300";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateIntArray(args) ;

	//Create third array
	char cr4[100] = "cnia x4 1 1000";
	numOfArgs = 0;
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	int* ptr4 = CreateIntArray(args) ;

	//Delete the 1st array
	char dl2[100] = "dnia y3";
	strsplit(dl2, WHITESPACE, args, &numOfArgs) ;
	DeleteIntArray(args);

	//Access 1st element of 1st and 2nd arrays (it should retrieve the one of the 2nd and 3rd arrays)
	if (ptr2[0] != 100 || ptr3[0] != 1000)
	{
		cprintf("[EVAL] #4 DeleteIntArray: Failed\n");
		return retValue;
	}

	//Get element from 1st (deleted), 2nd and 3rd arrays
	char f3[100] = "gea y3 1";
	strsplit(f3, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #5 DeleteIntArray: Failed\n");
		return 1;
	}
	char f4[100] = "gea z3 1";
	strsplit(f4, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 200)
	{
		cprintf("[EVAL] #6 DeleteIntArray: Failed\n");
		return 1;
	}
	char f5[100] = "gea z3 3";
	strsplit(f5, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #7 DeleteIntArray: Failed\n");
		return 1;
	}
	char f6[100] = "gea x4 0";
	strsplit(f6, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != 1000)
	{
		cprintf("[EVAL] #8 DeleteIntArray: Failed\n");
		return 1;
	}
	char f7[100] = "gea x4 1";
	strsplit(f7, WHITESPACE, args, &numOfArgs) ;
	ret = GetElementFromArray(args) ;
	if (ret != -1)
	{
		cprintf("[EVAL] #9 DeleteIntArray: Failed\n");
		return 1;
	}
	cprintf("[EVAL2] [BONUS] DeleteIntArray: Succeeded\n");

	return retValue;
}

