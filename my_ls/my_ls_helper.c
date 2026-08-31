/* Contains the funciton definitions to support my_ls */

#include "my_ls_helper.h"
#include "debug.h"

int compare_filenames(const void *a, const void *b)
{
  const struct dirent *a_struct = (const struct dirent *)a;
  const char *a_str = a_struct->d_name;
  const struct dirent *b_struct = (const struct dirent *)b;
  const char *b_str = b_struct->d_name;

  return strcoll(a_str, b_str);
}

int get_window_columns(int fd)
{
  struct winsize size;
  if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
  {
    fprintf(stderr, "TIOCGWINSZ error");
  }
  return size.ws_col;
}

void cleanup(FileList *file_list, PathNames *pathnames)
{
  // only free file_list if allocated
  if (file_list)
  {
    // only free the struct dirent *, if mem was allocated
    if (file_list->files)
    {
      free(file_list->files);
    }
    free(file_list);
  }

  if (pathnames->pathnames)
  {
    free(pathnames->pathnames);
  }
  // // only free col_widths if mem was allocated
  // if (col_widths)
  // {
  //   free(col_widths);
  // }
}

void *alloc_array(void *arr, size_t size, size_t elem_size)
{
  if (!(arr))
  {
    arr = malloc(elem_size * size);
    if (!(arr))
    {
      fprintf(stderr, "error allocating memory, returning NULL\n");
      return NULL;
    }
    return arr;
  }

  size_t *temp_arr = realloc(arr, elem_size * size);
  if (!(temp_arr))
  {
    fprintf(stderr, "error allocating memory, returning NULL\n");
    return NULL;
  }
  return temp_arr;
}

FileList *create_file_list(char *pathname)
{
  DIR *dp;
  struct dirent *dirp;

  dp = opendir(pathname);
  if (!(dp))
  {
    perror(pathname);
    return NULL;
  }

  // initialize FileList struct that holds details of directories files
  FileList *file_list = malloc(sizeof(*file_list));
  if (!(file_list))
  {
    fprintf(stderr, "Error allocating memory in create_file_list, exiting.\n");
    return NULL;
  }

  // initialize file_list then allocate memory for files
  file_list->files = NULL;
  file_list->file_capacity = 4; // set inital capacity for 4
  file_list->file_count = 0;
  file_list->offset = 0;
  file_list->files =
      alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));

  // read all files in pathname and all to files array
  while ((dirp = readdir(dp)) != NULL)
  {
    if (file_list->file_capacity <= file_list->file_count)
    {
      file_list->file_capacity *= 2;
      file_list->files =
          alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));
    }
    // add file to file_list
    file_list->files[file_list->file_count++] = *dirp;
  }
  closedir(dp);

  return file_list;
}

int parse_arguments(int size, char **argv, PathNames *path_names, unsigned int *options)
{
  int arg_return_code;
  int return_code = 0; // assume success, change only if failed pathname
  for (int i = 1; i < size; i++)
  {
    if (argv[i][0] == '-') // check for flag indicator
    {
      if ((return_code = parse_option(argv[i], options)))
      {
        // flag not set properly, should return 0
        if (return_code == 1)
        {
          fprintf(stderr, "%s: unrecognized option '%s'\nTry '%s --help' for more information.\n",
                  argv[0], argv[i], argv[0]);
        }
        else
        {
          // return code is the char that is unrecognized
          fprintf(stderr, "%s: unrecognized option '%c'\nTry '%s --help' for more information.\n",
                  argv[0], return_code, argv[0]);
        }
        return return_code;
      }
    }
    else // default argument is a pathname
    {
      if ((arg_return_code = parse_paths(argv[i], path_names)))
      {
        return_code = 1;
      }
    }
  }
  // if no pathname set, default to current directory
  if (!(path_names->count))
  {
    if ((arg_return_code = parse_paths(".", path_names)))
    {
      return_code = 1;
    }
  }
  return return_code;
}

int parse_option(char *flag, unsigned int *options)
{
  if (flag[0] != '-')
  {
    return 1;
  }

  if (flag[1] == '-') // long option
  {
    if (strcmp(flag, "--help") == 0)
    {
      *options |= FLAG_HELP;
      return 0;
    }
    else if (strcmp(flag, "--version") == 0)
    {
      *options |= FLAG_VER;
      return 0;
    }
    else if (strcmp(flag, "--all") == 0)
    {
      *options |= FLAG_ALL;
      return 0;
    }

    else
    {
      return 1;
    }
  }

  // loop through all flags set by single argument flag
  int i = 1;
  while (flag[i] != '\0') // loop until null char
  {
    switch (flag[1])
    {
    case 'a':
      *options |= FLAG_ALL;
      break;
    default:
      return flag[1];
    }
    i++;
  }

  // not a long option

  return 0;
}

int parse_paths(char *path, PathNames *path_names)
{
  DIR *dp = opendir(path);
  if (!(dp))
  {
    fprintf(stderr, "./my_ls: cannot access: %s: No such file or directory\n", path);
    return 1;
  }
  if (path_names->capacity <= path_names->count)
  {
    // double capacity and allocate memory for another pathname
    path_names->capacity *= 2;
    path_names->pathnames =
        alloc_array(path_names->pathnames, path_names->capacity, sizeof(path_names->pathnames));
  }
  // add new path to path_names and increment count
  path_names->pathnames[(path_names->count)++] = path;
  closedir(dp);
  return 0;
}

int set_hidden_files(FileList *file_list, int all_flag)
{
  if (all_flag) // if flag set, no change and returns
  {
    return 0;
  }
  size_t i = 0;
  for (; i < file_list->file_count; i++)
  {
    if (file_list->files[i].d_name[0] != '.')
    {
      break; // end of dotfiles
    }
  }
  if (file_list->files[i].d_name[0] == '.')
  {
    // no non-dotfiles, exit
    return 0;
  }
  // set files to first non-dotfile and decrease file_count
  file_list->offset += i;
  return 0; // success
}
