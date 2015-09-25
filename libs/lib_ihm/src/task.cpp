#include <task.hh>

//jmp_buf Task::envir;

Task::Task(char* cmd)
{
    binary_task = (char**)malloc(sizeof(char*)*get_nb_args(cmd));
    str2arg(cmd,binary_task,get_nb_args(cmd));
    func_task = 0;
    setjmp(envir);
}

/*
 *
**	Cree un fil a partir d'une fonction a executer
*
*/

Task::Task(int (*func)(char*[]), char** argv)
{
    local_std_in = STD_IN;
    local_std_out = STD_OUT;
    local_std_err = STD_ERR;

    func_task = func;
    binary_task = 0;
    args = argv;
    setjmp(envir);
}


/*
 *
**	Version tres simplifie de execvp
*
*/

Task::Task(char** arg)
{
    local_std_in = STD_IN;
    local_std_out = STD_OUT;
    local_std_err = STD_ERR;
    
    binary_task = arg;
    func_task = 0;
    setjmp(envir);
}

Task::~Task()
{
	longjmp(envir,1);
}

/*
 *
**	donne ne nombre d'arguments d'un ligne 
**	de commandes sous forme de chaine
*
*/

int Task::get_nb_args(char* str)
{
    int nb_args = 0;
    int index = 0;
    int len = 0;
    
    len=strlen(str);

    for (index=0; index<len; index++)
    {
	if(str[index] == ' ')
	{
	    if(!(str[index+1] == ' '))nb_args++;
	}
    }
    nb_args++;

    return nb_args;
}

/*
 *
** convertit une ligne de commande brute en tableau d'arguments
*
*/

void Task::str2arg(char* str, char** args, int nb_args)
{
    int index = 0;
    int len = 0;
    char* cp;
    
    len = strlen(str);
    cp = (char*) malloc(1+sizeof(char)*len);
    strcpy(cp,str);
    for(index = 0 ; index < len ; index++)
    {
	if(cp[index] == ' ')cp[index]='\0';
    }
    for(index = 0 ; index < nb_args ; index++)
    {
	args[index] = cp;
	cp += (1+strlen(args[index]));
    }
}

int Task::get_ret()
{
	return ret;
}

char** Task::get_binary_task(void)
{
	return binary_task;
}

int* Task::get_func_task(void)
{
    return (int*)func_task;
}

extern "C"
{
    void Task::end_of_child(int)
    {
	wait(NULL);
        signal(SIGCHLD, SIG_DFL);
    }
};

int Task::run(void)
{
    pid=fork();
    if (pid<0)
    {
        perror("fork:");
        exit(-1);
    }
    if(pid==0)
    {
        if (local_std_in  != STD_IN)  reconnect2(local_std_in , STD_IN);
        if (local_std_out != STD_OUT) reconnect2(local_std_out, STD_OUT);
        if (local_std_err != STD_ERR) reconnect2(local_std_err, STD_ERR);
        if(func_task)
	    return func_task(args);
	else
	{
	    if((ret = execvp(binary_task[0],binary_task))<0)
	    {
		perror("execvp:");
		exit(-1);
	    }
	    return ret;
	}
    }
    else
    {
        if(!cbackground)
	    return wait(&status);
	else
	{
	    signal(SIGCHLD,end_of_child);
	    return 0;
	}
    }
}

/*
 *
** Redirection simple de flux
*
*/

int Task::reconnect2(int initial_fd, int new_fd)
{
    if(close(new_fd)<0){perror("close:");return -1;}
    
    if(dup(initial_fd)<0){perror("dup:");return -1;}
    if(close(initial_fd)<0){perror("close:");return -1;}

    return 1;    
}

void Task::set_cwait(int val)
{
    cwait = val;
}

void Task::set_cbackground(int val)
{
    cbackground = val;
}
