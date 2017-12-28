#ifndef DECLDIR_ECS
#if defined DLL_EXPORT_ECS
#define DECLDIR_ECS  extern "C" __declspec(dllexport)
#else
#define DECLDIR_ECS extern "C" __declspec(dllimport)
#endif
#endif