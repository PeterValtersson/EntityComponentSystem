#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\SceneManager_Interface.h>
#include <ResourceHandler\ResourceHandler_Interface.h>
#include <Managers\TransformManager_Interface.h>
#include <filesystem>
#include <ThreadPool.h>
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

			TransformManager_InitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilities::GUID("Transform").id);

			SceneManager_InitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilities::GUID("Scene").id, Manager_Base_GetManagerType_C(sm));

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

			TransformManager_InitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilities::GUID("Transform").id);

			SceneManager_InitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilities::GUID("Scene").id, Manager_Base_GetManagerType_C(sm));

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

			TransformManager_InitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilities::GUID("Transform").id);

			SceneManager_InitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilities::GUID("Scene").id, Manager_Base_GetManagerType_C(sm));
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
			auto bl = CreateFileSystem(ResourceHandler::FileSystemType::Binary);
			bl->Init("wrcr.dat", ResourceHandler::Mode::EDIT);
			Utilities::ThreadPool pool(4);
			auto rh = CreateResourceHandler(bl, &pool);


			auto em = EntityManager_CreateEntityManager_C();

			TransformManager_InitializationInfo tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_CreateTransformManager_C(tmii);
			Assert::AreEqual(Manager_Base_GetManagerType_C(tm), Utilities::GUID("Transform").id);


			SceneManager_InitializationInfo smii;
			smii.entityManager = em;
			smii.transformManager = tm;
			auto sm = SceneManager_CreateSceneManager_C(smii);
			Assert::AreEqual(Utilities::GUID("Scene").id, Manager_Base_GetManagerType_C(sm));
			SceneManager_RegisterManager_C(sm, tm);
			em->RegisterManagerForDestroyNow(tm);
			em->RegisterManagerForDestroyNow(sm);

			auto scene = em->Create();
			sm->Create(scene, "World");
			tm->SetPosition(scene, { 1.0f });
			auto e1 = em->Create();
			sm->AddEntityToScene(scene, e1, "E1");
			auto e2 = em->Create();
			tm->SetPosition(e1, { 0.0f, 1.0f });
			tm->SetPosition(e2, { 0.0f, 0.0f,1.0f });
			
			auto childScene = em->Create();
			sm->Create(childScene, "Child");
			sm->AddEntityToScene(scene, childScene);
			sm->AddEntityToScene(childScene, e2, "E2");

			auto childScene2 = em->Create();
			sm->Create(childScene2, "Child");
			sm->AddEntityToScene(childScene, childScene2, "Child2");

			{
				Assert::AreEqual(0, Manager_Base_WriteComponent_C(sm, bl, scene, "World", "Scene").errornr);
				sm->SetNameOfScene(scene, "NewWorld");
				Assert::AreEqual(0, Manager_Base_WriteComponent_C(sm, bl, scene, "World", "Scene").errornr);
				Assert::AreEqual(0, Manager_Base_WriteComponent_C(sm, bl, childScene, "Child", "Scene").errornr);
				Assert::AreEqual(0, Manager_Base_WriteComponent_C(sm, bl, childScene2, "Child2", "Scene").errornr);

				uint32_t numRes = 0;
				numRes = SceneManager_GetNumberOfChildResourcesOfSceneResource_C(sm, Utilities::GUID("World"));
				Assert::AreEqual(1u, numRes);
				uint32_t res[2];
				SceneManager_GetChildResourcesOfSceneResource_C(sm, Utilities::GUID("World"), res, numRes);
				Assert::AreEqual(Utilities::GUID("Child").id, res[0]);

				numRes = SceneManager_GetNumberOfChildResourcesOfSceneResource_C(sm, Utilities::GUID("World"));
				Assert::AreEqual(1u, numRes);
				SceneManager_GetChildResourcesOfSceneResource_C(sm, Utilities::GUID("Child"), res, numRes);
				Assert::AreEqual(Utilities::GUID("Child2").id, res[0]);


				em->DestroyAll(true);
				Assert::AreEqual(0u, tm->GetNumberOfRegisteredEntities());
				Assert::AreEqual(0u, sm->GetNumberOfRegisteredEntities());

				auto sceneNew = em->Create();
				Manager_Base_CreateFromResourceG_C(sm, sceneNew, Utilities::GUID("World").id, Utilities::GUID("Scene").id);
				Assert::AreEqual(3u, sm->GetNumberOfRegisteredEntities());
				Assert::AreEqual(2u, sm->GetNumberOfEntitiesInScene(sceneNew));
				Assert::AreEqual(5u, tm->GetNumberOfRegisteredEntities());
				Assert::IsTrue(tm->IsRegistered(sceneNew));
				Assert::AreEqual("NewWorld", sm->GetNameOfScene(sceneNew));
				Entity ents[2];
				sm->GetEntitiesInScene(sceneNew, ents);
				Assert::AreEqual("E1", sm->GetNameOfEntityInScene(sceneNew, ents[0]));
				Assert::AreEqual("Child", sm->GetNameOfEntityInScene(sceneNew, ents[1]));
				Entity ents2[2];
				Assert::IsTrue(sm->IsRegistered(ents[1]));
				Assert::AreEqual(2u, sm->GetNumberOfEntitiesInScene(ents[1]));
				sm->GetEntitiesInScene(ents[1], ents2);
				Assert::AreEqual("E2", sm->GetNameOfEntityInScene(ents[1], ents2[0]));
				Assert::AreEqual("Child2", sm->GetNameOfEntityInScene(ents[1], ents2[1]));
			}


			
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