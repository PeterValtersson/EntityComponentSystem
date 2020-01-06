#include "stdafx.h"
#include "CppUnitTest.h"
#include <EntityManager_Interface.h>
#include <Managers\TransformManager_Interface.h>
#include <filesystem>

namespace fs = std::experimental::filesystem;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ECS;
#include <vector>

namespace Tests
{
	TEST_CLASS( TransformManagerTest ) {
public:
	TEST_METHOD( CreateManager )
	{
		auto em = EntityManager_Interface::create_manager();

		Assert::ExpectException<CouldNotCreateManager>( []()
		{
			TransformManager_Init_Info tmii;
			auto tm = TransformManager_Interface::create_manager( tmii );
		}, L"No Enitity Manager exception thrown", LINE_INFO() );

		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );
		Assert::AreEqual( tm->GetManagerType().id, Utilities::GUID( "TransformManager" ).id, L"Incorrect Manager Type", LINE_INFO() );
	}

	TEST_METHOD( Create )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		for ( size_t i = 0; i < ents.size(); i++ )
			tm->Create( ents[i] );
		Assert::AreEqual<size_t>( 10000u, tm->GetNumberOfRegisteredEntities(), L"Could not create transforms", LINE_INFO() );
	}
	TEST_METHOD( CreateMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		tm->CreateMultiple( ents );
		Assert::AreEqual<size_t>( 10000u, tm->GetNumberOfRegisteredEntities(), L"Could not create transforms", LINE_INFO() );
	}
	TEST_METHOD( Destroy )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );

		for ( auto e : ents )
			tm->Destroy( e );
		Assert::AreEqual<size_t>( 0u, tm->GetNumberOfRegisteredEntities(), L"Could not destroy transforms", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultiple )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );

		tm->DestroyMultiple( ents.data(), ents.size() );
		Assert::AreEqual<size_t>( 0u, tm->GetNumberOfRegisteredEntities(), L"Could not destroy transforms", LINE_INFO() );
	}
	TEST_METHOD( DestroyMultipleVector )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );

		tm->DestroyMultiple( ents );
		Assert::AreEqual<size_t>( 0u, tm->GetNumberOfRegisteredEntities(), L"Could not destroy transforms", LINE_INFO() );
	}
	TEST_METHOD( GarbageCollection )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );
		em->DestroyMultiple( ents );
		int count = 0;
		while ( em->GetNumberOfAliveEntities() > 0 )
		{
			tm->Frame();
			count++;
			Assert::AreNotEqual( count, 33, L"Entities not destroyed within 33 calls to Frame", LINE_INFO() );
		}
	}

	TEST_METHOD( DestroyNow )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );
		em->DestroyMultipleNow( ents );
	}

	TEST_METHOD( PosRotScale )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		for ( size_t i = 0; i < ents.size(); i++ )
			tm->Create( ents[i], { float( i ), float( i ), float( i ) }, { 0, float( i ) }, { 0, 0, float( i ) } );

		tm->Frame();

		for ( size_t i = 0; i < ents.size(); i++ )
		{
			Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).x, L"Position not the same", LINE_INFO() );
			Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).y, L"Position not the same", LINE_INFO() );
			Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).z, L"Position not the same", LINE_INFO() );
			//Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i), L"Rotation not the same", LINE_INFO());
			Assert::AreEqual( float( i ), tm->GetScale( ents[i] ).z, L"Scale not the same", LINE_INFO() );

			tm->SetPosition( ents[i], { float( i + 1 ), float( i + 1 ), float( i + 1 ) } );
			tm->SetRotation( ents[i], { 0, float( i + 1 ) } );
			tm->SetScale( ents[i], { float( i + 1 ) } );
		}

		tm->Frame();

		for ( size_t i = 0; i < ents.size(); i++ )
		{
			Assert::AreEqual( float( i + 1 ), tm->GetPosition( ents[i] ).x, L"Position not the same", LINE_INFO() );
			Assert::AreEqual( float( i + 1 ), tm->GetPosition( ents[i] ).y, L"Position not the same", LINE_INFO() );
			Assert::AreEqual( float( i + 1 ), tm->GetPosition( ents[i] ).z, L"Position not the same", LINE_INFO() );
			Assert::AreEqual( float( i + 1 ), tm->GetScale( ents[i] ).x, L"Scale not the same", LINE_INFO() );
		}

	}

	TEST_METHOD( BindChild )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );

		for ( size_t i = 0; i < ents.size(); i++ )
			tm->Create( ents[i], { float( i ) }, { float( i ) }, { float( i ) } );

		tm->Frame();

		for ( size_t i = 1; i < 1001; i++ )
			tm->BindChild( ents[0], ents[i], TransformFlags::NONE );

		Assert::AreEqual<size_t>( 1000u, tm->GetNumberOfChildren( ents[0] ), L"Entity did not have 1000 children", LINE_INFO() );
		Entity children[10000];
		tm->GetChildren( ents[0], children );
		for ( uint32_t i = 0; i < tm->GetNumberOfChildren( ents[0] ); i++ )
			Assert::AreEqual( ( uint32_t )ents[i + 1], ( uint32_t )children[i], ( L"Child " + std::to_wstring( i ) + L" was not correct" ).c_str(), LINE_INFO() );

		auto childrenV = tm->GetChildren( ents[0] );
		for ( uint32_t i = 0; i < childrenV.size(); i++ )
			Assert::AreEqual( ( uint32_t )ents[i + 1], ( uint32_t )childrenV[i], ( L"Child " + std::to_wstring( i ) + L" was not correct (Vector)" ).c_str(), LINE_INFO() );
	}

	TEST_METHOD( UnbindParent )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );
		auto parent = em->Create();
		tm->Create( parent );
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );

		for ( auto c : ents )
			tm->BindChild( parent, c, TransformFlags::NONE );


		tm->UnbindParent( ents.back() );
		ents.pop_back();
		Assert::AreEqual<size_t>( 9999u, tm->GetNumberOfChildren( parent ), L"Entity did not have 999 children", LINE_INFO() );

		auto childrenV = tm->GetChildren( parent );
		for ( uint32_t i = 0; i < childrenV.size(); i++ )
			Assert::AreEqual( ( uint32_t )ents[i], ( uint32_t )childrenV[i], ( L"Child " + std::to_wstring( i ) + L" was not correct" ).c_str(), LINE_INFO() );

		tm->UnbindParent( ents.front() );
		ents.erase( ents.begin() );
		Assert::AreEqual<size_t>( 9998u, tm->GetNumberOfChildren( parent ), L"Entity did not have 998 children", LINE_INFO() );

		auto childrenV2 = tm->GetChildren( parent );
		for ( uint32_t i = 0; i < childrenV2.size(); i++ )
			Assert::AreEqual( ents[i].id, childrenV2[i].id, ( L"Child " + std::to_wstring( i ) + L" was not correct" ).c_str(), LINE_INFO() );

		tm->UnbindParent( ents[200] );
		ents.erase( ents.begin() + 200 );
		Assert::AreEqual<size_t>( 9997u, tm->GetNumberOfChildren( parent ), L"Entity did not have 997 children", LINE_INFO() );

		auto childrenV3 = tm->GetChildren( parent );
		for ( uint32_t i = 0; i < childrenV3.size(); i++ )
			Assert::AreEqual( ents[i].id, childrenV3[i].id, ( L"Child " + std::to_wstring( i ) + L" was not correct" ).c_str(), LINE_INFO() );
	}
	TEST_METHOD( UnbindAllChildren )
	{
		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );
		auto parent = em->Create();
		tm->Create( parent );
		std::vector<Entity> ents( 10000 );
		em->CreateMultiple( ents );
		tm->CreateMultiple( ents );


		for ( auto c : ents )
			tm->BindChild( parent, c, TransformFlags::NONE );

		tm->UnbindAllChildren( parent );
		Assert::AreEqual<size_t>( 0u, tm->GetNumberOfChildren( parent ), L"Entity did not have 0 children", LINE_INFO() );

	}


	TEST_METHOD( InheritedTransforms )
	{

		auto em = EntityManager_Interface::create_manager();
		TransformManager_Init_Info tmii;
		tmii.entityManager = em;
		auto tm = TransformManager_Interface::create_manager( tmii );

		auto ent1 = em->Create();
		tm->Create( ent1 );

		auto ent2 = em->Create();
		tm->Create( ent2 );

		tm->BindChild( ent1, ent2 );

		tm->SetPosition( ent1, { 1.0f } );

		tm->Frame();

		Assert::AreEqual( 1.0f, tm->GetPosition( ent1 ).x, L"Ent1 Position not correct" );
		Assert::AreEqual( 0.0f, tm->GetPosition( ent2 ).x, L"Ent2 Position not correct" );
		auto parentTransform = tm->GetTransform( ent1 );
		Assert::IsTrue( parentTransform == tm->GetTransform( ent2 ), L"Translations not Equal" );
	}

	TEST_METHOD( EntityManager_FileExport )
	{
		std::stringstream ss;
		{
			auto em = ECS::EntityManager_Interface::create_manager();
			TransformManager_Init_Info tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_Interface::create_manager( tmii );

			std::vector<Entity> ents( 10000 );
			em->CreateMultiple( ents );

			for ( size_t i = 0; i < ents.size(); i++ )
				tm->Create( ents[i], { float( i ), float( i ), float( i ) }, { 0, float( i ) }, { 0, 0, float( i ) } );

			tm->Frame();
			em->shrink_to_fit();
			em->write_to_stream( ss );

			auto usage = tm->get_memory_usage();
			tm->shrink_to_fit();
			Assert::IsTrue( tm->get_memory_usage() <= usage, L"Shink failed", LINE_INFO() );
			tm->write_to_stream( ss );
		}
		ss.seekg( 0 );
		{
			auto em = ECS::EntityManager_Interface::create_manager();
			TransformManager_Init_Info tmii;
			tmii.entityManager = em;
			auto tm = TransformManager_Interface::create_manager( tmii );
			em->read_from_stream( ss );
			Assert::AreEqual<size_t>( 0u, tm->GetNumberOfRegisteredEntities(), L"Not empty", LINE_INFO() );
			tm->read_from_stream( ss );
			Assert::AreEqual<size_t>( 10000u, tm->GetNumberOfRegisteredEntities(), L"Could not read from stream", LINE_INFO() );
			auto ents = tm->GetRegisteredEntities();
			Assert::AreEqual<size_t>( 10000, ents.size(), L"Could not read from stream", LINE_INFO() );
			for ( size_t i = 0; i < ents.size(); i++ )
			{
				Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).x, L"Position not the same", LINE_INFO() );
				Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).y, L"Position not the same", LINE_INFO() );
				Assert::AreEqual( float( i ), tm->GetPosition( ents[i] ).z, L"Position not the same", LINE_INFO() );
				//Assert::AreEqual(TransformManager_GetRotation_C(tm, ents[i]).x, float(i), L"Rotation not the same", LINE_INFO());
				Assert::AreEqual( float( i ), tm->GetScale( ents[i] ).z, L"Scale not the same", LINE_INFO() );
			}
		}
	}
	};
}