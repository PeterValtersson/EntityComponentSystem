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
				Assert::AreNotEqual(count, 33u, L"Entities not destroyed with 33 calls to Frame", LINE_INFO());
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
				//Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetScale_C(tm, ents[i]).x, float(i), L"Scale not the same", LINE_INFO());

				TransformManager_SetPosition_C(tm, ents[i], { float(i + 1) });
				TransformManager_SetRotation_C(tm, ents[i], { float(i + 1) });
				TransformManager_SetScale_C(tm, ents[i], { float(i + 1) });
			}
			Manager_Base_Frame(tm);
			for (size_t i = 0; i < ents.size(); i++)
			{
				Assert::AreEqual(TransformManager_GetPosition_C(tm, ents[i]).x, float(i+1), L"Position not the same", LINE_INFO());
				//Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i+1), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual(TransformManager_GetScale_C(tm, ents[i]).x, float(i+1), L"Scale not the same", LINE_INFO());

			}

			Delete_C(tm);
			Delete_C(em);
		}
	
		TEST_METHOD(ParentChild)
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
				TransformManager_Create_C(tm, ents[i], { float(i) }, { float(i) }, { float(i) });

			Manager_Base_Frame(tm);

			for (size_t i = 1; i < 1001; i++)
				TransformManager_BindChild_C(tm, ents[0], ents[i], 0);

			Assert::AreEqual(1000u, TransformManager_GetNumberOfChildren_C(tm, ents[0]), L"Entity did not have 1000 children", LINE_INFO());
			Entity children[10000];
			TransformManager_GetChildren_C(tm, ents[0], (uint32_t*)children);
			for (uint32_t i = 0; i < TransformManager_GetNumberOfChildren_C(tm, ents[0]); i++)
			{
				Assert::AreEqual((uint32_t)ents[i+1], (uint32_t)children[i], L"A child was not correct", LINE_INFO());
			}
			
			TransformManager_UnbindParent_C(tm, children[999], 0);
			Assert::AreEqual(999u, TransformManager_GetNumberOfChildren_C(tm, ents[0]), L"Entity did not have 999 children", LINE_INFO());

			TransformManager_GetChildren_C(tm, ents[0], (uint32_t*)children);
			for (uint32_t i = 0; i < TransformManager_GetNumberOfChildren_C(tm, ents[0]); i++)
			{
				Assert::AreEqual((uint32_t)ents[i + 1], (uint32_t)children[i], L"A child was not correct", LINE_INFO());
			}

			TransformManager_UnbindParent_C(tm, children[0], 0);
			Assert::AreEqual(998u, TransformManager_GetNumberOfChildren_C(tm, ents[0]), L"Entity did not have 998 children", LINE_INFO());
			TransformManager_GetChildren_C(tm, ents[0], (uint32_t*)children);
			for (uint32_t i = 0; i < TransformManager_GetNumberOfChildren_C(tm, ents[0]); i++)
			{
				Assert::AreEqual((uint32_t)ents[i + 2], (uint32_t)children[i], L"A child was not correct", LINE_INFO());
			}

			TransformManager_UnbindParent_C(tm, children[200], 0);
			Assert::AreEqual(997u, TransformManager_GetNumberOfChildren_C(tm, ents[0]), L"Entity did not have 998 children", LINE_INFO());
			TransformManager_GetChildren_C(tm, ents[0], (uint32_t*)children);
			uint32_t add = 2;
			for (uint32_t i = 0; i < TransformManager_GetNumberOfChildren_C(tm, ents[0]); i++)
			{
				if (i == 200)
					add++;
				Assert::AreEqual((uint32_t)ents[i + add], (uint32_t)children[i], L"A child was not correct", LINE_INFO());
			}

			TransformManager_UnbindAllChildren_C(tm, ents[0], 0);
			Assert::AreEqual(0u, TransformManager_GetNumberOfChildren_C(tm, ents[0]), L"Entity did not have 0 children", LINE_INFO());


			Delete_C(tm);
			Delete_C(em);
		}
		bool MatE(const Matrix& a, const Matrix& b)
		{
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					if (std::abs(a.m[y][x] - b.m[y][x]) > 0.001f)
						return false;
		return true;
		}
		TEST_METHOD(InheritedTransforms)
		{

			auto em = EntityManager_CreateEntityManager_C();
			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			std::vector<Entity> ents;
			ents.resize(1000);
			EntityManager_CreateMultiple_C(em, (uint32_t*)ents.data(), (uint32_t)ents.size());
			Assert::AreEqual(EntityManager_GetNumberOfAliveEntities_C(em), 1000u, L"Could not create 10000 entities", LINE_INFO());

			for (size_t i = 0; i < ents.size(); i++)
				TransformManager_Create_C(tm, ents[i], {}, {}, {1.0f,1.0f,1.0f});

			Manager_Base_Frame(tm);

			Entity parentEnt = EntityManager_Create_C(em);
			TransformManager_Create_C(tm, parentEnt, {}, {}, {1.0f, 1.0f, 1.0f});

			for (size_t i = 0; i < ents.size(); i++)
				TransformManager_BindChild_C(tm, parentEnt, ents[i], 0);

			Assert::AreEqual(1000u, TransformManager_GetNumberOfChildren_C(tm, parentEnt), L"Entity did not have 1000 children", LINE_INFO());
			Manager_Base_Frame(tm);

			auto parentTransform = TransformManager_GetTransform_C(tm, parentEnt);
			for (size_t i = 0; i < ents.size(); i++)
				Assert::IsTrue(MatE(parentTransform, TransformManager_GetTransform_C(tm, ents[i])), L"Transforms not equal", LINE_INFO());

			Entity parentEnt2 = EntityManager_Create_C(em);
			TransformManager_Create_C(tm, parentEnt2, { 1.0f }, {1.0f}, {2.0f, 2.0f, 2.0f});
			TransformManager_BindChild_C(tm, parentEnt2, parentEnt, 0);
			//TransformManager_SetPosition_C(tm, parentEnt, { 1.0f });

			Manager_Base_Frame(tm);

			parentTransform = TransformManager_GetTransform_C(tm, parentEnt2);
			auto parentTransform2 = TransformManager_GetTransform_C(tm, parentEnt);
			Assert::IsTrue(MatE(parentTransform, parentTransform2), L"Transforms not equal", LINE_INFO());

			for (size_t i = 0; i < ents.size(); i++)
				Assert::IsTrue(MatE(parentTransform, TransformManager_GetTransform_C(tm, ents[i])), L"Transforms not equal", LINE_INFO());

			Delete_C(tm);
			Delete_C(em);
		}
	};
}