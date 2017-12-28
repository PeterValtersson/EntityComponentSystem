#include <Memory_Base.h>

DECLDIR_ECS uint64_t Memory_Base_GetMemoryUsage_C(ECS::Memory_Base * mb)
{
	return mb->GetMemoryUsage();
}

DECLDIR_ECS void Memory_Base_ShrinkToFit_C(ECS::Memory_Base * mb)
{
	mb->ShrinkToFit();
}

DECLDIR_ECS void Memory_Base_WriteToFile_C(ECS::Memory_Base * mb, std::ofstream * file)
{
	mb->WriteToFile(*file);
}

DECLDIR_ECS void Memory_Base_CreateFromFile_C(ECS::Memory_Base * mb, std::ifstream * file)
{
	mb->CreateFromFile(*file);
}

DECLDIR_ECS void Delete_C(ECS::Memory_Base * mb)
{
	delete mb;
}
