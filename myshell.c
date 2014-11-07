#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define PS1 "MyShell> "


// Heap Variables
char *cwd;


// Function declarations
void sigIntEventHandle(int some); // This function handles interrupts by 
                                  //returning caller to the start of the main event loop
int ioLoop(); // Main event loop takes care of IO and commands
void graceExit(); // This handles a SIGSEGV to prevent a segmenation fault on C-d
void charStrip(); // Removes trailing newlines



// Struct delclarations

struct args {
  int argc;
  char *argv;
};


int main(int argc, char *argv[]) {
  cwd = malloc(sizeof(char)*150);
  signal(SIGINT, sigIntEventHandle);
  signal(SIGHUP, graceExit);

  return ioLoop();
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


int ioLoop() {
  char *input = malloc(sizeof(char)*200);

  while (1) {
    printf(PS1);
    fgets(input, 100, stdin);
    charStrip(input);

    // This handles a call to exit
    if (!strcmp(input,"exit")) {
      int n = 0;
      //obtainArgs(input);
      exit(n);
    }

    if (strcmp(input,"cd") == 0) {
      cwd = getenv("HOME");
      chdir(cwd);
      printf("%s\n", cwd);
    } else if (strcmp(input,"cd") > 0)
      ;

    if (strcmp(input,"pwd") == 0) {
      getcwd(cwd, 150);
      printf("%s\n", cwd);
    }

    *input = '\0';
  }
  free(input);
  return 0;
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
