#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <math.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

const char* digits = "61c";

// UTILITY FUNCTIONS ->

/*
 * Converts c61 digits ('c', '6', or '1') to a integer digit value (0, 1, 2)
 */
int c61_digit_to_int(char d) {
  switch (d) {
    case '6': return 0;
    case '1': return 1;
    case 'c': return 2;
    default:
      fprintf(stderr, "invalid tern digit encountered");
      return -1;
  }
}

/*
 * Converts integer digit value (0, 1, 2) to a c61 digit ('c', '6', or '1')
 */
char int_to_c61_digit(int i) {
  switch (i) {
    case 0: return '6';
    case 1: return '1';
    case 2: return 'c';
    default:
      fprintf(stderr, "invalid tern int encountered");
      return -1;
  }
}

/*
 * Converts a ternary c61 number into a decimal value. unsgined long value to accomodate for a ternay number of up to 30 digits
 */
unsigned long ttoi(char t_number[31]) {
  unsigned long dec; // can hold 3 ^ 30
  int idx, d;

  for (dec = idx = 0; (d = t_number[idx]) != '\0'; ++idx) {
    dec = (dec * 3) + c61_digit_to_int(d); // assuming correct responses
  }

  return dec;
}

/*
 * Converts a decimal value into a c61 number and stores it in the desc array. Allows for numbers of up to 30 digits
 */
void itot(char dest[31], unsigned long dec) {
  char new_id[31] = "666666666666666666666666666666";
  unsigned long long one, two;
  int i;

  for (i = 0, two = START_TWO_VAL, one = two / 2; dec > 0; ++i, one /= 3, two /= 3) {
    if (dec >= two) {
      new_id[i] = int_to_c61_digit(2);
      dec -= two;
    } else if (dec >= one) {
      new_id[i] = int_to_c61_digit(1);
      dec -= one;
    }
  }

  strcpy(dest, new_id);
}

/*
 * Checks commit message is valid (containes the exact string "GO BEARS1!""
 */

const char* go_bears = "GO BEARS!";

int is_commit_msg_ok(const char* msg) {
  int msg_c, bears_c, bears_idx, msg_idx;

  for (bears_idx = msg_idx = 0; bears_c = go_bears[bears_idx], (msg_c = msg[msg_idx]) != '\0'; msg_idx++) {  
    if (bears_c == '\0') {
      // entire string matched
      return 1;
    } else if (msg_c == bears_c) {
      bears_idx++;
    } else {
      bears_idx = 0;
    }
  }

  return bears_c == msg_c; // string found at end;
}

/*
 * Gets the next 30 digits ternary c61 ID in a sequential order (1..MAX_COMMIT_ID) 
 * With MAX_COMMIT_ID being the largest number that can be represented in this format (3 ^ 30 - 1)
 * Numbers are padded with 1s (that represent ternary 0s)
 * ex: 1 == "...16" | 2 == "...1c" | 3 == "...161" etc.. 
 */
void next_commit_id_hw1(char commit_id[31]) {
  unsigned long id_int;
  char new_id[31] = INIT_ID;

  if (strcmp(commit_id, NULL_ID_PART) == 0) {
    strcpy(commit_id, new_id);
    return;
  }

  id_int = ttoi(commit_id);

  if (id_int == MAX_COMMIT_ID) {
    strcpy(commit_id, new_id);
    return;
  }

  itot(commit_id, ++id_int);
}

void next_commit_id(char* commit_id) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // The first COMMIT_ID_BRANCH_BYTES=10 characters of the commit ID will
  // be used to encode the current branch number. This is necessary to avoid
  // duplicate IDs in different branches, as they can have the same pre-
  // decessor (so next_commit_id has to depend on something else).
  int n = get_branch_number(current_branch);
  for (int i = 0; i < COMMIT_ID_BRANCH_BYTES; i++) {
    // This translates the branch number into base 3 and substitutes 0 for
    // 6, 1 for 1 and c for 2.
    commit_id[i] = digits[n%3];
    n /= 3;
  }

  // Use next_commit_id to fill in the rest of the commit ID.
  next_commit_id_hw1(commit_id + COMMIT_ID_BRANCH_BYTES);
}

// BEARGIT METHODS ->

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(INDEX_FILE, "w");
  fclose(findex);

  FILE* fbranches = fopen(".beargit/.branches", "w");
  fprintf(fbranches, "%s\n", "master");
  fclose(fbranches);
   
  write_string_to_file(PREV_FILE, NULL_ID_FULL); 
  write_string_to_file(".beargit/.current_branch", "master");

  return 0;
}

/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(INDEX_FILE, "r");
  FILE *fnewindex = fopen(TEMP_INDEX, "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(TEMP_INDEX);
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(TEMP_INDEX, INDEX_FILE);

  return 0;
}

