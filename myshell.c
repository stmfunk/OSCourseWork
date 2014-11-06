#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define PS1 "MyShell> "

// Heap Variables
char *cwd;

// Function declarations
void sigIntEventHandle(int some); // This function handles interrupts by 
                                  //returning caller to the start of the main event loop
int ioLoop(); // Main event loop takes care of IO and commands
//void graceExit(); // This handles a SIGSEGV to prevent a segmenation fault on C-d
void charStrip(); // Removes trailing newlines


int main(int argc, char *argv[]) {
  signal(SIGINT, sigIntEventHandle);
 // signal(SIGSEGV, graceExit);

  return ioLoop();
}


void sigIntEventHandle(int event) {
  printf("\n");
  ioLoop();
}


/*void graceExit(int event) {
  write("\n");
  exit(0);
}*/


int ioLoop() {
  char *input = malloc(sizeof(char)*200);
  if (cwd == NULL)
    cwd = getenv("HOME");

  while (1) {
    printf(PS1);
    fgets(input, 100, stdin);
    charStrip(input);
    if (!strcmp(input,"exit")) return 0;

    if (strcmp(input,"cd") == 0) {
      cwd = getenv("HOME");
      printf("%s\n", cwd);
    } else if (strcmp(input,"cd") > 0);

    if (strcmp(input,"pwd") == 0)
      printf("%s\n", cwd);
  }
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
