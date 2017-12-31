#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\SceneManager_Interface.h>
#include <Managers\TransformManager_Interface.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>
namespace Tests
{
	TEST_CLASS(SceneManagerTest)
	{
	public:

		TEST_METHOD(SceneManager_CreateDestroy)
		{
			auto em = EntityManager_CreateEntityManager_C();

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType(sm));

			Entity scene = em->Create();
			SceneManager_Create_C(sm, scene, "World");
			Assert::IsTrue(Manager_Base_IsRegistered_C(sm, scene));
			Assert::AreEqual("World", SceneManager_GetNameOfScene(sm, scene));

			auto ent = em->Create();
			SceneManager_AddNamedEntityToScene_C(sm, scene, ent, "Char");
			Assert::AreEqual(1u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			Assert::AreEqual("Char", SceneManager_GetNameOfEntityInScene(sm, scene, ent));

			SceneManager_RemoveEntityFromScene(sm, scene, ent);
			Assert::AreEqual(0u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			Assert::IsTrue(SceneManager_GetNameOfEntityInScene(sm, scene, ent) == nullptr);

			Manager_Base_Destroy_C(sm, scene);
			Assert::AreEqual(0u, Manager_Base_GetNumberOfRegisteredEntities(sm));

			Delete_C(sm);
			Delete_C(em);
		}

		TEST_METHOD(SceneManager_GetEntitiesInScene)
		{
			auto em = EntityManager_CreateEntityManager_C();

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType(sm));

			Entity scene = em->Create();
			SceneManager_Create_C(sm, scene, "World");
			Assert::IsTrue(Manager_Base_IsRegistered_C(sm, scene));
			Assert::AreEqual("World", SceneManager_GetNameOfScene(sm, scene));

			std::vector<Entity> ents(1000);
			em->CreateMultiple(ents.data(), uint32_t(ents.size()));
			for (size_t i = 0; i < ents.size(); i++)
			{
				SceneManager_AddNamedEntityToScene_C(sm, scene, ents[i], ("Ent" + std::to_string(i)).c_str());
				Assert::AreEqual(uint32_t(i+1), SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
				Assert::AreEqual(("Ent" + std::to_string(i)).c_str(), SceneManager_GetNameOfEntityInScene(sm, scene, ents[i]));
			}
			
			Assert::AreEqual(1000u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			std::vector<Entity> getEnts(ents.size());
			SceneManager_GetEntitiesInScene_C(sm, scene, (uint32_t*)getEnts.data());
			for (size_t i = 0; i < ents.size(); i++)
			{
				Assert::AreEqual(uint32_t(ents[i]), uint32_t(getEnts[i]));
				Assert::AreEqual(("Ent" + std::to_string(i)).c_str(), SceneManager_GetNameOfEntityInScene(sm, scene, getEnts[i]));
			}

			Manager_Base_Destroy_C(sm, scene);
			Assert::AreEqual(0u, Manager_Base_GetNumberOfRegisteredEntities(sm));
			Assert::AreEqual(0u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));

			Delete_C(sm);
			Delete_C(em);
		}

		TEST_METHOD(SceneManager_WriteReadComponent)
		{
			auto em = EntityManager_CreateEntityManager_C();

			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType(tm), Utilz::GUID("TransformManager").id);

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType(sm));
			SceneManager_RegisterManager_C(sm, tm);

			Entity scene = em->Create();
			SceneManager_Create_C(sm, scene, "World");
	
			std::function<bool(std::ostream* file)> writer;
			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			std::stringstream ss;
			Assert::IsTrue(writer(&ss));

			auto ent = em->Create();
			sm->AddEntityToScene(scene, ent, "Ent");

			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			ss.seekp(0);
			Assert::IsTrue(writer(&ss));

			tm->Create(scene, { 1.0f });

			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			ss.seekp(0);
			Assert::IsTrue(writer(&ss));
			
			tm->Create(ent, { 0.0f, 1.0f });

			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			ss.seekp(0);
			Assert::IsTrue(writer(&ss));

			Assert::AreEqual(1u, sm->GetNumberOfRegisteredEntities());
			Assert::AreEqual(2u, tm->GetNumberOfRegisteredEntities());

			auto newScene = em->Create();
			ss.seekg(0);
			sm->CreateFromStream(newScene, &ss);
			Assert::AreEqual(2u, sm->GetNumberOfRegisteredEntities());
			Assert::AreEqual(4u, tm->GetNumberOfRegisteredEntities());

			Delete_C(sm);
			Delete_C(em);
		}
	};
}