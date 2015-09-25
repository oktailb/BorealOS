#ifndef __CORE_H__
#define __CORE_H__

#include <defines.hh>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#ifndef __PIPE_H__
#endif

class Task
{
private:
	void str2arg(char* str, char** args, int nb_args);
	int get_nb_args(char* str);
	int ret;
	
	int local_std_in;
	int local_std_out;
	int local_std_err;
	
	char** binary_task;
	int (*func_task)(char*[]);
	char** args;
	jmp_buf envir;
	int cwait;
	int cbackground;
	int status;
	pid_t pid;

public:
	Task(char* cmd);
	Task(char * arg[]);
	Task(int (*func)(char*[]), char* argv[]);
	~Task(void);
	int get_ret();
	//jmp_buf get_envir(void);
	
	char** get_binary_task(void);
	int* get_func_task(void);
	void set_cwait(int val);
	void set_cbackground(int val);
	static void end_of_child(int);
	
	int reconnect2(int initial_fd, int new_fd);
	int run_func_task(int (*func)(char*[]), char* argv[]);
	int run_binary_task(char * arg[]);
	int run(void);
};

//Task* core_dump_generator;

#endif
