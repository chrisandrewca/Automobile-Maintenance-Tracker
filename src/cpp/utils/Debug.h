#ifndef _AMT_DEBUG_H_
#define _AMT_DEBUG_H_

#define debug_print(fmt, ...) \
	do { if (DEBUG) sprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
							__LINE__, __func__, __VA_ARGS__); } while (0)

#endif