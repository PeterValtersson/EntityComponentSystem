#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers/DataManager_Interface.h>

#include <filesystem>

namespace fs = std::experimental::filesystem;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>

namespace Tests
{
	TEST_CLASS( DataManagerTests ){
public:
	TEST_METHOD( CreateManager )
	{
		auto em = EntityManager_Interface::create_manager();

		Assert::ExpectException<CouldNotCreateManager>( []()
		{
			DataManager_Init_Info dmii;
			auto dm = DataManager_Interface::create_manager( dmii );
		}, L"No Enitity Manager exception thrown", LINE_INFO() );

		DataManager_Init_Info dmii;

		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );
		Assert::AreEqual( dm->GetManagerType().id, Utilities::GUID( "DataManager" ).id, L"Incorrect Manager Type", LINE_INFO() );
	}

	TEST_METHOD( Create )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		for ( size_t i = 0; i < ents.size(); i++ )
			dm->Create( ents[i] );
		Assert::AreEqual<size_t>( 10000u, dm->GetNumberOfRegisteredEntities(), L"Could not create", LINE_INFO() );
	}
	TEST_METHOD( CreateMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		dm->CreateMultiple( ents );
		Assert::AreEqual<size_t>( 10000u, dm->GetNumberOfRegisteredEntities(), L"Could not create", LINE_INFO() );
	}
	TEST_METHOD( Destroy )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		for ( auto e : ents )
			dm->Destroy( e );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		dm->DestroyMultiple( ents.data(), ents.size() );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultipleVector )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		dm->DestroyMultiple( ents );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( GarbageCollection )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );
		em->DestroyMultiple( ents );
		int count = 0;
		while ( em->GetNumberOfAliveEntities() > 0 )
		{
			dm->Frame();
			count++;
			Assert::AreNotEqual( count, 33, L"Entities not destroyed within 33 calls to Frame", LINE_INFO() );
		}
	}

	TEST_METHOD( DestroyNow )
	{
		auto em = EntityManager_Interface::create_manager();
		DataManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = DataManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );
		em->DestroyMultipleNow( ents );
	}

	TEST_METHOD( EntityManager_FileExport )
	{
		std::stringstream ss;
		{
			auto em = EntityManager_Interface::create_manager();
			DataManager_Init_Info dmii;
			dmii.entity_manager = em;
			auto dm = DataManager_Interface::create_manager( dmii );


			std::vector<Entity> ents( 10000 );
			em->CreateMultiple( ents );

			for ( size_t i = 0; i < ents.size(); i++ )
			{
				dm->Create( ents[i] );
				dm->Set_Data(ents[i],  )
			}

			dm->Frame();
			em->shrink_to_fit();
			em->write_to_stream( ss );

			auto usage = dm->get_memory_usage();
			dm->shrink_to_fit();
			Assert::IsTrue( dm->get_memory_usage() <= usage, L"Shink failed", LINE_INFO() );
			dm->write_to_stream( ss );
		}
		ss.seekg( 0 );
		{
			auto em = ECS::EntityManager_Interface::create_manager();
			TransformManager_Init_Info tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_Interface::create_manager( tmii );
			em->read_from_stream( ss );
			Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Not empty", LINE_INFO() );
			dm->read_from_stream( ss );
			Assert::AreEqual<size_t>( 10000u, dm->GetNumberOfRegisteredEntities(), L"Could not read from stream", LINE_INFO() );
			auto ents = dm->GetRegisteredEntities();
			Assert::AreEqual<size_t>( 10000, ents.size(), L"Could not read from stream", LINE_INFO() );
			for ( size_t i = 0; i < ents.size(); i++ )
			{
				Assert::AreEqual( float( i ), dm->GetPosition( ents[i] ).x, L"Position not the same", LINE_INFO() );
				Assert::AreEqual( float( i ), dm->GetPosition( ents[i] ).y, L"Position not the same", LINE_INFO() );
				Assert::AreEqual( float( i ), dm->GetPosition( ents[i] ).z, L"Position not the same", LINE_INFO() );
				//Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual( float( i ), dm->GetScale( ents[i] ).z, L"Scale not the same", LINE_INFO() );
			}
		}
	}
	};
}