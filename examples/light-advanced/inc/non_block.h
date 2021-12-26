// https://ostack.cn/?qa=407277/

#ifndef __NON_BLOCK_H__
#define __NON_BLOCK_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

struct termios orig_termios;

void reset_terminal_mode();
void set_conio_terminal_mode();

int kbhit();
int getch();

#endif