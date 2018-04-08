#pragma once
#include "Definitions.h"
#include "MyEntity.h"

namespace Simplex
{
	/*
		Entity manager needs a list of all entities in the scene
		Also needs a function which checks collision detection. (passing in two Entities)
		Functions to remove and add entities to list

		Functions that return an entity (by ID and Reference).
	
	*/
	class EntityManager_E
	{
	public:
		std::vector<MyEntity> entityList;

		bool isColliding(MyEntity ob1, MyEntity obj2);
		void AddEntity(MyEntity newEntity);
		void RemoveEntity(MyEntity entity);

		MyEntity* GetEntityById(String id);

		void SetModelMatrix(MyEntity entity, matrix4 tMat);


		EntityManager_E();
		~EntityManager_E();
	};

}
