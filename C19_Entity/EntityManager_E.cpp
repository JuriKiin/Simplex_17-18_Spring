#include "EntityManager_E.h"
using namespace Simplex;


bool Simplex::EntityManager_E::isColliding(MyEntity ob1, MyEntity obj2)
{
	return ob1.IsColliding(&obj2);
}

void Simplex::EntityManager_E::AddEntity(MyEntity newEntity)
{
	entityList.push_back(newEntity);
}

void Simplex::EntityManager_E::RemoveEntity(MyEntity entity)
{
	for (int i = 0; i < entityList.size(); i++) {
		if (entity.GetUniqueID() == entityList[i].GetUniqueID()) {
			entityList.erase(entityList.begin() + i);
			return;
		}
	}
}

MyEntity * Simplex::EntityManager_E::GetEntityById(String id)
{
	for (int i = 0; i < entityList.size(); i++) {
		if (id == entityList[i].GetUniqueID()) {
			return &entityList[i];
		}
	}
}

void Simplex::EntityManager_E::SetModelMatrix(MyEntity entity, matrix4 tMat)
{
		/*

	entity.GetModel()->SetModelMatrix(tMat);

	//Set model matrix to Steve
	matrix4 mSteve = glm::translate(vector3(2.25f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, -55.0f, AXIS_Z);
	m_pSteve->SetModelMatrix(mSteve);
	m_pSteveRB->SetModelMatrix(mSteve);
	m_pMeshMngr->AddAxisToRenderList(mSteve);
	*/
}

EntityManager_E::EntityManager_E()
{
}


EntityManager_E::~EntityManager_E()
{
}
