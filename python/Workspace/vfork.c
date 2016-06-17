#include <sys/types.h>

static int glob = 6;

int main(void){
	int var;
	pid_t pid;
	var = 88;
	printf("befor vfork \n");

	if((pid = vfork()) < 0)
		err_sys("vfork error");
	else if (pid == 0){
		glob++;
		var++;
		_exit(0);
	}
	printf("pid = %d, glob = %d, var %d\n",getpid(),glob,var);
	return 0;
}
