#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Copying the features of cat cli program */
// will contain CLI flags
typedef struct {
  bool number_lines;
  bool number_nonblank_lines;
  bool show_ends;
  bool squeeze_blank;
  bool show_tabs;
  bool show_nonprinting;
} Options;

void print_file(FILE *fp, Options * flag, int * line_number);
FILE * open_file(char *filepath);
void parse_flags(char * flag, Options *flags); 
void parse_arguments(int size, char * arg_list[], char *file_list[], Options * flags);
void print_help(char * program_name); 
void print_version(char * program_name);
char *allocate_string(int size); 

int main(int argc, char *argv[]) {
  int exit_status = 0;
  int line_number = 1;
  char ** file_list = NULL;
  Options flags = {0}; // init all flags to false
  
  parse_arguments(argc, argv, file_list, &flags);

  // flag_set = parse_flags(argv, argc, &flags);
  
  // if no arguments or if only flags as arguments
  if (!file_list) {
    print_file(stdin, &flags, &line_number);
    return exit_status;
  }

  // opens each file path and displays them one after another
  FILE *fp;
  for (int i = 1; i < argc; i++) {
    // if argument is "-" takes input from stdin until EOF (ctrl+D) then continues  to next argument
    if (strcmp("-", argv[i]) == 0) {
      print_file(stdin, &flags, &line_number); 
    } else if (argv[i][0] == '-') {
        continue;
    }
    else {
      fp = open_file(argv[i]);
      if (fp == NULL) {
        exit_status = 1;
        continue;
      }
      print_file(fp, &flags, &line_number); 
      fclose(fp);
    }
  }
  
  fclose(stdout);
  return exit_status;
}

// displays the contents of fp to stdout
void print_file(FILE *fp, Options * flags, int * line_number) {
  if (fp == NULL) {
    return;
  }

  int c;
  int newline_count = 0;
  bool line_start = true;
  while ((c = fgetc(fp)) != EOF) {

    if (flags->squeeze_blank) {
      if (c == '\n') {
        if (newline_count < 2) {
          newline_count++;
        } else {
          while ((c = fgetc(fp)) == '\n') {
            continue;
          }
          ungetc(c, fp);
          continue;
        }
      }
      else {
        newline_count = 0;
      }
    }
    if (line_start){
      if (flags->number_lines) {
        printf("%6d\t", (*line_number)++);
        line_start = false;
      } else if (flags->number_nonblank_lines && c != '\n') {
        printf("%6d\t",(*line_number)++);
        line_start = false;
      }
    }

    if (c == '\n') {
      line_start = true;
    }

    if (flags->show_ends && (c == 10 || c == 13)) {
      if (c == 10) {
        printf("$\n");
        continue;
      } else if (c == 13) {
        printf("^%c", c + 64);
        continue;
      }
    }
    if (flags->show_tabs && c == 9) {
      printf("^%c", c+64);
      continue;
    }

    if (flags->show_nonprinting && c < 32 && !(c == 9 || c == 10)) {
      printf("^%c", c+64);
      continue;
    }

    fputc(c, stdout);
  }
  
}

FILE * open_file(char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    perror(filepath);
    return NULL;
  }
  return fp;
}

