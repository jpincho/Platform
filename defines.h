#pragma once
#include <stdlib.h>

#ifdef __cplusplus
#define BEGIN_C_DECLARATIONS extern "C" {
#define END_C_DECLARATIONS }
#else
#define BEGIN_C_DECLARATIONS
#define END_C_DECLARATIONS
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

#define SAFE_DEL_C(POINTER) {if((POINTER)!=NULL) {free((POINTER));(POINTER)=NULL;}}

#ifndef MIN
#define MIN(A,B) ((A)<(B)?(A):(B))
#endif

#ifndef MAX
#define MAX(A,B) ((A)>(B)?(A):(B))
#endif

#if !defined (__PRETTY_FUNCTION__)
#if defined(_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#endif

#if !defined (__FUNCTION__)
#if defined(_MSC_VER)
#define __FUNCTION__ __func__
#endif
#endif

#if !defined (UNUSED)
#define UNUSED(x) (void)(x)
#endif

#if defined(_MSC_VER)
#define strdup _strdup
#endif
