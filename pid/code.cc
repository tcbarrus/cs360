#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
main(){
	int pid = fork();
	std::cout << "PID is "<<pid<<std::endl;
	if(pid == 0){
		std::cout<< "about to exec"<<std::endl;
		execl("/bin/ls","/bin/ls",(char*)0);
	}else{
		int status;
		wait(&status);
		std::cout <<"after wait "<<std::endl;
	}
	exit(4);
}
