#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>
namespace Tests
{
	TEST_CLASS( EntityManagerTest ) {
public:

	TEST_METHOD( Create )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents;
		for ( int i = 0; i < 10000; i++ )
		{
			ents.push_back( em->Create() );
		}
		Assert::AreEqual<size_t>( 10000u, em->GetNumberOfAliveEntities(), L"Could not create 10000 entities", LINE_INFO() );
	}
	TEST_METHOD( CreateMultiple )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents.data(), ents.size() );
		Assert::AreEqual<size_t>( 10000u, em->GetNumberOfAliveEntities(), L"Could not create 10000 entities", LINE_INFO() );
	}
	TEST_METHOD( CreateMultipleVector )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		Assert::AreEqual<size_t>( 10000u, em->GetNumberOfAliveEntities(), L"Could not create 10000 entities", LINE_INFO() );
	}
	TEST_METHOD( Destroy )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		int j = 0;
		for ( ; j < 5000; j++ )
		{
			em->Destroy( ents[j] );
		}
		Assert::AreEqual<size_t>( 5000u, em->GetNumberOfAliveEntities(), L"Not 5000 alive left", LINE_INFO() );
		for ( ; j < 10000; j++ )
		{
			em->Destroy( ents[j] );
		}
		Assert::AreEqual<size_t>( 0u, em->GetNumberOfAliveEntities(), L"Not 0 alive left", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultiple )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		em->DestroyMultiple( ents.data(), 5000 );
		Assert::AreEqual<size_t>( 5000u, em->GetNumberOfAliveEntities(), L"Not 5000 alive left", LINE_INFO() );

		em->DestroyMultiple( &ents[5000], 5000 );
		Assert::AreEqual<size_t>( 0u, em->GetNumberOfAliveEntities(), L"Not 0 alive left", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultipleVector )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		em->DestroyMultiple( ents );
		Assert::AreEqual<size_t>( 0u, em->GetNumberOfAliveEntities(), L"Not 0 alive left", LINE_INFO() );

	}
	TEST_METHOD( DestroyAll )
	{
		auto em = ECS::EntityManager_Interface::create_manager();
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		em->DestroyAll();
		Assert::AreEqual<size_t>( 0u, em->GetNumberOfAliveEntities(), L"Could not destroy all entities", LINE_INFO() );
	}

	TEST_METHOD( FileExport )
	{
		std::stringstream ss;
		{
			auto em = ECS::EntityManager_Interface::create_manager();
			std::vector<Entity> ents( 10000 );
			em->CreateMultiple( ents );

			auto usage = em->get_memory_usage();
			em->shrink_to_fit();
			Assert::AreEqual( usage, em->get_memory_usage(), L"Shink failed", LINE_INFO() );
			em->write_to_stream( ss );
		}
		ss.seekg( 0 );
		{
			auto em = ECS::EntityManager_Interface::create_manager();
			Assert::AreEqual<size_t>( 0u, em->GetNumberOfAliveEntities(), L"Not 0 entities alive", LINE_INFO() );
			em->read_from_stream( ss );
			Assert::AreEqual<size_t>( 10000u, em->GetNumberOfAliveEntities(), L"Could not create 10000 entities", LINE_INFO() );
		}
	}
	};
}