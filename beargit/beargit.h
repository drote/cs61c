int beargit_init(void);
int beargit_add(const char* filename);
int beargit_rm(const char* filename);
int beargit_commit(const char* message);
int beargit_status();
int beargit_log();
int beargit_branch();
int beargit_checkout(const char* arg, int new_branch);

// Helper functions
int get_branch_number(const char* branch_name);
void next_commit_id(char* commit_id);

// Number of bytes in a commit id
#define COMMIT_ID_BYTES 40
#define COMMIT_FOLDER_SIZE 50
#define MAX_COMMIT_ID 205891132094648 // 3 ^ 30 - 1
// Preprocessor macros capturing the maximum size of different  structures
#define FILENAME_SIZE 512
#define COMMIT_ID_SIZE (COMMIT_ID_BYTES+1)
#define MSG_SIZE 512

#define NULL_ID_FULL "0000000000000000000000000000000000000000"
#define NULL_ID_PART "000000000000000000000000000000"
#define INIT_ID "666666666666666666666666666661"
#define START_TWO_VAL 137260754729766 //"c1111..."

#define INDEX_FILE ".beargit/.index"
#define TEMP_INDEX ".beargit/.index_tmp"
#define PREV_FILE ".beargit/.prev"
#define BRANCHNAME_SIZE 128
#define COMMIT_ID_BRANCH_BYTES 10

// 3 ^ 2  2 ^ 4
       
//  0200  1 0000

//  162 54 18 6 2
//  1 * 2 // 3 * 2 // 3 * 3 * 2 // 3 * 3 * 3 * 2