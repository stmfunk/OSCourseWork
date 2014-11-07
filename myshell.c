#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define PS1 "MyShell> "


// Heap Variables
char *cwd;


// Struct delclarations

struct arguments {
  int argc;
  char *argv[30];
};

typedef struct arguments args;


// Function declarations
void sigIntEventHandle(int some); // This function handles interrupts by 
                                  //returning caller to the start of the main event loop
void graceExit(); // This handles a SIGSEGV to prevent a segmenation fault on C-d
void charStrip(); // Removes trailing newlines
args *getArgs(char *input);



int main(int argc, char *argv[]) {
  cwd = malloc(sizeof(char)*150);
  signal(SIGINT, sigIntEventHandle);
  signal(SIGHUP, graceExit);

   char *input = malloc(sizeof(char)*200);

  while (1) {
    printf(PS1);
    fgets(input, 100, stdin);
    charStrip(input);

    // This handles a call to exit
    if (strcmp(input,"exit") >= 0) {
      int n;
      args *argEx = getArgs(input);
      if (argEx->argv[0] != NULL) n = atoi(argEx->argv[0]);
      exit(n);
    }

    if (strcmp(input,"cd") == 0) {
      cwd = getenv("HOME");
      chdir(cwd);
      printf("%s\n", cwd);
    } else if (strcmp(input,"cd") > 0) {
      printf("%s\n", getArgs(input)->argv[0]);
    }

    if (strcmp(input,"pwd") == 0) {
      getcwd(cwd, 150);
      printf("%s\n", cwd);
    }

    *input = '\0';
  }
  free(input);
  
  return 0;
}


args *getArgs(char *input) {
  args *retArgs = malloc(sizeof(args));

  int i,j;
  for (i = 0; input[i] != '\0'; i++)
    if (input[i] == ' ') j++;

  char *ourArgs = malloc(sizeof(char)*i);
  strcpy(ourArgs,input);

  j = 0;
  for (i = 0; ourArgs[i] != '\0'; i++) {
    if (ourArgs[i] == ' ') {
      ourArgs[i] = '\0';
      retArgs->argv[j++] = ourArgs + i + 1;
    }
  }
  return retArgs;
}


void sigIntEventHandle(int event) {
  signal(SIGINT, sigIntEventHandle);
  printf("\n");
  printf(PS1);
  fflush(stdout);
  return;
}

void graceExit(int event) {
  printf("\n");
  fflush(stdout);
  exit(0);
}


void charStrip(char* str) {
  int i;
  for (i = 0; ; i++)
    if (str[i] == '\0') {
      if (str[i-1] == '\n') {
        str[i-1] = '\0';
        return;
      }
    }
}
