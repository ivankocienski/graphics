
#ifndef IK_UTIL_INC
#define IK_UTIL_INC

#include <stdlib.h>

typedef unsigned char byte;
typedef byte* pbyte;

inline double frand() {
  return (double)rand() / (double)RAND_MAX;
}

#endif // IK_UTIL_INC
