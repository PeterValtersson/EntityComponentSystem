#include <Memory_Base.h>

DECLDIR uint64_t Memory_Base_GetMemoryUsage_C(ECS::Memory_Base * mb)
{
	return mb->GetMemoryUsage();
}

DECLDIR void Memory_Base_ShrinkToFit_C(ECS::Memory_Base * mb)
{
	mb->ShrinkToFit();
}

DECLDIR void Memory_Base_WriteToFile_C(ECS::Memory_Base * mb, std::ofstream * file)
{
	mb->WriteToFile(*file);
}

DECLDIR void Memory_Base_CreateFromFile_C(ECS::Memory_Base * mb, std::ifstream * file)
{
	mb->CreateFromFile(*file);
}

DECLDIR void Delete_C(ECS::Memory_Base * mb)
{
	delete mb;
}
