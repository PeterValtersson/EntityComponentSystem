#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>
namespace Tests
{		
	TEST_CLASS(EntityManagerTest)
	{
	public:
		
		TEST_METHOD(EntityManager_CreateAndDestroy)
		{
			auto em = EntityManager_CreateEntityManager_C();
			std::vector<Entity> ents;
			for (int i = 0; i < 10000; i++)
			{
				ents.push_back(EntityManager_Create_C(em));
			}
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em),10000u, L"Could not create 10000 entities", LINE_INFO());
			int j = 0;
			for (; j < 5000; j++)
			{
				EntityManager_Destroy_C(em, ents[j]);
			}
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 5000u, L"Not 5000 alive left", LINE_INFO());

			EntityManager_DestroyMultiple_C(em, (uint32_t*)&ents[j], 5000u);
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 0u, L"Not 0 alive left", LINE_INFO());
		
			Delete_C(em);
		}
		TEST_METHOD(EntityManager_CreateAndDestroyAll)
		{
			auto em = EntityManager_CreateEntityManager_C();
			std::vector<Entity> ents;
			ents.resize(10000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t) ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Could not create 10000 entities", LINE_INFO());
			EntityManager_DestroyMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 0u, L"Could not destroy all entities", LINE_INFO());

			Delete_C(em);

		}

		TEST_METHOD(EntityManager_FileExport)
		{
			auto em = EntityManager_CreateEntityManager_C();
			std::vector<Entity> ents;
			ents.resize(10000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Could not create 10000 entities", LINE_INFO());
			
			
			std::ofstream fileout("file.test", std::ios::binary);
			Assert::IsTrue(fileout.is_open(), L"File not open", LINE_INFO());

			auto usage = Memory_Base_GetMemoryUsage_C(em);
			Memory_Base_ShrinkToFit_C(em);
			Assert::IsTrue(Memory_Base_GetMemoryUsage_C(em) <= usage, L"Shink failed", LINE_INFO());
			Memory_Base_WriteToFile_C(em, &fileout);
			fileout.close();

			Delete_C(em);

			em = EntityManager_CreateEntityManager_C();
			std::ifstream filein("file.test", std::ios::binary);
			Assert::IsTrue(filein.is_open(), L"Could not open file", LINE_INFO());
			Memory_Base_CreateFromFile_C(em, &filein);
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Not 10000 entities alive", LINE_INFO());
			
			
			EntityManager_DestroyMultiple_C(em, (uint32_t*)ents.data(),(uint32_t) ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 0u, L"Could not destroy all entities", LINE_INFO());


			Delete_C(em);
		}
	};
}