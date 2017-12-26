#include <Memory_Base.h>

DECLDIR uint64_t Memory_Base_GetMemoryUsage_C(ECS::Memory_Base * mb)
{
	return mb->GetMemoryUsage();
}