/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char* filename) {
  FILE* findex = fopen(INDEX_FILE, "r");
  FILE *fnewindex = fopen(TEMP_INDEX, "w");

  char file_removed = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
  
    if (strcmp(line, filename) == 0) {
      file_removed = 1;
    } else {
      fprintf(fnewindex, "%s\n", line);
    }
  }

  fclose(findex);
  fclose(fnewindex);

  if (!file_removed) {
    fprintf(stderr, "ERROR: File %s not tracked\n", filename);
    fs_rm(TEMP_INDEX);
    return 1;
  }

  fs_mv(TEMP_INDEX, INDEX_FILE);

  return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */


int beargit_commit_hw1(const char* msg) {

  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  char new_folder[COMMIT_FOLDER_SIZE];
  char msg_file[FILENAME_SIZE];
  char new_index_file[FILENAME_SIZE];
  char new_prev_file[FILENAME_SIZE];
  char file_buffer[FILENAME_SIZE];
  char old_file_buffer[FILENAME_SIZE];

  FILE* findex = fopen(INDEX_FILE, "r");


  // get previous commit_id. store it in the commit_id variable
  read_string_from_file(PREV_FILE, commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);

  // create new commit folder and store a pointer to it in the new_folder variable
  sprintf(new_folder, ".beargit/%s", commit_id);
  fs_mkdir(new_folder);

  // create commit message file
  sprintf(msg_file, "%s/.msg", new_folder);
  
  FILE* fmsg = fopen(msg_file, "w");
  fclose(fmsg);
  write_string_to_file(msg_file, msg);

  // copy dotfiles
  sprintf(new_index_file, "%s/.index", new_folder);
  fs_cp(INDEX_FILE, new_index_file);

  sprintf(new_prev_file, "%s/.prev", new_folder);
  fs_cp(PREV_FILE, new_prev_file);

  // copy files in index
  while(fgets(old_file_buffer, sizeof(old_file_buffer), findex)) {
    strtok(old_file_buffer, "\n");
    sprintf(file_buffer, "%s/%s", new_folder, old_file_buffer);
    fs_cp(old_file_buffer, file_buffer);
  }

  // write commit_id to current .prev file
  write_string_to_file(PREV_FILE, commit_id);

  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {
  FILE* findex = fopen(INDEX_FILE, "r");
  unsigned int fileN = 0;

  fprintf(stdout, "Tracked files:\n\n");

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    fprintf(stdout, "  %s\n", line);
    ++fileN;
  }

  fprintf(stdout, "\n%d files total\n", fileN);
  /* COMPLETE THE REST */

  return 0;
}

/*
 * beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */
int beargit_log() {
  char commit_folder[COMMIT_FOLDER_SIZE];
  char commit_id[COMMIT_ID_SIZE];
  char msg_file[FILENAME_SIZE];
  char prev_file[FILENAME_SIZE];
  char msg[MSG_SIZE];
  char commit_count = 0;

  read_string_from_file(PREV_FILE, commit_id, COMMIT_ID_SIZE);
  
  while (strcmp(commit_id, NULL_ID_FULL) != 0) {
    sprintf(msg_file, ".beargit/%s/.msg", commit_id);
    sprintf(prev_file, ".beargit/%s/.prev", commit_id);
  
    read_string_from_file(msg_file, msg, MSG_SIZE);
    fprintf(stdout, "\ncommit %s\n    %s\n", commit_id, msg);
    
    read_string_from_file(prev_file, commit_id, COMMIT_ID_SIZE);
    ++commit_count;
  }

  if (!commit_count) {
    // no commits yet
    fprintf(stderr, "ERROR: There are no commits!\n");
    return 1;
  }

  fprintf(stdout, "\n");

  return 0;
}

// ---------------------------------------
// HOMEWORK 2 
// ---------------------------------------

// This adds a check to beargit_commit that ensures we are on the HEAD of a branch.
int beargit_commit(const char* msg) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  if (strlen(current_branch) == 0) {
    fprintf(stderr, "ERROR: Need to be on HEAD of a branch to commit\n");
    return 1;
  }

  return beargit_commit_hw1(msg);
}

// This helper function returns the branch number for a specific branch, or
// returns -1 if the branch does not exist.
int get_branch_number(const char* branch_name) {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  int branch_index = -1;
  int counter = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, branch_name) == 0) {
      branch_index = counter;
    }
    counter++;
  }

  fclose(fbranches);

  return branch_index;
}

/* beargit branch
 *
 * See "Step 6" in the homework 1 spec.
 *
 */

