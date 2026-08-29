#pragma once
#include <stdlib.h>
#include "Platform.h"

#if defined (PLATFORM_WINDOWS)
#define PLATFORM_PATH_SEPARATOR '\\'
#elif defined (PLATFORM_LINUX)
#define PLATFORM_PATH_SEPARATOR '/'
#endif

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

#define SAFE_DEL_C(POINTER) do{if((POINTER)!=NULL) {free((POINTER));(POINTER)=NULL;}}while(0)

#ifndef MIN
#define MIN(A,B) ((A)<(B)?(A):(B))
#endif

#ifndef MAX
#define MAX(A,B) ((A)>(B)?(A):(B))
#endif

#if !defined (__PRETTY_FUNCTION__)
#if defined(_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#define __PRETTY_FUNCTION__ __func__
#endif
#endif

#if !defined (__FUNCTION__)
#if defined(_MSC_VER)
#define __FUNCTION__ __func__
#else
#define __FUNCTION__ __func__
#endif
#endif

#if !defined (UNUSED)
#define UNUSED(x) (void)(x)
#endif

#if defined(_MSC_VER)
#define strdup _strdup
#define alloca _alloca
#endif

#define ASSERT_FAIL(__CONDITION__) if(!(__CONDITION__)) {printf("ASSERT FAILED! (%s, %u) '%s'\n", __FILE__, __LINE__, #__CONDITION__); exit(-1);}