#if defined DLL_EXPORT_ECS
#define DECLDIR  extern "C" __declspec(dllexport)
#else
#define DECLDIR extern "C" __declspec(dllimport)
#endif