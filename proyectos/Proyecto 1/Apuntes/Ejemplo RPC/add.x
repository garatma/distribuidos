
struct numbers {
	int a;
	int b;
};

program ADD_PROGRAM {
	version ADD_VERSION {
		int add(numbers)=1;
	}=1;
} = 0x20000000;
