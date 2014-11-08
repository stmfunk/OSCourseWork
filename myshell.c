#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define PS1 "MyShell> "


// Heap Variables
char *cwd;
int lastReturn;


// Struct delclarations

struct arguments {
  // This handles command line arguments
  int argc;
  char *argv[30];
};

typedef struct arguments args;


// Function declarations
void sigIntEventHandle(int some);                 // This function handles interrupts by 
                                                  //returning caller to the start of the main event loop
void graceExit();                                 // This handles a SIGSEGV to prevent a segmenation fault on C-d
void charStrip();                                 // Removes trailing newlines
args *getArgs(char *input);                       // Formats arguments into a nice list
int cd(args *cdArgs);                             // Function takes care of the changing of directory
int startSame(char *shorter, char *longer);       // Takes a shorter string and a longer string and if the start of 
                                                  // the longer string matches the shorter string return 1




int main(int argc, char *argv[]) {
  cwd = malloc(sizeof(char)*150);
  getcwd(cwd, 150);
  signal(SIGINT, sigIntEventHandle);
  signal(SIGHUP, graceExit);

  char *input = malloc(sizeof(char)*200);

  while (1) {
    printf(PS1);
    fgets(input, 100, stdin);
    charStrip(input);

    // This handles a call to exit
    if (startSame("exit",input)) {
      int n;
      args *argEx = getArgs(input);
      if (argEx->argv[0] != NULL) n = strtol(argEx->argv[0], NULL, 10);
      exit(n);
    }

    if (startSame("cd",input))
      cd(getArgs(input));


    if (startSame("pwd",input)) {
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

  int i,j,k;
  for (i = 0; input[i] != '\0'; i++)
    if (input[i] == ' ')
      for (; input[i] == ' '; i++); // This is here to remove any additional spaces between arguments
      j++;

  char *ourArgs = malloc(sizeof(char)*i);
  strcpy(ourArgs,input);
  retArgs->argc = j;

  j = 0;
  for (i = 0; ourArgs[i] != '\0'; i++) {
    if (ourArgs[i] == ' ') {
      ourArgs[i] = '\0';
      for (; input[i] == ' '; i++) 
        ourArgs[i] = '\0';
      retArgs->argv[j++] = ourArgs + i;
    }
  }
  return retArgs;
}

int cd(args *cdArgs) {
  char *temp_cwd;
  if (cdArgs->argc == 0) {
    temp_cwd = getenv("HOME");
  } else {
    if (cdArgs->argv[0][0] == '/') {
      temp_cwd = cdArgs->argv[0];
    }
  }

  if (chdir(temp_cwd) == -1)
    printf("cd: no such file or directory %s\n", temp_cwd);
  else cwd = temp_cwd;
  return 0;
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
      if (str[i-1] == '\n' || str[i-1] == ' ') {
        str[i-1] = '\0';
        return;
      }
    }
}


int startSame(char *shorter, char *longer) {
  int i;
  if (strlen(shorter) > strlen(longer)) return 0;
  for (i = 0; shorter[i] != '\0' && longer[i] != '\0'; i++) {
    if (shorter[i] != longer[i]) return 0;
  }
  return 1;
}
