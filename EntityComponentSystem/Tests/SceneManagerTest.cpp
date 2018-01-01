#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\SceneManager_Interface.h>
#include <ResourceHandler\ResourceHandler_Interface.h>
#include <Managers\TransformManager_Interface.h>
#include <filesystem>
namespace fs = std::experimental::filesystem;
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

			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilz::GUID("TransformManager").id);

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType_C(sm));

			Entity scene = em->Create();
			SceneManager_Create_C(sm, scene, "World");
			Assert::IsTrue(Manager_Base_IsRegistered_C(sm, scene));
			Assert::AreEqual("World", SceneManager_GetNameOfScene_C(sm, scene));

			auto ent = em->Create();
			SceneManager_AddNamedEntityToScene_C(sm, scene, ent, "Char");
			Assert::AreEqual(1u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			Assert::AreEqual("Char", SceneManager_GetNameOfEntityInScene_C(sm, scene, ent));

			SceneManager_RemoveEntityFromScene_C(sm, scene, ent);
			Assert::AreEqual(0u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			Assert::IsTrue(SceneManager_GetNameOfEntityInScene_C(sm, scene, ent) == nullptr);

			Manager_Base_Destroy_C(sm, scene);
			Assert::AreEqual(0u, Manager_Base_GetNumberOfRegisteredEntities_C(sm));

			Delete_C(sm);
			Delete_C(em);
		}

		TEST_METHOD(SceneManager_GetEntitiesInScene)
		{
			auto em = EntityManager_CreateEntityManager_C();

			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilz::GUID("TransformManager").id);

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType_C(sm));

			Entity scene = em->Create();
			SceneManager_Create_C(sm, scene, "World");
			Assert::IsTrue(Manager_Base_IsRegistered_C(sm, scene));
			Assert::AreEqual("World", SceneManager_GetNameOfScene_C(sm, scene));

			std::vector<Entity> ents(1000);
			em->CreateMultiple(ents.data(), uint32_t(ents.size()));
			for (size_t i = 0; i < ents.size(); i++)
			{
				SceneManager_AddNamedEntityToScene_C(sm, scene, ents[i], ("Ent" + std::to_string(i)).c_str());
				Assert::AreEqual(uint32_t(i+1), SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
				Assert::AreEqual(("Ent" + std::to_string(i)).c_str(), SceneManager_GetNameOfEntityInScene_C(sm, scene, ents[i]));
			}
			
			Assert::AreEqual(1000u, SceneManager_GetNumberOfEntitiesInScene_C(sm, scene));
			std::vector<Entity> getEnts(ents.size());
			SceneManager_GetEntitiesInScene_C(sm, scene, (uint32_t*)getEnts.data());
			for (size_t i = 0; i < ents.size(); i++)
			{
				Assert::AreEqual(uint32_t(ents[i]), uint32_t(getEnts[i]));
				Assert::AreEqual(("Ent" + std::to_string(i)).c_str(), SceneManager_GetNameOfEntityInScene_C(sm, scene, getEnts[i]));
			}

			Manager_Base_Destroy_C(sm, scene);
			Assert::AreEqual(0u, Manager_Base_GetNumberOfRegisteredEntities_C(sm));
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
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilz::GUID("TransformManager").id);

			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType_C(sm));
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
			tm->SetPosition(scene, { 1.0f });

			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			ss.seekp(0);
			Assert::IsTrue(writer(&ss));
			
			tm->Create(ent, { 0.0f, 1.0f });
			tm->SetPosition(ent, { 0.0f, 1.0f });

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
			Assert::AreEqual(1u, sm->GetNumberOfEntitiesInScene(newScene));
			Assert::AreEqual(1.0f, tm->GetPosition(scene).x);
			Assert::AreEqual(1.0f, tm->GetPosition(ent).y);
			Assert::AreEqual(1.0f, tm->GetPosition(newScene).x);
			Entity newEnt;
			sm->GetEntitiesInScene(newScene, &newEnt);
			Assert::AreEqual(1.0f, tm->GetPosition(newEnt).y);
			Assert::AreEqual(scene.id, tm->GetParent(ent).id);
			Assert::AreEqual(newScene.id, tm->GetParent(newEnt).id);

			auto ent2 = em->Create();
			sm->AddEntityToScene(scene, ent2);
			tm->SetPosition(ent2, { 0.0f,0.0f,1.0f });

			Assert::AreNotEqual(0Ui64, sm->GetDataWriter(scene, writer));
			ss.seekp(0);
			Assert::IsTrue(writer(&ss));

			auto newScene2 = em->Create();
			ss.seekg(0);
			sm->CreateFromStream(newScene2, &ss);
			Assert::AreEqual(2u, sm->GetNumberOfEntitiesInScene(newScene2));
			Assert::AreEqual(3u, sm->GetNumberOfRegisteredEntities());
			Assert::AreEqual(8u, tm->GetNumberOfRegisteredEntities());
			Assert::AreEqual(1.0f, tm->GetPosition(newScene2).x);
			Entity newEnt2[2];
			sm->GetEntitiesInScene(newScene2, newEnt2);
			Assert::AreEqual(1.0f, tm->GetPosition(newEnt2[0]).y);
			Assert::AreEqual(newScene2.id, tm->GetParent(newEnt2[0]).id);
			Assert::AreEqual(1.0f, tm->GetPosition(newEnt2[1]).z);
			Assert::AreEqual(newScene2.id, tm->GetParent(newEnt2[1]).id);

			Delete_C(sm);
			Delete_C(tm);
			Delete_C(em);
		}
		TEST_METHOD(SceneManager_WriteReadComponentResource)
		{
			std::error_code err;
			fs::remove("wrcr.dat", err);
			auto bl = CreateLoader(ResourceHandler::LoaderType::Binary);
			bl->Init("wrcr.dat", ResourceHandler::Mode::EDIT);
			Utilz::ThreadPool pool(4);
			auto rh = CreateResourceHandler(bl, &pool);


			auto em = EntityManager_CreateEntityManager_C();

			TransformManagerInitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilz::GUID("TransformManager").id);


			SceneManagerInitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilz::GUID("SceneManager").id, Manager_Base_GetManagerType_C(sm));
			SceneManager_RegisterManager_C(sm, tm);


			auto scene = em->Create();
			sm->Create(scene, "World");
			tm->SetPosition(scene, { 1.0f });
			auto e1 = em->Create();
			sm->AddEntityToScene(scene, e1, "E1");
			auto e2 = em->Create();
			sm->AddEntityToScene(scene, e2, "E2");
			tm->SetPosition(e1, { 0.0f, 1.0f });
			tm->SetPosition(e2, { 0.0f, 0.0f,1.0f });
			Assert::AreEqual(long(0), Manager_Base_WriteComponent_C(sm, bl, scene, "World", "Scene"));

			auto sceneNew = em->Create();
			Manager_Base_CreateFromResource_C(sm, sceneNew, "World", "Scene");
			Assert::AreEqual(2u, sm->GetNumberOfRegisteredEntities());
			Assert::AreEqual(2u, sm->GetNumberOfEntitiesInScene(sceneNew));
			Assert::AreEqual("World", sm->GetNameOfScene(sceneNew));
			Entity ents[2];
			sm->GetEntitiesInScene(sceneNew, ents);
			Assert::AreEqual("E1", sm->GetNameOfEntityInScene(sceneNew, ents[0]));
			Assert::AreEqual("E2", sm->GetNameOfEntityInScene(sceneNew, ents[1]));

			Delete_C(sm);
			Delete_C(tm);
			Delete_C(em);

			delete rh;
			bl->Shutdown();
			delete bl;
			fs::remove("wrcr.dat", err);
		}
	};
}