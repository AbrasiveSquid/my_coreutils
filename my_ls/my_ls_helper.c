/* Contains the funciton definitions to support my_ls */
#define _POSIX_C_SOURCE 200809L
#include "my_ls_helper.h"

int compare_filenames(const void *a, const void *b)
{
  const char *a_str = (*(const FileDetails **)a)->filename;
  const char *b_str = (*(const FileDetails **)b)->filename;

  return strcoll(a_str, b_str);
}

int compare_paths(const void *a, const void *b)
{
  const char *a_str = *(const char **)a;
  const char *b_str = *(const char **)b;
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
  free_file_list(file_list);
  if (pathnames->pathnames)
  {
    free(pathnames->pathnames);
  }
}

void free_file_list(FileList *file_list)
{
  if (!(file_list))
  {
    return;
  }

  if (file_list->files)
  {
    for (size_t i = 0; i < file_list->file_count; i++)
    {
      free(file_list->files[i]->filename);
      free(file_list->files[i]->file_stats);
      free(file_list->files[i]);
    }
    free(file_list->files);
  }
  free(file_list);
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

FileList *create_file_list(char *pathname, bool hidden_files)
{
  struct stat buf;
  FileList *file_list = NULL;

  // checks if file can be accessed or exists
  if (stat(pathname, &buf) < 0)
  {
    perror(pathname);
    return NULL;
  }

  // creates file_list depending on file type
  if (S_ISREG(buf.st_mode))
  {
    file_list = create_file_list_file(pathname);
  }
  else if (S_ISDIR(buf.st_mode))
  {
    file_list = create_file_list_dir(pathname, hidden_files);
  }

  if (!(file_list))
  {
    perror(pathname);
    return NULL;
  }
  return file_list;
}

FileList *create_file_list_dir(char *pathname, bool hidden_files)
{
  DIR *dp;
  struct dirent *dirp;
  FileDetails *curr_file = NULL;
  int pathlen = 0;

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
    free_file_list(file_list);
    return NULL;
  }

  // initialize file_list then allocate memory for files
  file_list->files = NULL;
  file_list->file_capacity = 4; // set inital capacity for 4
  file_list->file_count = 0;
  file_list->files =
      alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));

  // read all files in pathname and all to files array
  while ((dirp = readdir(dp)) != NULL)
  {
    // if hidden_files not set, skips all dotfiles
    if (!hidden_files && ((*dirp).d_name[0] == '.'))
    {
      continue;
    }
    if (file_list->file_capacity <= file_list->file_count)
    {
      file_list->file_capacity *= 2;
      file_list->files =
          alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));
    }

    // builds up the full path of a filename to pass to create_file_details
    pathlen = strlen(pathname) + strlen((*dirp).d_name) + 2; // +2 for null char  and /
    char fullpath[pathlen];
    strncpy(fullpath, pathname, pathlen);
    // check that pathname is not empty, and if it doesnt end with /, appends a /
    if (strlen(pathname) > 0 && pathname[strlen(pathname) - 1] != '/')
    {
      strcat(fullpath, "/");
    }
    strncat(fullpath, (*dirp).d_name, strlen((*dirp).d_name));
    curr_file = create_file_details(fullpath, (*dirp).d_name);
    if (!(curr_file))
    {
      perror((*dirp).d_name);
      free_file_list(file_list);
      closedir(dp);
      return NULL;
    }
    // add file to file_list
    file_list->files[file_list->file_count++] = curr_file;
  }
  closedir(dp);

  return file_list;
}

FileList *create_file_list_file(char *pathname)
{
  FileList *file_list = malloc(sizeof(*file_list));
  FileDetails *curr_file = create_file_details(pathname, pathname);

  if (!(file_list) || !(curr_file))
  {
    fprintf(stderr, "Error allocating memory in create_file_list, exiting.\n");
    return NULL;
  }

  // initialize file_list then allocate memory for files
  file_list->files = NULL;
  file_list->file_capacity = 1; // set inital capacity for 4
  file_list->file_count = 0;
  file_list->files =
      alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));

  file_list->files[(file_list->file_count)++] = curr_file;
  return file_list;
}

FileDetails *create_file_details(char *pathname, char *filename)
{
  // allocate memory for FileDetails and check it was allocated
  FileDetails *curr_file = malloc(sizeof(*curr_file));
  if (!(curr_file))
  {
    return NULL;
  }
  // allocate memory and assign filename
  curr_file->filename = malloc(sizeof(char) * (strlen(filename) + 1));
  if (!(curr_file->filename))
  {
    free(curr_file);
    return NULL;
  }
  strcpy(curr_file->filename, filename);

  // assign memory and assign file stats
  curr_file->file_stats = malloc(sizeof(*(curr_file->file_stats)));
  if (!(curr_file->file_stats))
  {
    free(curr_file->filename);
    free(curr_file);
    return NULL;
  }

  // lstat filename
  if (lstat(pathname, curr_file->file_stats) < 0)
  {
    perror(pathname);
    free(curr_file->filename);
    free(curr_file->file_stats);
    free(curr_file);
    return NULL;
  }

  return curr_file;
}

int parse_arguments(int size, char **argv, PathNames *path_names, unsigned int *options)
{
  int arg_return_code;
  int return_code = 0; // assume success, change only if failed pathname
  for (int i = 1; i < size; i++)
  {
    if (argv[i][0] == '-') // check for flag indicator
    {
      if ((return_code = parse_options(argv[i], options)))
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

int parse_options(char *flag, unsigned int *options)
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

  // loop through all flags set by single argument flag, and if flag is valid, sets bitmask
  int i = 1;
  while (flag[i] != '\0') // loop until null char
  {
    switch (flag[1])
    {
    case 'a':
      *options |= FLAG_ALL;
      break;
    case 'l':
      *options |= FLAG_LIST;
      break;
    default:
      return flag[1];
    }
    i++;
  }

  return 0;
}

int parse_paths(char *path, PathNames *path_names)
{
  struct stat buf;
  // DIR *dp = opendir(path);
  if (stat(path, &buf) < 0)
  {
    fprintf(stderr, "./my_ls: cannot access: %s: No such file or directory\n", path);
    return 1;
  }
  if (path_names->capacity <= path_names->count)
  {
    // double capacity and allocate memory for another pathname
    path_names->capacity *= 2;
    path_names->pathnames =
        alloc_array(path_names->pathnames, path_names->capacity, sizeof(*(path_names->pathnames)));
  }
  // add new path to path_names and increment count
  path_names->pathnames[(path_names->count)++] = path;
  return 0;
}
