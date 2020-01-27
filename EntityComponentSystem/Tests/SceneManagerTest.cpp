#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\SceneManager_Interface.h>
#include <Managers\TransformManager_Interface.h>
#include <filesystem>
namespace fs = std::experimental::filesystem;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>


namespace Tests
{
	TEST_CLASS( SceneManagerTest ) {
public:
	TEST_METHOD( CreateManager )
	{
		SceneManager_Init_Info smii;
		Assert::ExpectException<CouldNotCreateManager>( [&]()
		{
			auto sm = SceneManager_Interface::create_manager( smii );
		}, L"No Enitity Manager exception thrown", LINE_INFO() );

		auto em = EntityManager_Interface::create_manager();
		smii.entityManager = em;
		Assert::ExpectException<CouldNotCreateManager>( [&]()
		{
			auto sm = SceneManager_Interface::create_manager( smii );
		}, L"No Transform Manager exception thrown", LINE_INFO() );


		auto sm = SceneManager_Interface::create_manager( smii );
		Assert::AreEqual( sm->GetManagerType().id, Utilities::GUID( "SceneManager" ).id, L"Incorrect Manager Type", LINE_INFO() );
	}
	TEST_METHOD( Create )
	{
		auto em = EntityManager_Interface::create_manager();
		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );
		Assert::AreEqual<size_t>( 1, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );
		Assert::IsTrue( sm->is_registered( scene ) );
		Assert::AreEqual<std::string>( "World", sm->GetNameOfScene( scene ) );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Create( ents[i], std::to_string( i ) );
		Assert::AreEqual<size_t>( 10001, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );

		for ( size_t i = 0; i < ents.size(); i++ )
		{
			Assert::IsTrue( sm->is_registered( ents[i] ), ( L"Not registered " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			Assert::AreEqual<std::string>( "World", sm->GetNameOfScene( scene ), ( L"Name not correct " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
		}

	}
	TEST_METHOD( Destroy )
	{
		auto em = EntityManager_Interface::create_manager();


		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Create( ents[i], std::to_string( i ) );

		sm->Destroy( scene );
		Assert::AreEqual<size_t>( 10000, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );
		Assert::IsFalse( sm->is_registered( scene ) );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Destroy( ents[i] );

		Assert::AreEqual<size_t>( 0, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );

	}

	TEST_METHOD( DestroyMultple )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );


		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Create( ents[i], std::to_string( i ) );

		sm->DestroyMultiple( ents.data(), 5000 );
		Assert::AreEqual<size_t>( 5000, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );
		sm->DestroyMultiple( &ents[5000], 5000 );
		Assert::AreEqual<size_t>( 0, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );

	}
	TEST_METHOD( DestroyMultpleVector )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );


		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Create( ents[i], std::to_string( i ) );

		sm->DestroyMultiple( ents );
		Assert::AreEqual<size_t>( 0, sm->GetNumberOfRegisteredEntities(), L"Incorrect number of entities registered", LINE_INFO() );
	}
	TEST_METHOD( GarbageCollection )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		for ( size_t i = 0; i < ents.size(); i++ )
			sm->Create( ents[i], std::to_string( i ) );
		em->DestroyMultiple( ents );

