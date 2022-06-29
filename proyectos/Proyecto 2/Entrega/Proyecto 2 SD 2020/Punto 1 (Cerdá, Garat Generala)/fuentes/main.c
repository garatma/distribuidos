#include <stdlib.h>
#include "gui.h"
#include "logica.h"

int main(int argc, char *argv[])
{
    inicializar_logica(&argc, &argv);
    inicializar_gui(&argc, &argv);
    return EXIT_SUCCESS;
}
