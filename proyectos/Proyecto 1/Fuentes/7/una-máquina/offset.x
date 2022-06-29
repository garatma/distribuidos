const ERROR = -1;
const TIEMPO = 0;

union salida switch(int tipo) {
	case TIEMPO:
		long int tiempo;
	default:
		void;
};

program OFFSET_PROGRAMA {
	version OFFSET_VERSION {
		struct salida offset() = 1;
	} = 1;
} = 0x20000000;
