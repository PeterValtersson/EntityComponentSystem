#ifndef _ECS_MEMORY_BASE_H_
#define _ECS_MEMORY_BASE_H_
#include <stdint.h>
#include "DLLExport.h"
#include <fstream>
namespace ECS
{
	class Memory_Base
	{
	public:
		virtual ~Memory_Base() {}
		virtual uint64_t GetMemoryUsage()const noexcept = 0;
		virtual void ShrinkToFit() noexcept = 0;
		virtual void WriteToFile(std::ofstream& file)const = 0;
		virtual void CreateFromFile(std::ifstream& file) = 0;
	protected:
		Memory_Base() {}
	};
}

DECLDIR uint64_t Memory_Base_GetMemoryUsage_C(ECS::Memory_Base* mb);
DECLDIR void Memory_Base_ShrinkToFit_C(ECS::Memory_Base* mb);
DECLDIR void Memory_Base_WriteToFile_C(ECS::Memory_Base* mb, std::ofstream* file);
DECLDIR void Memory_Base_CreateFromFile_C(ECS::Memory_Base* mb, std::ifstream* file);
DECLDIR void Delete_C(ECS::Memory_Base* mb);
#endif