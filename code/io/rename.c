#include "../include/csapp.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Rename all file in a specified directory */

/* Replace all `rep` occurrence in orig */
// Must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with) {
  char *result;  // the return string
  char *ins;     // the next insert point
  char *tmp;     // varies
  int len_rep;   // length of rep (the string to remove)
  int len_with;  // length of with (the string to replace rep with)
  int len_front; // distance between rep and end of last rep
  int count;     // number of replacements

  // sanity checks and initialization
  if (!orig || !rep)
    return NULL;
  len_rep = strlen(rep);
  if (len_rep == 0)
    return NULL; // empty rep causes infinite loop during count
  if (!with)
    with = "";
  len_with = strlen(with);

  // count the number of replacements needed
  ins = orig;
  /* Is 'rep' even in 'ins'? */
  for (count = 0; (tmp = strstr(ins, rep)); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    return NULL;

  // first time through the loop, all the variable are set correctly
  // from here on,
  //    tmp points to the end of the result string
  //    ins points to the next occurrence of rep in orig
  //    orig points to the remainder of orig after "end of rep"
  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
  }
  strcpy(tmp, orig);
  return result;
}
/* 程序不足之处：每次重命名一个文件， 调用str_replace 就需要`malloc`分配一次内存 */
int main(int argc, char *argv[]) {
  DIR *dp;
  struct stat stat;
  struct dirent *dirp;
  if (argc != 2) {
    app_error("usage: ls directory_name");
  }
  if ((dp = opendir(argv[1])) == NULL) {
    char *message = "can't open %s";
    sprintf(message, argv[1]);
    app_error(message);
  }
  while ((dirp = readdir(dp)) != NULL) {
    /* Init  char array with `\0` */
    char absolute_file_name[256] = {'\0'};
    strcat(absolute_file_name, argv[1]);
    strcat(absolute_file_name, "/");
    strcat(absolute_file_name, dirp->d_name);
    char *target = "犬夜叉";
    char *replaced = "";
    Stat(absolute_file_name, &stat);
    if (S_ISREG(stat.st_mode)) {
      char *new_file_name = str_replace(dirp->d_name, target, replaced);
      char absolute_new_file_name[256] = {'\0'};
      strcat(absolute_new_file_name, argv[1]);
      strcat(absolute_new_file_name, "/");
      strcat(absolute_new_file_name, new_file_name);
      /* For example, rename /home/samray/Video/犬夜叉/犬夜叉1.mp4 to
       * /home/samray/Video/犬夜叉/1.mp4*/
      if ((rename(absolute_file_name, absolute_new_file_name))) {
        printf("Failed to rename %s to %s\n", absolute_file_name,
               absolute_new_file_name);
      }
      free(new_file_name);
    }
  }
  closedir(dp);
  exit(0);
  return 0;
}
