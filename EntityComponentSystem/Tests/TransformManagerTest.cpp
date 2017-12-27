#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\TransformManager_Interface.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>
namespace Tests
{
	TEST_CLASS(TransformManagerTest)
	{
	public:

		TEST_METHOD(CreateAndDestroy)
		{
			auto em = EntityManager_CreateEntityManager_C();
			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			std::vector<Entity> ents;
			ents.resize(10000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Could not create 10000 entities", LINE_INFO());

			for (size_t i = 0; i < ents.size(); i++)
				TransformManager_Create_C(tm, ents[i], {}, {}, {});
			Assert::AreEqual(Manager_Base_GetNumberOfRegisteredEntities(tm), 10000u, L"Could not create transforms", LINE_INFO());
			EntityManager_DestroyMultiple_C(em, (uint32_t*) ents.data(), uint32_t(ents.size()));

			uint32_t count = 0;
			while (Manager_Base_GetNumberOfRegisteredEntities(tm) > 0)
			{
				Manager_Base_Frame(tm);
				count++;
				Assert::AreNotEqual(count, 10000u, L"Entities not destroyed with 10000 calls to Frame", LINE_INFO());
			}

			Delete_C(tm);
			Delete_C(em);
		}

		TEST_METHOD(CreateAndDestroyNow)
		{
			auto em = EntityManager_CreateEntityManager_C();
			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			std::vector<Entity> ents;
			ents.resize(10000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Could not create 10000 entities", LINE_INFO());

			for (size_t i = 0; i < ents.size(); i++)
				TransformManager_Create_C(tm, ents[i], {}, {}, {});
			Assert::AreEqual(Manager_Base_GetNumberOfRegisteredEntities(tm), 10000u, L"Could not create transforms", LINE_INFO());
			EntityManager_DestroyMultipleNow_C(em, (uint32_t*)ents.data(), uint32_t(ents.size()));
		
			Assert::AreEqual(0u, Manager_Base_GetNumberOfRegisteredEntities(tm), L"DestroyNow did not work", LINE_INFO());

			Delete_C(tm);
			Delete_C(em);
		}

		TEST_METHOD(PosRotScale)
		{
			auto em = EntityManager_CreateEntityManager_C();
			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			std::vector<Entity> ents;
			ents.resize(10000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 10000u, L"Could not create 10000 entities", LINE_INFO());

			for (size_t i = 0; i < ents.size(); i++)
				TransformManager_Create_C(tm, ents[i], {float(i)}, {float(i)}, {float(i)});

			Manager_Base_Frame(tm);

			for (size_t i = 0; i < ents.size(); i++)
			{
				Assert::AreEqual(TransformManager_GetPosition_C(tm, ents[i]).x, float(i), L"Position not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetScale_C(tm, ents[i]).x, float(i), L"Scale not the same", LINE_INFO());

				TransformManager_SetPosition_C(tm, ents[i], { float(i + 1) });
				TransformManager_SetRotation_C(tm, ents[i], { float(i + 1) });
				TransformManager_SetScale_C(tm, ents[i], { float(i + 1) });
			}
			Manager_Base_Frame(tm);
			for (size_t i = 0; i < ents.size(); i++)
			{
				Assert::AreEqual(TransformManager_GetPosition_C(tm, ents[i]).x, float(i+1), L"Position not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i+1), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetScale_C(tm, ents[i]).x, float(i+1), L"Scale not the same", LINE_INFO());

			}

			Delete_C(tm);
			Delete_C(em);
		}
	};
}