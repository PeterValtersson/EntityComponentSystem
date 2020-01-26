#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers/PropertyManager_Interface.h>

#include <filesystem>

namespace fs = std::experimental::filesystem;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>

namespace Tests
{
	TEST_CLASS( PropertyManagerTests ){
public:
	TEST_METHOD( CreateManager )
	{
		auto em = EntityManager_Interface::create_manager();

		Assert::ExpectException<CouldNotCreateManager>( []()
		{
			PropertyManager_Init_Info dmii;
			auto dm = PropertyManager_Interface::create_manager( dmii );
		}, L"No Enitity Manager exception thrown", LINE_INFO() );

		PropertyManager_Init_Info dmii;

		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );
		Assert::AreEqual( dm->GetManagerType().id, Utilities::GUID( "PropertyManager" ).id, L"Incorrect Manager Type", LINE_INFO() );
	}

	TEST_METHOD( Create )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );

		for ( size_t i = 0; i < ents.size(); i++ )
			dm->Create( ents[i] );
		Assert::AreEqual<size_t>( 1000u, dm->GetNumberOfRegisteredEntities(), L"Could not create", LINE_INFO() );
	}
	TEST_METHOD( CreateMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );

		dm->CreateMultiple( ents );
		Assert::AreEqual<size_t>( 1000u, dm->GetNumberOfRegisteredEntities(), L"Could not create", LINE_INFO() );
	}
	TEST_METHOD( Destroy )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		for ( auto e : ents )
			dm->Destroy( e );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		dm->DestroyMultiple( ents.data(), ents.size() );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultipleVector )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );

		dm->DestroyMultiple( ents );
		Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Could not destroy", LINE_INFO() );
	}
	TEST_METHOD( GarbageCollection )
	{
		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
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
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );

		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );
		dm->CreateMultiple( ents );
		em->DestroyMultipleNow( ents );
	}
	TEST_METHOD( EntityManager_CreatePropery )
	{

		auto em = EntityManager_Interface::create_manager();
		PropertyManager_Init_Info dmii;
		dmii.entity_manager = em;
		auto dm = PropertyManager_Interface::create_manager( dmii );


		std::vector<Entity> ents( 1000 );
		em->CreateMultiple( ents );

		for ( int i = 0; i < ents.size(); i++ )
		{
			dm->Create( ents[i] );
			dm->CreateProperty( ents[i], "Value1", i );
			dm->CreateProperty( ents[i], "Value2", std::to_string( i ) );
			Assert::AreEqual<size_t>( 2, dm->GetNumProperties( ents[i] ), ( L"Could not create properties for " + std::to_wstring( i ) ).c_str(), LINE_INFO() );

			Assert::AreEqual<std::string>( "Value1", dm->GetPropertyKey( ents[i], "Value1" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			Assert::AreEqual( i, dm->GetProperty<int>( ents[i], "Value1" ), (L"Incorrect property value " + std::to_wstring(i)).c_str(), LINE_INFO());

			Assert::AreEqual<std::string>( "Value2", dm->GetPropertyKey( ents[i], "Value2" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			Assert::AreEqual<std::string>( std::to_string(i), dm->GetProperty<std::string>( ents[i], "Value2" ), ( L"Incorrect property value " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
		}
		for ( int i = 0; i < ents.size(); i++ )
		{
			Assert::AreEqual<size_t>( 2, dm->GetNumProperties( ents[i] ), ( L"Could not create properties for " + std::to_wstring( i ) ).c_str(), LINE_INFO() );

			Assert::AreEqual<std::string>( "Value1", dm->GetPropertyKey( ents[i], "Value1" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			Assert::AreEqual( i, dm->GetProperty<int>( ents[i], "Value1" ), ( L"Incorrect property value " + std::to_wstring( i ) ).c_str(), LINE_INFO() );

			Assert::AreEqual<std::string>( "Value2", dm->GetPropertyKey( ents[i], "Value2" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			Assert::AreEqual<std::string>( std::to_string( i ), dm->GetProperty<std::string>( ents[i], "Value2" ), ( L"Incorrect property value " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
		}
	}
	TEST_METHOD( EntityManager_FileExport )
	{
		std::stringstream ss;
		{
			auto em = EntityManager_Interface::create_manager();
			PropertyManager_Init_Info dmii;
			dmii.entity_manager = em;
			auto dm = PropertyManager_Interface::create_manager( dmii );


			std::vector<Entity> ents( 1000 );
			em->CreateMultiple( ents );

			for ( int i = 0; i < ents.size(); i++ )
			{
				dm->Create( ents[i] );
				dm->CreateProperty( ents[i], "Value1", i );
				dm->CreateProperty( ents[i], "Value2", std::to_string( i ) );
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
			auto em = EntityManager_Interface::create_manager();
			PropertyManager_Init_Info dmii;
			dmii.entity_manager = em;
			auto dm = PropertyManager_Interface::create_manager( dmii );
			em->read_from_stream( ss );
			Assert::AreEqual<size_t>( 0u, dm->GetNumberOfRegisteredEntities(), L"Not empty", LINE_INFO() );
			dm->read_from_stream( ss );
			Assert::AreEqual<size_t>( 1000u, dm->GetNumberOfRegisteredEntities(), L"Could not read from stream", LINE_INFO() );
			auto ents = dm->GetRegisteredEntities();
			Assert::AreEqual<size_t>( 1000, ents.size(), L"Could not read from stream", LINE_INFO() );
			for ( int i = 0; i < ents.size(); i++ )
			{
				Assert::AreEqual<size_t>( 2, dm->GetNumProperties( ents[i] ), ( L"Could not create properties for " + std::to_wstring( i ) ).c_str(), LINE_INFO() );

				Assert::AreEqual<std::string>( "Value1", dm->GetPropertyKey( ents[i], "Value1" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
				Assert::AreEqual( i, dm->GetProperty<int>( ents[i], "Value1" ), ( L"Incorrect property value " + std::to_wstring( i ) ).c_str(), LINE_INFO() );

				Assert::AreEqual<std::string>( "Value2", dm->GetPropertyKey( ents[i], "Value2" ), ( L"Incorrect property key " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
				Assert::AreEqual<std::string>( std::to_string( i ), dm->GetProperty<std::string>( ents[i], "Value2" ), ( L"Incorrect property value " + std::to_wstring( i ) ).c_str(), LINE_INFO() );
			}
		}
	}
	};
}