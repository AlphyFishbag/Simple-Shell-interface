//Commands to look for:
// getcwd() - obtains the current directory
// getlogin() - get the username 
// gethostname - get the name of the host running the program
// fork() and execvp() to implement the ability for the user to execute commands
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>


const  char* get_info(){

// This function gets all the info required to print out the current directory, username, and hostname
    char *cwd = getcwd(NULL, 1024);
    char *username = getlogin();
    char hostname[1024];
    char hostname_final = gethostname(hostname, 1024);
    char *temp;
//Using strcat I have created a variable temp which stores all the information.
    temp = username;
    temp = strcat(temp, "@");
    temp = strcat(temp, hostname);
    temp = strcat(temp, cwd);
    temp = strcat(temp, ">");

        return temp;


  }

void execute(char **str, int background){

//This is my function execute, it forks and checks if pid == 0, if it is that is the child process, else it is the parent

    pid_t pid = fork();
    int status;

    if (pid == 0){              //if pid == 0, execute the command.

        execvp(str[0], str);
        exit(0);
    }
    

    else{                      //else, wait

        
        waitpid(pid, &status, 0); 
    }
    return;
}

int commandcontrol(char** commands){

//This function is my commandcontrol. 

     int status;
     

    if(strcmp(commands[0], "exit") == 0){       //if exit is typed, exit the program
        exit(0);
    }

    else if(strcmp(commands[0], "cd") == 0){    // if cd is typed, check the other parameters
        
        if(commands[1] == NULL){                // if just "cd" is typed, go to the "HOME" directory
            chdir(getenv("HOME"));
        }

        else if (strcmp(commands[1], "~") == 0){    // if "cd ~" is typed, go to the home directory
            chdir(getenv("HOME"));
        }
        else{                                       //else, go to the specified directory
            chdir(commands[1]);
        
    }
    }

    if(strcmp(commands[0], "bg") == 0){
        char* commands_background[1024];
        

        int i =1;
        int j = 0;

        while(commands[i] != NULL){

            commands_background[j] = commands[j];
            i++;
            j++;
        }

        pid_t pid = fork();

            if(pid == 0){
            execvp(commands_background[0],commands_background);
            

            exit(0);
        }
        else{

            waitpid(pid,&status,0);
        }
        
    }

    else{
        execute(commands,1);
    }
}

int main()
{

    //main function

    char *listofcommands[4] = {"bg","ls","-l",NULL};        //List of pre determined commands
    
        
    while(1){
       const char* output = get_info();                     //Call our get_info function to get our hostname,username and directory

        char *line;

        line = readline(output);                           //Use readline to print output
        if(strlen(line) > 0){
            add_history(line);
        

        char *token;                                        //tokenize string 

        token = strtok(line, " ");

        int count = 0;

        while(token != NULL){

            listofcommands[count] = token;

            count++;

            token = strtok(NULL, " ");

        }

        listofcommands[count] = NULL;

        commandcontrol(listofcommands);

        }
    else{

        printf("enter a command\n");
    }

    }

}
