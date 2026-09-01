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

void sort_pathnames(PathNames *pathnames)
{
  if (pathnames->dir_count > 1)
  {
    qsort(pathnames->dir_names, pathnames->dir_count, sizeof(*(pathnames->dir_names)),
          compare_paths);
  }
  if (pathnames->file_count > 1)
  {
    qsort(pathnames->file_names, pathnames->file_count, sizeof(*(pathnames->file_names)),
          compare_paths);
  }
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

void cleanup(PathNames *pathnames)
{
  if (pathnames->file_names)
  {
    free(pathnames->file_names);
  }
  if (pathnames->dir_names)
  {
    free(pathnames->dir_names);
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
  file_list->blocksize_sum = 0;
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
    if (S_ISREG(curr_file->file_stats->st_mode))
    {
      file_list->blocksize_sum += curr_file->file_stats->st_blocks; // sum of blocksize
    }
  }
  closedir(dp);

  return file_list;
}

FileList *create_file_list_files(char **filenames, size_t size, bool hidden_files)
{
  FileList *file_list = malloc(sizeof(*file_list));
  if (!(file_list))
  {
    fprintf(stderr, "Error allocating memory in create_file_list, exiting.\n");
    return NULL;
  }

  // initialize file_list then allocate memory for files
  file_list->files = NULL;
  file_list->file_capacity = size; // capacity to size
  file_list->file_count = 0;
  file_list->blocksize_sum = 0; // init value but not used for files by themselves
  file_list->files =
      alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));

  // loop through filenames and create file_details and add to file_list
  FileDetails *curr_file = NULL;
  for (size_t i = 0; i < size; i++)
  {
    if (!hidden_files)
    {
      if (filenames[i][0] == '.' && (strlen(filenames[i]) > 1) && filenames[i][1] != '/')
        continue; // skip dotfiles if hidden_files flag not set
    }

    curr_file = create_file_details(filenames[i], filenames[i]);
    if (!(curr_file))
    {
      fprintf(stderr, "Error allocating memory in create_file_list, exiting.\n");
      free_file_list(file_list);
      return NULL;
    }
    file_list->files[(file_list->file_count)++] = curr_file;
  }
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
  int arg_return_code = 0;
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
        return_code = 2;
      }
    }
  }
  // if no pathname set, default to current directory
  if (!(path_names->dir_count || path_names->file_count) && !arg_return_code)
  // if arg_return_code == 1, means only invalid paths given and program shouldn't continue
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
    fprintf(stderr, "./my_ls: cannot access '%s': No such file or directory\n", path);
    return 1;
  }

  // check if path is a file or directory and add to correct array in pathnames
  if (S_ISREG(buf.st_mode))
  {
    if (path_names->file_capacity <= path_names->file_count)
    { // double capacity and allocate memory
      path_names->file_capacity *= 2;
      path_names->file_names = alloc_array(path_names->file_names, path_names->file_capacity,
                                           sizeof(*(path_names->file_names)));
    }
    path_names->file_names[(path_names->file_count++)] = path;
  }
  else if (S_ISDIR(buf.st_mode))
  {

    if (path_names->dir_capacity <= path_names->dir_count)
    { // double capacity and allocate memory
      path_names->dir_capacity *= 2;
      path_names->dir_names = alloc_array(path_names->dir_names, path_names->dir_capacity,
                                          sizeof(*(path_names->dir_names)));
    }
    path_names->dir_names[(path_names->dir_count++)] = path;
  }
  else
  {
    fprintf(stderr, "./my_ls: cannot access '%s': No such file or directory\n", path);
    return 1;
  }

  // add new path to path_names and increment count
  return 0;
}

int get_blocksize(void)
{
  int blocksize = 1024; // set the default
  int size = 4;
  char *env_arr[size];

  env_arr[0] = (getenv("LS_BLOCK_SIZE"));
  env_arr[1] = (getenv("BLOCK_SIZE"));
  env_arr[2] = (getenv("BLOCKSIZE"));
  env_arr[3] = (getenv("POSIXLY_CORRECT"));

  for (int i = 0; i < size; i++)
  {

    if (env_arr[i])
      return atoi(env_arr[i]);
  }
  return blocksize;
}

char *build_file_perm_string(char *c_ptr, int size, struct stat *file_stats)
{

  if (size < 11)
  {
    fprintf(stderr, "Not enough memory allocated for permission string, exiting\n");
    return NULL;
  }

  // reset string
  for (int i = 0; i < size - 1; i++)
  {
    c_ptr[i] = '-';
  }
  c_ptr[size - 1] = '\0'; // set nullbyte

  // set file bit
  if (S_ISDIR(file_stats->st_mode))
  {
    c_ptr[0] = 'd';
  }

  // set userbits
  if (file_stats->st_mode & S_IRUSR)
  {
    c_ptr[1] = 'r';
  }
  if (file_stats->st_mode & S_IWUSR)
  {
    c_ptr[2] = 'w';
  }
  if (file_stats->st_mode & S_IXUSR)
  {
    c_ptr[3] = 'x';
  }
  // set group bits
  if (file_stats->st_mode & S_IRGRP)
  {
    c_ptr[4] = 'r';
  }
  if (file_stats->st_mode & S_IWGRP)
  {
    c_ptr[5] = 'w';
  }
  if (file_stats->st_mode & S_IXGRP)
  {
    c_ptr[6] = 'x';
  }
  // set other bits
  if (file_stats->st_mode & S_IROTH)
  {
    c_ptr[7] = 'r';
  }
  if (file_stats->st_mode & S_IWOTH)
  {
    c_ptr[8] = 'w';
  }
  if (file_stats->st_mode & S_IXOTH)
  {
    c_ptr[9] = 'x';
  }

  return c_ptr;
}
