struct nums
{
	int a;
	int b;
};

program ADD_PROGRAM
{
	version ADD_VERSION
	{
		int add(nums) = 1;
	} = 1;
} = 0x20000000;

program MINUS_PROGRAM
{
	version MINUS_VERSION
	{
		int minus(nums) = 1;
	} = 1;
} = 0x20000001;


