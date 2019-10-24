#ifndef DECLDIR_ECS_C
#if defined DLL_EXPORT_ECS
#define DECLDIR_ECS_C  extern "C" __declspec(dllexport)
#define DECLDIR_ECS __declspec(dllexport)
#else
#define DECLDIR_ECS_C extern "C" __declspec(dllimport)
#define DECLDIR_ECS __declspec(dllimport)
#endif
#endif