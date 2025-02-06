#include "test_framework.h"

#include "split.c"
#include "chess_engine.c"
#include "fen.c"

#include <stdio.h>
#include <setjmp.h>

extern jmp_buf jmpbuf;
extern char *error_msg;

struct Test tests[] = {
{ "split_empty", test_split_empty },
{ "split", test_split },
{ "chess_engine_allocation", test_chess_engine_allocation },
{ "load_startpos", test_load_startpos },
{ "load_fen", test_load_fen },
};

int main() {
for (int i = 0; i < 5; i++) {
if (setjmp(jmpbuf) == 0) {
tests[i].test();
printf("Test `%s` passed\n", tests[i].name);
} else {
printf("Test `%s` failed: %s\n", tests[i].name, error_msg);
}
}
return 0;
}