// goes through the input and sets and flag in the CL args
void parse_flags(char * flag, Options *flags) {
  if (flag[0] != '-') {
    fprintf(stderr, "Invalid flag %s, exiting\n", flag);
    exit(EXIT_FAILURE);
  }

  if (flag[1] == '-') {
   if (strcmp(flag, "--help") == 0) {
     print_help(flag);
   } else if (strcmp(flag, "--version") == 0) {
     print_version(flag);
   } else if (strcmp(flag, "--number") == 0) {
     flags->number_lines = true;
   } else if (strcmp(flag,"--show-all") == 0)  {
     flags->show_ends = true;
     flags->show_tabs = true;
     flags->show_nonprinting = true;
   } else if (strcmp(flag, "--number-nonblank") == 0) {
     flags->number_nonblank_lines = true;
   } else if (strcmp(flag, "--show-ends") == 0) {
     flags->show_ends = true;
   } else if (strcmp(flag, "--squeeze-blank") == 0) {
     flags->squeeze_blank = true;
   } else if (strcmp(flag, "--show-tabs") == 0) {
     flags->show_tabs = true;
   } else if (strcmp(flag, "--show-nonprinting") == 0) {
     flags->show_nonprinting = true;
    } else if (flag[1] == '-') {
     fprintf(stderr, "my_cat: invalid option -- '%s'\n", flag);
     fprintf(stderr, "Try my_cat --help for more information.\n");
     exit(EXIT_FAILURE);
    }
  }
  
  int i = 1;
  while (flag[i] != '\0') {
    switch (flag[i]) {
      case 'n':
        flags->number_lines = true;
        break;
      case 'A':
        flags->show_ends = true;
        flags->show_tabs = true;
        flags->show_nonprinting = true;
        break;
      case 'b':
        flags->number_nonblank_lines = true;
         break;
      case 'e':
         flags->show_ends = true;
         flags->show_nonprinting = true;
         break;
      case 'E':
         flags->show_ends = true;
         break;
      case 's':
         flags->squeeze_blank = true;
         break;
      case 't':
         flags->show_nonprinting = true;
         flags->show_tabs = true;
         break;
      case 'T':
         flags->show_tabs = true;
         break;
      case 'v':
         flags->show_nonprinting = true;
         break;
      case 'u':
         break;
      default:
         fprintf(stderr, "my_cat: invalid option -- '%c'\n", flag[i]);
         fprintf(stderr, "Try my_cat --help for more information.\n");
         exit(EXIT_FAILURE);
    }
    i++;
  }
  // -b flag overrides -n
  if (flags->number_nonblank_lines) {
    flags->number_lines = false;
  }
}

void parse_arguments(int size, char * argv[], char *file_list[], Options * flags) {
  char *temp_file_list[size - 1];
  int j = 0, i;

  for (i = 1; i < size; i++) {

    if (argv[i][0] == '-' && argv[i][1] != '\0' ) {
      parse_flags(argv[i], flags);
    } 
    else {
      temp_file_list[j] =  allocate_string(strlen(argv[i]));
      strcpy(temp_file_list[j++], argv[i]);
    }
  }
  // allocate memory for number of file list arguments
  if (j) {
    file_list = malloc(sizeof(file_list[0]) * j);
    if (!file_list) {
      perror(*file_list);
      exit(EXIT_FAILURE);
    }
    for (i = 0; i < j; i++) {
      strcpy(file_list[i], temp_file_list[i]);
    }
  }
}

char *allocate_string(int size) {
  char *str = malloc((sizeof(char) * size + 1)); // add 1 for null char

  if (str == NULL) {
    fprintf(stderr, "Error allocating memory in allocate_string, exiting\n");
    exit(EXIT_FAILURE);
  }

  return str;
}

void print_help(char * program_name) {
  printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
  printf("Concatenate File(s) to standard output.\n");
  printf("\nWith no FILE, or when FILE is -, read standard input.\n");
  printf("\n   %-25sequivalent to -vET\n","-A, --show-all");
  printf("   %-25snumber nonempty output lines, overrides -n\n", "-b, --number-nonblank");
  printf("   %-25sequivalent to -vE\n", "-e");
  printf("   %-25sdisplay $ at end of each line\n", "-E, --show-ends");
  printf("   %-25snumber all output lines\n", "-n, --number");
  printf("   %-25ssuppress repeated empty output lines\n", "-s, --squeeze-blank");
  printf("   %-25sequivalent to -vT\n", "-t");
  printf("   %-25sdisplay TAB characters as ^I\n", "-T, --show-tabs");
  printf("   %-25s(ignored)\n", "-u");
  printf("   %-25suse ^ and M- notation, except for LFD and TAB\n", "-v, --show-nonprinting");
  printf("\t    %-14sdisplay this help and exit\n", "--help");
  printf("\t    %-14soutput version information and exit\n", "--version");

  printf("\nExamples:\n");
  printf("   %s %-3s Output f's contents, then standard input, then g's contents.\n", program_name, "f - g");
  printf("   %-11s Copy standard input to standard output.\n", program_name);

  printf("\nThis is a copy of cat, a GNU coreutils program. It was made to practice programming in C.\n");

  exit(EXIT_SUCCESS);
}

void print_version(char * program_name) {
  printf("%s (clone of GNU coreutils) 9.4", program_name);
  printf("\nNo claim of any copyright or license\n");
  printf("There is NO WARRANTY, to the extent permitted by law.\n");
  printf("\nWritten by Abrasive Squid\n");

  exit(EXIT_SUCCESS);
}
