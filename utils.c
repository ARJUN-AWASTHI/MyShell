#include "shell.h"

void printBanner() {
  p("\n\n");
  p(" $$$$$$\\   $$\\   $$\\  $$$$$$$$\\  $$\\        $$\\   \n"
    "$$  __$$\\  $$ |  $$ | $$  _____| $$ |       $$ | \n"
    "$$ /  \\_|  $$ |  $$ | $$ |       $$ |       $$ |   \n"
    "\\$$$$$$\\   $$$$$$$$ | $$$$$\\     $$ |       $$ |     \n"
    "\\____$$\\   $$  __$$ | $$  __|    $$ |       $$ |      \n"
    "$$\\   $$|  $$ |  $$ | $$ |       $$ |       $$ |     \n "
    "\\$$$$$$|  $$ |  $$ | $$$$$$$$\\  $$$$$$$$\\  $$$$$$$$\\ \n"
    "\\______/   \\__|  \\__| \\________| \\________| \\________| \n\n");
  p("\n\n");
}

// return one matching directory name per call; strdup() each result
char* dir_generator(const char* text, int state) {
  static DIR* dir;
  static struct dirent* entry;
  static size_t len;

  if (state == 0) {
    // first call: open dir and store length of text
    if (dir) {
      closedir(dir);
      dir = NULL;
    }  // be safe
    dir = opendir(".");
    len = strlen(text);
  }

  if (!dir) return NULL;

  while ((entry = readdir(dir)) != NULL) {
    // skip "." and ".."
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    // Only directories (DT_DIR may be DT_UNKNOWN on some FS; we handle that
    // below)
#ifdef DT_DIR
    if (entry->d_type != DT_DIR && entry->d_type != DT_UNKNOWN) continue;
#endif

    if (strncmp(entry->d_name, text, len) == 0) {
      return strdup(entry->d_name);
    }
  }

  closedir(dir);
  dir = NULL;
  return NULL;
}

char** my_completion(const char* text, int start, int end) {
  // don't attempt completion for empty input
  rl_attempted_completion_over = 0;

  // If first word (start == 0): suggest commands / executables
  if (start == 0) {
    // Use command list first (you can also combine with PATH lookup)
    return NULL;
  }

  // If the line begins with "cd " (first 3 chars are 'c','d',' ')
  // then complete only directories
  if (start >= 3 && strncmp(rl_line_buffer, "cd ", 3) == 0) {
    return rl_completion_matches(text, dir_generator);
  }

  // Default: let readline complete filenames (files+dirs)
  return rl_completion_matches(text, rl_filename_completion_function);
}
