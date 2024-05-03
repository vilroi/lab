#ifndef		__ERROR_H

#include <stdio.h>
#include <err.h>

void error_exit(char *,...);
void error(char *,...);

#define ERROR_RET(retval,...) 				\
do {										\
	fprintf(stderr, "%s(): ", __func__);	\
	error(__VA_ARGS__);						\
	return retval;							\
} while (0); 								\

#define ERROR_EXIT(...) 					\
do {										\
	fprintf(stderr, "%s(): ", __func__);	\
	error_exit(__VA_ARGS__);					\
} while (0); 								\

#define ERR_EXIT(...) 						\
do {										\
	fprintf(stderr, "%s(): ", __func__);	\
	err(1, __VA_ARGS__);					\
} while (0); 								\

#endif
