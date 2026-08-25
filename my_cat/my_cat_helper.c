#include "my_cat_helper.h"

// displays the contents of fp to stdout
void print_file(FILE *fp, Options *flags, size_t *line_number)
{
  if (fp == NULL)
  {
    return;
  }

  if (!(flags->active_flags))
  {
    print_file_basic(fp);
    return;
  }

  int c;
  int newline_count = 0;
  bool line_start = true;
  while ((c = fgetc(fp)) != EOF)
  {

    if (flags->squeeze_blank)
    {
      if (c == '\n')
      {
        // count newlines as squeeze removes 2+ newlines into 1
        if (newline_count < 2)
        {
          newline_count++;
        }
        else
        {
          while ((c = fgetc(fp)) == '\n')
          {
            continue;
          }

          if (c == EOF)
          {
            break; // outer while loop should end if EOF
          }

          ungetc(c, fp);
          continue;
        }
      }
      else
      {
        newline_count = 0;
      }
    }
    if (line_start)
    {
      if (flags->number_lines)
      {
        printf("%6zu\t", (*line_number)++);
        line_start = false;
      }
      else if (flags->number_nonblank_lines && c != '\n')
      {
        printf("%6zu\t", (*line_number)++);
        line_start = false;
      }
    }

    if (c == '\n')
    {
      line_start = true;
    }

    if (flags->show_ends && (c == 10 || c == 13))
    {
      if (c == 10)
      {
        printf("$\n");
        continue;
      }
      else if (c == 13)
      {
        printf("^%c", c + 64);
        continue;
      }
    }
    if (flags->show_tabs && c == 9)
    {
      printf("^%c", c + 64);
      continue;
    }

    if (flags->show_nonprinting && c < 32 && !(c == 9 || c == 10))
    {
      printf("^%c", c + 64);
      continue;
    }

    fputc(c, stdout);
  }
}

void print_file_basic(FILE *fp)
{
  if (fp == NULL)
  {
    return;
  }

  int c;
  while ((c = fgetc(fp)) != EOF)
  {
    fputc(c, stdout);
  }
}

FILE *open_file(char *filepath)
{
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL)
  {
    fflush(stdout);
    perror(filepath);
    return NULL;
  }
  return fp;
}

// goes through the input and sets and flag in the CL args
void parse_flags(char *flag, Options *flags)
{
  if (flags->option_terminator)
  {
    return; // no more flags if this option is set
  }
  if (flag[0] != '-')
  {
    fprintf(stderr, "Invalid flag %s, exiting\n", flag);
    flags->invalid_flag = true;
    flags->exit_early = true;
  }

  if (flag[1] == '-')
  {
    if (strcmp(flag, "--") == 0)
    {
      flags->option_terminator = true; // stop processing flags
      return;
    }
    if (strcmp(flag, "--help") == 0)
    {
      flags->show_help = true;
      flags->exit_early = true;
    }
    else if (strcmp(flag, "--version") == 0)
    {
      flags->show_version = true;
      flags->exit_early = true;
    }
    else if (strcmp(flag, "--number") == 0)
    {
      if (flags->number_nonblank_lines)
      {
        return; // this cancels out -n flag
      }
      flags->number_lines = true;
    }
    else if (strcmp(flag, "--show-all") == 0)
    {
      flags->show_ends = true;
      flags->show_tabs = true;
      flags->show_nonprinting = true;
    }
    else if (strcmp(flag, "--number-nonblank") == 0)
    {
      flags->number_nonblank_lines = true;
      flags->number_lines = false; // if -b set it overrides -n
    }
    else if (strcmp(flag, "--show-ends") == 0)
    {
      flags->show_ends = true;
    }
    else if (strcmp(flag, "--squeeze-blank") == 0)
    {
      flags->squeeze_blank = true;
    }
    else if (strcmp(flag, "--show-tabs") == 0)
    {
      flags->show_tabs = true;
    }
    else if (strcmp(flag, "--show-nonprinting") == 0)
    {
      flags->show_nonprinting = true;
    }
    else if (flag[1] == '-')
    {
      fprintf(stderr, "my_cat: invalid option -- '%s'\n", flag);
      fprintf(stderr, "Try my_cat --help for more information.\n");
      flags->invalid_flag = true;
      flags->exit_early = true;
    }
    return; // always return early after setting a long option flag
  }

  int i = 1;
  while (flag[i] != '\0')
  {
    switch (flag[i])
    {
    case 'n':
      if (flags->number_nonblank_lines)
      {
        break; // -b flag overrides -n
      }
      flags->number_lines = true;
      break;
    case 'A':
      flags->show_ends = true;
      flags->show_tabs = true;
      flags->show_nonprinting = true;
      break;
    case 'b':
      flags->number_nonblank_lines = true;
      flags->number_lines = false; // -b flag overrides -n
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
      flags->invalid_flag = true;
      flags->exit_early = true;
    }
    i++;
  }
}