		int count = 0;
		while ( sm->GetNumberOfRegisteredEntities() > 0 )
		{
			sm->Frame();
			count++;
			Assert::AreNotEqual( count, 33, L"Entities not destroyed within 33 calls to Frame", LINE_INFO() );
		}
	}
	TEST_METHOD( AddEntityToScene )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		auto ent = em->Create();
		sm->AddEntityToScene( scene, ent, "House" );
		Assert::AreEqual<size_t>( 1u, sm->GetNumberOfEntitiesInScene( scene ) );
		Assert::AreEqual<std::string>( "House", sm->GetNameOfEntityInScene( scene, ent ) );
	}
	TEST_METHOD( AddEntitiesToScene )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		sm->AddEntitiesToScene( scene, ents.data(), ents.size() );
		Assert::AreEqual<size_t>( 10000u, sm->GetNumberOfEntitiesInScene( scene ) );
	}
	TEST_METHOD( RemoveEntityFromScene )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		auto ent = em->Create();
		sm->AddEntityToScene( scene, ent, "House" );
		sm->RemoveEntityFromScene( scene, ent );
		Assert::AreEqual<size_t>( 0u, sm->GetNumberOfEntitiesInScene( scene ) );
	}
	TEST_METHOD( GetEntitiesInScene )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		sm->AddEntitiesToScene( scene, ents.data(), ents.size() );

		Entity in_scene[10000];
		sm->GetEntitiesInScene( scene, in_scene, 10000 );
		for ( size_t i = 0; i < ents.size(); i++ )
		{
			Assert::AreEqual( ents[i].id, in_scene[i].id, ( L"Incorrect entity " + std::to_wstring( i ) ).c_str() );
			Assert::AreEqual( "Entity_" + std::to_string(ents[i]), sm->GetNameOfEntityInScene(scene, ents[i]), ( L"Incorrect entity name" + std::to_wstring( i ) ).c_str() );
		}		
	}
	TEST_METHOD( GetEntitiesInSceneVector )
	{
		auto em = EntityManager_Interface::create_manager();

		SceneManager_Init_Info smii;
		smii.entityManager = em;
		auto sm = SceneManager_Interface::create_manager( smii );

		auto scene = em->Create();
		sm->Create( scene, "World" );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		sm->AddEntitiesToScene( scene, ents.data(), ents.size() );

		
		auto in_scene = sm->GetEntitiesInScene( scene );
		for ( size_t i = 0; i < ents.size(); i++ )
		{
			Assert::AreEqual( ents[i].id, in_scene[i].id, ( L"Incorrect entity " + std::to_wstring( i ) ).c_str() );
			Assert::AreEqual( "Entity_" + std::to_string( ents[i] ), sm->GetNameOfEntityInScene( scene, ents[i] ), ( L"Incorrect entity name" + std::to_wstring( i ) ).c_str() );
		}

	}


	//TEST_METHOD( WriteReadComponent )
	//{
	//	auto em = EntityManager_CreateEntityManager_C();

	//	TransformManager_Init_Info tmii;
	//	tmii.entityManager = em;
	//	auto tm = TransformManager_CreateTransformManager_C( tmii );
	//	Assert::AreEqual( Manager_Base_GetManagerType_C( tm ), Utilities::GUID( "Transform" ).id );

	//	SceneManager_InitializationInfo smii;
	//	smii.entityManager = em;
	//	smii.transformManager = tm;
	//	auto sm = SceneManager_CreateSceneManager_C( smii );
	//	Assert::AreEqual( Utilities::GUID( "Scene" ).id, Manager_Base_GetManagerType_C( sm ) );
	//	SceneManager_RegisterManager_C( sm, tm );

	//	Entity scene = em->Create();
	//	SceneManager_Create_C( sm, scene, "World" );

	//	std::function<bool( std::ostream * file )> writer;
	//	Assert::AreNotEqual( 0Ui64, sm->GetDataWriter( scene, writer ) );
	//	std::stringstream ss;
	//	Assert::IsTrue( writer( &ss ) );

	//	auto ent = em->Create();
	//	sm->AddEntityToScene( scene, ent, "Ent" );

	//	Assert::AreNotEqual( 0Ui64, sm->GetDataWriter( scene, writer ) );
	//	ss.seekp( 0 );
	//	Assert::IsTrue( writer( &ss ) );

	//	tm->Create( scene, { 1.0f } );
	//	tm->SetPosition( scene, { 1.0f } );

	//	Assert::AreNotEqual( 0Ui64, sm->GetDataWriter( scene, writer ) );
	//	ss.seekp( 0 );
	//	Assert::IsTrue( writer( &ss ) );

	//	tm->Create( ent, { 0.0f, 1.0f } );
	//	tm->SetPosition( ent, { 0.0f, 1.0f } );

	//	Assert::AreNotEqual( 0Ui64, sm->GetDataWriter( scene, writer ) );
	//	ss.seekp( 0 );
	//	Assert::IsTrue( writer( &ss ) );

	//	Assert::AreEqual( 1u, sm->GetNumberOfRegisteredEntities() );
	//	Assert::AreEqual( 2u, tm->GetNumberOfRegisteredEntities() );

	//	auto newScene = em->Create();
	//	ss.seekg( 0 );
	//	sm->CreateFromStream( newScene, &ss );
	//	Assert::AreEqual( 2u, sm->GetNumberOfRegisteredEntities() );
	//	Assert::AreEqual( 4u, tm->GetNumberOfRegisteredEntities() );
	//	Assert::AreEqual( 1u, sm->GetNumberOfEntitiesInScene( newScene ) );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( scene ).x );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( ent ).y );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( newScene ).x );
	//	Entity newEnt;
	//	sm->GetEntitiesInScene( newScene, &newEnt );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( newEnt ).y );
	//	Assert::AreEqual( scene.id, tm->GetParent( ent ).id );
	//	Assert::AreEqual( newScene.id, tm->GetParent( newEnt ).id );

	//	auto ent2 = em->Create();
	//	sm->AddEntityToScene( scene, ent2 );
	//	tm->SetPosition( ent2, { 0.0f,0.0f,1.0f } );

	//	Assert::AreNotEqual( 0Ui64, sm->GetDataWriter( scene, writer ) );
	//	ss.seekp( 0 );
	//	Assert::IsTrue( writer( &ss ) );

	//	auto newScene2 = em->Create();
	//	ss.seekg( 0 );
	//	sm->CreateFromStream( newScene2, &ss );
	//	Assert::AreEqual( 2u, sm->GetNumberOfEntitiesInScene( newScene2 ) );
	//	Assert::AreEqual( 3u, sm->GetNumberOfRegisteredEntities() );
	//	Assert::AreEqual( 8u, tm->GetNumberOfRegisteredEntities() );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( newScene2 ).x );
	//	Entity newEnt2[2];
	//	sm->GetEntitiesInScene( newScene2, newEnt2 );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( newEnt2[0] ).y );
	//	Assert::AreEqual( newScene2.id, tm->GetParent( newEnt2[0] ).id );
	//	Assert::AreEqual( 1.0f, tm->GetPosition( newEnt2[1] ).z );
	//	Assert::AreEqual( newScene2.id, tm->GetParent( newEnt2[1] ).id );

	//	Delete_C( sm );
	//	Delete_C( tm );
	//	Delete_C( em );
	//}
	//TEST_METHOD( WriteReadComponentResource )
	//{
	//	std::error_code err;
	//	fs::remove( "wrcr.dat", err );
	//	auto bl = CreateFileSystem( ResourceHandler::FileSystemType::Binary );
	//	bl->Init( "wrcr.dat", ResourceHandler::Mode::EDIT );
	//	Utilities::ThreadPool pool( 4 );
	//	auto rh = CreateResourceHandler( bl, &pool );


	//	auto em = EntityManager_CreateEntityManager_C();

	//	TransformManager_Init_Info tmii;
	//	tmii.entityManager = em;
	//	auto tm = TransformManager_CreateTransformManager_C( tmii );
	//	Assert::AreEqual( Manager_Base_GetManagerType_C( tm ), Utilities::GUID( "Transform" ).id );


	//	SceneManager_InitializationInfo smii;
	//	smii.entityManager = em;
	//	smii.transformManager = tm;
	//	auto sm = SceneManager_CreateSceneManager_C( smii );
	//	Assert::AreEqual( Utilities::GUID( "Scene" ).id, Manager_Base_GetManagerType_C( sm ) );
	//	SceneManager_RegisterManager_C( sm, tm );
	//	em->RegisterManagerForDestroyNow( tm );
	//	em->RegisterManagerForDestroyNow( sm );

	//	auto scene = em->Create();
	//	sm->Create( scene, "World" );
	//	tm->SetPosition( scene, { 1.0f } );
	//	auto e1 = em->Create();
	//	sm->AddEntityToScene( scene, e1, "E1" );
	//	auto e2 = em->Create();
	//	tm->SetPosition( e1, { 0.0f, 1.0f } );
	//	tm->SetPosition( e2, { 0.0f, 0.0f,1.0f } );

	//	auto childScene = em->Create();
	//	sm->Create( childScene, "Child" );
	//	sm->AddEntityToScene( scene, childScene );
	//	sm->AddEntityToScene( childScene, e2, "E2" );

	//	auto childScene2 = em->Create();
	//	sm->Create( childScene2, "Child" );
	//	sm->AddEntityToScene( childScene, childScene2, "Child2" );

	//	{
	//		Assert::AreEqual( "Success"_hash, Manager_Base_WriteComponent_C( sm, bl, scene, "World", "Scene" ).hash );
	//		sm->SetNameOfScene( scene, "NewWorld" );
	//		Assert::AreEqual( "Success"_hash, Manager_Base_WriteComponent_C( sm, bl, scene, "World", "Scene" ).hash );
	//		Assert::AreEqual( "Success"_hash, Manager_Base_WriteComponent_C( sm, bl, childScene, "Child", "Scene" ).hash );
	//		Assert::AreEqual( "Success"_hash, Manager_Base_WriteComponent_C( sm, bl, childScene2, "Child2", "Scene" ).hash );

	//		uint32_t numRes = 0;
	//		numRes = SceneManager_GetNumberOfChildResourcesOfSceneResource_C( sm, Utilities::GUID( "World" ) );
	//		Assert::AreEqual( 1u, numRes );
	//		uint32_t res[2];
	//		SceneManager_GetChildResourcesOfSceneResource_C( sm, Utilities::GUID( "World" ), res, numRes );
	//		Assert::AreEqual( Utilities::GUID( "Child" ).id, res[0] );

	//		numRes = SceneManager_GetNumberOfChildResourcesOfSceneResource_C( sm, Utilities::GUID( "World" ) );
	//		Assert::AreEqual( 1u, numRes );
	//		SceneManager_GetChildResourcesOfSceneResource_C( sm, Utilities::GUID( "Child" ), res, numRes );
	//		Assert::AreEqual( Utilities::GUID( "Child2" ).id, res[0] );


	//		em->DestroyAll( true );
	//		Assert::AreEqual( 0u, tm->GetNumberOfRegisteredEntities() );
	//		Assert::AreEqual( 0u, sm->GetNumberOfRegisteredEntities() );

	//		auto sceneNew = em->Create();
	//		Manager_Base_CreateFromResourceG_C( sm, sceneNew, Utilities::GUID( "World" ).id, Utilities::GUID( "Scene" ).id );
	//		Assert::AreEqual( 3u, sm->GetNumberOfRegisteredEntities() );
	//		Assert::AreEqual( 2u, sm->GetNumberOfEntitiesInScene( sceneNew ) );
	//		Assert::AreEqual( 5u, tm->GetNumberOfRegisteredEntities() );
	//		Assert::IsTrue( tm->is_registered( sceneNew ) );
	//		Assert::AreEqual( "NewWorld", sm->GetNameOfScene( sceneNew ) );
	//		Entity ents[2];
	//		sm->GetEntitiesInScene( sceneNew, ents );
	//		Assert::AreEqual( "E1", sm->GetNameOfEntityInScene( sceneNew, ents[0] ) );
	//		Assert::AreEqual( "Child", sm->GetNameOfEntityInScene( sceneNew, ents[1] ) );
	//		Entity ents2[2];
	//		Assert::IsTrue( sm->is_registered( ents[1] ) );
	//		Assert::AreEqual( 2u, sm->GetNumberOfEntitiesInScene( ents[1] ) );
	//		sm->GetEntitiesInScene( ents[1], ents2 );
	//		Assert::AreEqual( "E2", sm->GetNameOfEntityInScene( ents[1], ents2[0] ) );
	//		Assert::AreEqual( "Child2", sm->GetNameOfEntityInScene( ents[1], ents2[1] ) );
	//	}



	//	Delete_C( sm );
	//	Delete_C( tm );
	//	Delete_C( em );

	//	delete rh;
	//	bl->Shutdown();
	//	delete bl;
	//	fs::remove( "wrcr.dat", err );
	//}
	};
}