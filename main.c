#define HAVE_STRUCT_TIMESPEC
#include "set_header.h"

int main() {
  char input_command[100];
  Users *userlist = Initial();
  DirectTree *dirtree = NULL;
  dirtree = Load_Directory_Info(userlist, "Directory.txt", "Data.txt");
  if (dirtree == NULL) {
    dirtree = Initial_Dir(userlist);
  }
  while (1) {
    printf("\nCurrent : %s\n\n", dirtree->current->name);
    if (fgets(input_command, sizeof(input_command), stdin) == NULL) {
      if (feof(stdin)) {
        // Clear the EOF flag on stdin
        clearerr(stdin);
        printf("\n");
        continue;
      }
      break;
    }

    // Remove newline character if present
    size_t len = strlen(input_command);
    if (len > 0 && input_command[len - 1] == '\n') {
      input_command[len - 1] = '\0';
    }

    char *command = strtok(input_command, " ");
    char *argument = strtok(NULL, "");

    if (command == NULL) {
      continue; // If no command was entered, continue the loop
    }

    if (strcmp(command, "mkdir") == 0) {
      multi_mkdir(argument, dirtree, userlist);
    } else if (strcmp(command, "ls") == 0) {
      ls(argument, dirtree, userlist);
    } else if (strcmp(command, "cd") == 0) {
      cd(argument, dirtree, userlist);
    } else if (strcmp(command, "touch") == 0) {
      touch(argument, dirtree);
    } else if (strcmp(command, "rmdir") == 0) {
      removedir(argument, dirtree, userlist);
    } else if (strcmp(command, "grep") == 0) {
      grep(argument, dirtree, userlist);
    } else if (strcmp(command, "head") == 0) {
      head(argument, dirtree, userlist);
    } else if (strcmp(command, "tail") == 0) {
      tail(argument, dirtree, userlist);
    } else if (strcmp(command, "cp") == 0) {
      cp(argument, dirtree, userlist);
    } else if (strcmp(command, "chown") == 0) {
      chown(argument, dirtree, userlist);
    } else if (strcmp(command, "chmod") == 0) {
      chmod(dirtree, argument);
    } else if (strcmp(command, "cat") == 0) {
      cat(dirtree, argument);

    }

    else {
      printf("Invalid command: %s\n", input_command);
    }
  }
  free(dirtree);
  free(userlist);
  pthread_exit(NULL);
  return 0;
}
