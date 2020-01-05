#ifndef _ECS_MEMORY_BASE_H_
#define _ECS_MEMORY_BASE_H_
#include <stdint.h>
#include "DLLExport.h"
#include <fstream>
#include <memory>

namespace ECS
{
	class Memory_Base
	{
	public:
		virtual ~Memory_Base() {}
		virtual size_t get_memory_usage()const noexcept = 0;
		virtual void shrink_to_fit() = 0;
		virtual void write_to_stream(std::ostream& stream)const = 0;
		virtual void create_from_stream(std::istream& stream) = 0;
	protected:
		Memory_Base() {}
	};
}/*

DECLDIR_ECS_C size_t Memory_Base_GetMemoryUsage_C(ECS::Memory_Base* mb);
DECLDIR_ECS_C void Memory_Base_ShrinkToFit_C(ECS::Memory_Base* mb);
DECLDIR_ECS_C void Memory_Base_WriteToFile_C(ECS::Memory_Base* mb, std::ofstream* file);
DECLDIR_ECS_C void Memory_Base_CreateFromFile_C(ECS::Memory_Base* mb, std::ifstream* file);
DECLDIR_ECS_C void Delete_C(ECS::Memory_Base* mb);*/
#endif