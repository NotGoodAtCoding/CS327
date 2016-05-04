#ifndef HEADER_H
# define HEADER_H

# ifdef __cplusplus
extern "C" {
# endif

void cpp_function(const char *s);
void *get_cout(void);
void use_cout(void *ptr);

# ifdef __cplusplus
}
# endif

#endif