void parse_files(char *file_name, File_List *file_list)
{
  if (!(file_list->files))
  {
    // allocate room for one pointer
    file_list->files = malloc(sizeof(*file_list->files));

    if (!(file_list->files))
    {
      perror(file_name);
      exit(EXIT_FAILURE);
    }
    file_list->capacity++;
  }
  else if (file_list->capacity <= file_list->file_count)
  {
    char **file_list_temp =
        realloc(file_list->files,
                (file_list->capacity * 2) * sizeof(*file_list->files));
    if (!file_list_temp)
    {
      perror(file_name);
      cleanup(&(file_list->files));
      exit(EXIT_FAILURE);
    }
    file_list->files = file_list_temp;
    file_list->capacity *= 2;
  }
  // assign the ptr to file_name in file_list
  file_list->files[(file_list->file_count)++] = file_name;
}

void parse_arguments(int size, char **argv, File_List *file_list,
                     Options *flags)
{
  for (int i = 0; i < size; i++)
  {
    if (argv[i][0] == '-' && argv[i][1] != '\0' && !flags->option_terminator)
    {
      parse_flags(argv[i], flags);

      if (flags->exit_early) // some flags or invalid option cause program to
                             // exit early
      {
        // return early to main
        return;
      }
    }
    else
    {
      parse_files(argv[i], file_list);
    }
  }
}

void set_active_flags(Options *flags)
{
  flags->active_flags = flags->number_lines || flags->number_nonblank_lines ||
                        flags->show_ends || flags->squeeze_blank ||
                        flags->show_tabs || flags->show_nonprinting;
}

void print_help(char *program_name)
{
  printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
  printf("Concatenate File(s) to standard output.\n");
  printf("\nWith no FILE, or when FILE is -, read standard input.\n");
  printf("\n   %-25sequivalent to -vET\n", "-A, --show-all");
  printf("   %-25snumber nonempty output lines, overrides -n\n",
         "-b, --number-nonblank");
  printf("   %-25sequivalent to -vE\n", "-e");
  printf("   %-25sdisplay $ at end of each line\n", "-E, --show-ends");
  printf("   %-25snumber all output lines\n", "-n, --number");
  printf("   %-25ssuppress repeated empty output lines\n",
         "-s, --squeeze-blank");
  printf("   %-25sequivalent to -vT\n", "-t");
  printf("   %-25sdisplay TAB characters as ^I\n", "-T, --show-tabs");
  printf("   %-25s(ignored)\n", "-u");
  printf("   %-25suse ^ and M- notation, except for LFD and TAB\n",
         "-v, --show-nonprinting");
  printf("\t    %-14sdisplay this help and exit\n", "--help");
  printf("\t    %-14soutput version information and exit\n", "--version");

  printf("\nExamples:\n");
  printf("   %s %-3s Output f's contents, then standard input, then g's "
         "contents.\n",
         program_name, "f - g");
  printf("   %-11s Copy standard input to standard output.\n", program_name);

  printf("\nThis is a copy of cat, a GNU coreutils program. It was made to "
         "practice programming in C.\n");
}

void print_version(char *program_name)
{
  printf("%s (clone of GNU coreutils) 9.4", program_name);
  printf("\nNo claim of any copyright or license\n");
  printf("There is NO WARRANTY, to the extent permitted by law.\n");
  printf("\nWritten by Abrasive Squid\n");
}

void cleanup(char ***files)
{
  if (!(*files))
  {
    return; // doesn't deallocate if *files is null
  }
  free(*files);
}
