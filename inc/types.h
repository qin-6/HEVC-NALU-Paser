#ifndef __TYPES_H
#define __TYPES_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef signed int sint32;


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CEIL(a, b) ((a)+(b)-1)/(b)

#endif