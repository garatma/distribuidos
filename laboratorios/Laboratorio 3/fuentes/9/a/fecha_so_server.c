#include "fecha_so.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/utsname.h>

char ** so_1_svc(void *argp, struct svc_req *rqstp) {
	static char * result;
	struct utsname uts;

    result = (char *) malloc(sizeof(char)*SIZE_CADENA);

	printf("Un cliente quiere saber el sistema operativo de esta máquina.\n");

	if (uname(&uts) == -1) {
        perror("uname() error");
        result[0] = '\0';
    }
    else strcpy(result, uts.sysname);

	return &result;
}

char ** fecha_hora_1_svc(void *argp, struct svc_req *rqstp) {
	static char * result;
    result = (char *) malloc(sizeof(char)*SIZE_CADENA);

	printf("Un cliente quiere saber la hora local en esta máquina.\n");

	time_t tiempo_local;
	struct tm * tm_info;

	tiempo_local = time(NULL);
	tm_info = localtime(&tiempo_local);

	strftime(result, SIZE_CADENA-1, "%a %b %d %H:%M:%S", tm_info);

	return &result;
}