int beargit_branch() {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  char branch[BRANCHNAME_SIZE];
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  while (fgets(branch, sizeof(branch), fbranches)) {
    strtok(branch, "\n");
    if (strcmp(current_branch, branch) == 0) {
      fprintf(stdout, "*");
    } else {
      fprintf(stdout, " ");
    }
    
    fprintf(stdout, " %s\n", branch);
  }

  fclose(fbranches);

  return 0;
}

/* beargit checkout
 *
 * See "Step 7" in the homework 1 spec.
 *
 */

int checkout_commit(const char* commit_id) {
  /* DELETE CURRENTLY TRACKED FILES */ 
  FILE* findex = fopen(INDEX_FILE, "r");

  char filename[FILENAME_SIZE];
  while (fgets(filename, sizeof(filename), findex)) {
    strtok(filename, "\n");
    fs_rm(filename);
  }

  fclose(findex);

  write_string_to_file(".beargit/new_prev", commit_id);
  fs_mv(".beargit/new_prev", PREV_FILE);

  /* IF COMMIT IS NULL JUST CREATE A NEW INDEX FILE AND EARLY OUT */ 

  if (strcmp(commit_id, NULL_ID_FULL) == 0) {
    FILE* new_index = fopen(".beargit/new_index", "w");
    fclose(new_index);

    fs_mv(".beargit/new_index", INDEX_FILE);
    return 0;
  };


  /* COPY INDEX FILE AND ALL THE CHECKED OUT COMMIT'S TRACKED FILES */
  char commit_folder[FILENAME_SIZE];
  char commit_index[FILENAME_SIZE];
  char commit_filename[FILENAME_SIZE];

  sprintf(commit_folder, ".beargit/%s", commit_id);
  sprintf(commit_index, "%s/.index", commit_folder);
  
  fs_cp(commit_index, INDEX_FILE);

  findex = fopen(INDEX_FILE, "r");

  while (fgets(filename, sizeof(filename), findex)) {
    strtok(filename, "\n");
    sprintf(commit_filename, "%s/%s", commit_folder, filename);
    fs_cp(commit_filename, filename);
  }

  fclose(findex);

  return 0;
}

int is_it_a_commit_id(const char* commit_id) {
  int i, c, len;

  for (i = len = 0; (c = commit_id[i]) != '\0'; ++i, ++len) {
    if (c != '6' && c != '1' && c != 'c' || len > 40) {
      return 0;
    }
  }

  return len == 40;
}

int beargit_checkout(const char* arg, int new_branch) {
  // Get the current branch
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // If not detached, update the current branch by storing the current HEAD into that branch's file...
  // Even if we cancel later, this is still ok.
  if (strlen(current_branch)) {
    char current_branch_file[BRANCHNAME_SIZE+50];
    sprintf(current_branch_file, ".beargit/.branch_%s", current_branch);
    fs_cp(".beargit/.prev", current_branch_file);
  }

  // Check whether the argument is a commit ID. If yes, we just stay in detached mode
  // without actually having to change into any other branch.
  if (is_it_a_commit_id(arg)) {
    char commit_dir[FILENAME_SIZE] = ".beargit/";
    strcat(commit_dir, arg);
    if (!fs_check_dir_exists(commit_dir)) {
      fprintf(stderr, "ERROR: Commit %s does not exist\n", arg);
      return 1;
    }

    // Set the current branch to none (i.e., detached).
    write_string_to_file(".beargit/.current_branch", "");

    return checkout_commit(arg);
  }

  // Just a better name, since we now know the argument is a branch name.
  const char* branch_name = arg;

  // Read branches file (giving us the HEAD commit id for that branch).
  int branch_exists = (get_branch_number(branch_name) >= 0);

  // Check for errors.
  if (branch_exists && new_branch) {
    fprintf(stderr, "ERROR: A branch named %s already exists\n", branch_name);
    return 1;
  } else if (!branch_exists && !new_branch) {
    fprintf(stderr, "ERROR: No branch %s exists\n", branch_name);
    return 1;
  }

  // File for the branch we are changing into.
  char branch_file[FILENAME_SIZE] = ".beargit/.branch_";
  strcat(branch_file, branch_name);

  // Update the branch file if new branch is created (now it can't go wrong anymore)
  if (new_branch) {
    FILE* fbranches = fopen(".beargit/.branches", "a");
    fprintf(fbranches, "%s\n", branch_name);
    fclose(fbranches);
    fs_cp(".beargit/.prev", branch_file); 
  }

  write_string_to_file(".beargit/.current_branch", branch_name);

  // Read the head commit ID of this branch.
  char branch_head_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(branch_file, branch_head_commit_id, COMMIT_ID_SIZE);

  // Check out the actual commit.
  return checkout_commit(branch_head_commit_id);
}
