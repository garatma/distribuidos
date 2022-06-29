struct argumento {
	char arr [2048];
};

program SUMA_ARREGLOS_PROGRAM {
	version SUMA_ARREGLOS_VERSION {
		argumento suma_arreglos(argumento) = 1;
	} = 1;
} = 0x20000000;
