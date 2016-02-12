#include <iostream>
#include <unistd.h>
main(){
	int pid = fork();
	std::cout << "PID is "<<pid<<std::endl;
	if(pid == 0){

	}else{
		int status;
		wait($status);
	}
}