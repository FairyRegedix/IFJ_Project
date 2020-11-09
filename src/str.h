#ifndef IFJ_PROJECT_STR_H
#define IFJ_PROJECT_STR_H

#define BLOCK_SIZE 8

typedef struct {
    char* str;
    size_t len;
    size_t size;
}string;

int str_init(string *s);
void str_reinit(string *s);
void str_free(string *s);
int str_add_char(string *s1, char c);
int str_concat(string *s1, const char* s2);
int str_copy(string *s1, const string *s2);
int str_cmp(const string *s1, const string *s2);
int str_cmp_c_str(const string *s1, const char* s2);
char* str_to_c_str(const string *s);
string* c_str_to_str(const char *s);
size_t str_len(const string *s);

#endif //IFJ_PROJECT_STR_H