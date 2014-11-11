#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define DIR_NUM 2


// Heap Variables
char *cwd;
int lastReturn;
char *prompt;


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
char *getLastDirectories(int n, char *path);      // Take the last n directories from the part given




int main(int argc, char *argv[]) {
  cwd = malloc(sizeof(char)*150);
  signal(SIGINT, sigIntEventHandle);
  signal(SIGHUP, graceExit);

  char *input = malloc(sizeof(char)*200);

  prompt = malloc(sizeof(char)*80);

  while (1) {
    getcwd(cwd, 150);
    
    *prompt = '\0';
    strcat(prompt, getLastDirectories(DIR_NUM, cwd));
    strcat(prompt, "> ");

    printf("%s", prompt);
    fgets(input, 100, stdin);
    charStrip(input);

    // This handles a call to exit
    if (startSame("exit",input)) {
      int n;
      args *argEx = getArgs(input);
      if (argEx->argv[0] != NULL) n = strtol(argEx->argv[0], NULL, 10);
      exit(n);
    }

    if (startSame("cd",input)) {
      args *cdArgs = getArgs(input);
      cd(cdArgs);
      free(cdArgs);
    }


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

  int i,j,k = 0;;
  j = 0;
  charStrip(input);
  for (i = 0; input[i] != '\0'; i++) {
    if (input[i] == ' ') {
      for (; input[i] == ' '; i++); // This is here to remove any additional spaces between arguments
      j++;
    }
  }

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
    temp_cwd = cdArgs->argv[0];
  }

  if (chdir(temp_cwd) == -1)
    printf("cd: no such file or directory %s\n", temp_cwd);
  else cwd = temp_cwd;
  return 0;
}


void sigIntEventHandle(int event) {
  signal(SIGINT, sigIntEventHandle);
  printf("\n");
  printf("%s", prompt);
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
        i--;
      } else {
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


char *getLastDirectories(int n, char *path) {
  int i = 0;
  char *c;
  for (c = path; *c != '\0'; c++);
  c--;

  for (; c != path; c--) {
    if (*(c+1) == '/') i++;
    if (i == n) {
      c += 2;
      break;
    }
  }
  if (i < n) return path;
  else return c;
}
