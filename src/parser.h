#ifndef PARSER_H_
#define PARSER_H_

#define STR_BUF_SIZE 256

int entered_line_check(char** opt, const char* line, const int line_num);
int parse_use_line(const char* line, const int line_num, void** dll);
int parse_call_line(const char* line, const int line_num, void** dll);
int process_line(const char* line, char* line_wout_slashn, int* line_num,
                 void** dll, const int mode);
int manual_mode();
int getopt(int argc);
int auto_mode(const int argc, char* argv[]);

enum {
  NONE = 0,
  MANUAL,
  AUTO,
};

#endif
