#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#define PS1 "MyShell> "

// Heap Variables
char *cwd;
char *glob_env;


// Function declarations
void sigIntEventHandle(int some); // This function handles interrupts by 
                                  //returning caller to the start of the main event loop
char *getEnv(char *envName, char *env[]); // Basic search for enviroment variables (linear)
                                          // not very sophisticated just searches for anything matching the first part of the envName
int ioLoop(char *env); // Main event loop takes care of IO and commands
void graceExit(); // This handles a SIGSEGV to prevent a segmenation fault on C-d
void charStrip(); // Removes trailing newlines


int main(int argc, char *argv[], char *env[]) {
  glob_env = *env;
  signal(SIGINT, sigIntEventHandle);
  signal(SIGSEGV, graceExit);

  return ioLoop(&env);
}


void sigIntEventHandle(int event) {
  printf("\n");
  ioLoop(glob_env);
}


void graceExit(int event) {
  printf("\n");
  exit(0);
}


char *getEnv(char *envName, char *env[]) {
  int i;
  for (i = 0; env[i] != NULL; i++) 
    if (strcmp(env[i], envName) > 0) 
      break;
  return env[i];
}


int ioLoop(char *env) {
  char *input = malloc(sizeof(char)*200);
  if (cwd == NULL)
    cwd = getEnv("HOME", env);

  while (1) {
    printf(PS1);
    fgets(input, 100, stdin);
    charStrip(input);
    if (!strcmp(input,"exit")) return 0;

    if (strcmp(input,"cd") == 0) {
      cwd = getEnv("HOME",env);
      printf("%s\n", cwd);
    } else if (strcmp(input,"cd") > 0);

    if (strcmp(input,"cwd") == 0)
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
