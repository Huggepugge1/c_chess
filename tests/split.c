#include "../src/uci.h"
#include "test_framework.h"

TEST(split_empty) {
    char *string = strdup("   ");
    Split *command = split(string);
    ASSERT_EQ(command->len, 0);
    free_split(command);
    free(string);
}

TEST(split) {
    char *string = strdup("This is  a test    string");
    Split *command = split(string);
    ASSERT_STRING_EQ(command->strings[0], "This");
    ASSERT_STRING_EQ(command->strings[1], "is");
    ASSERT_STRING_EQ(command->strings[2], "a");
    ASSERT_STRING_EQ(command->strings[3], "test");
    ASSERT_STRING_EQ(command->strings[4], "string");
    free_split(command);
    free(string);
}
