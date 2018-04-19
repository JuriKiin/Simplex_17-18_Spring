#include "MyOctant.h"
using namespace Simplex;

static uint m_uOctantCount; //will store the number of octants instantiated
static uint m_uMaxLevel;//will store the maximum level an octant can go to
static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//Get instances of each of the managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	//Set the id and octant count
	m_uID = m_uOctantCount;
	m_uOctantCount++;



	//Set the root
	m_pRoot = this;

	//Set the max level and ideal entity count of the octree
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	//Set the center of the octree
	m_v3Center = vector3(0, 0, 0);

	m_fSize = 68.0f;

	//Get the min and max
	m_v3Min = m_v3Center - vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));
	m_v3Max = m_v3Center + vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));

	//Construct the tree
	ConstructTree();

	//Assign the id of the octree
	AssignIDtoEntity();
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//Get instances of each of the managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	//Set the id and octant count
	m_uID = m_uOctantCount;
	m_uOctantCount++;

	//Set the size and center
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	//Set the min and max
	m_v3Min = m_v3Center - vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));
	m_v3Max = m_v3Center + vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));

}

Simplex::MyOctant::MyOctant(MyOctant const & other) {}

Simplex::MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	return *this;
}

Simplex::MyOctant::~MyOctant()
{
	//Calls release
	Release();
}

void Simplex::MyOctant::Swap(MyOctant & other) {}

float Simplex::MyOctant::GetSize(void)
{
	//Retruns the size
	return m_fSize;
}

Simplex::vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	//Retruns the center
	return m_v3Center;
}

Simplex::vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	//Retruns the min
	return m_v3Min;
}

Simplex::vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	//Retruns the max
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	//Gets the min and max of the objects rigidbody
	vector3 colliderMax = m_pEntityMngr->GetRigidBody(a_uRBIndex)->GetMaxGlobal();
	vector3 colliderMin = m_pEntityMngr->GetRigidBody(a_uRBIndex)->GetMinGlobal();

	//Checks the objects bounding boxes to see if they are colliding
	if (colliderMin.x < m_v3Max.x && colliderMax.x > m_v3Min.x && colliderMin.y < m_v3Max.y && colliderMax.y > m_v3Min.y && colliderMin.z < m_v3Max.z && colliderMax.z > m_v3Min.z) {
		return true;
	}

	return false;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//Calls the display method on that child
	m_pChild[a_nIndex]->Display(a_v3Color);
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	//Adds a wire cube to be rendered
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(IDENTITY_M4, vector3(m_fSize)), a_v3Color);

	//Calls the display method on all children
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//Loops through the children of this octant
	for (int i = 0; i < m_uChildren; i++)
	{
		//If the child is a leaf then call its display method
		if (m_pChild[i]->IsLeaf()) {
			m_pChild[i]->Display();
		}
		else {
			//Else call displayleafs on this child
			m_pChild[i]->DisplayLeafs();
		}
	}
}

void Simplex::MyOctant::ClearEntityList(void)
{
	//Loops through the children of this octant
	for (int i = 0; i < m_uChildren; i++)
	{
		//If the child is a leaf then clear its entitylist
		if (m_pChild[i]->IsLeaf()) {
			m_EntityList.clear();
		}
		else {
			//Else call ClearEntityList on this child
			m_pChild[i]->ClearEntityList();
		}
	}
}

void Simplex::MyOctant::Subdivide(void)
{
	//Checks if we are at the max level for this object
	if (m_uLevel >= m_uMaxLevel) {
		return;
	}

	//Creates the eight children
	m_pChild[0] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / -4, m_fSize / -4), m_fSize / 2);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / -4, m_fSize / -4), m_fSize / 2);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / -4, m_fSize / 4), m_fSize / 2);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / -4, m_fSize / 4), m_fSize / 2);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / 4, m_fSize / -4), m_fSize / 2);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, m_fSize / -4), m_fSize / 2);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / 4, m_fSize / 4), m_fSize / 2);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, m_fSize / 4), m_fSize / 2);

	//Sets the number of children
	m_uChildren = 8;
}

Simplex::MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	//Returns the child at that index
	return m_pChild[a_nChild];
}

Simplex::MyOctant * Simplex::MyOctant::GetParent(void)
{
	//Returns the parent
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	//Checks if the object has children, if it does then it is not a leaf
	if (m_uChildren > 0) {
		return false;
	}

	return true;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	//Checks if the list has more entities than the number passed in
	if (a_nEntities < m_EntityList.size()) {
		return true;
	}
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	//Loops through all of the children
	for (int i = 0; i < m_uChildren; i++)
	{
		//Checks if the object is a leaf, if so then delete
		if (m_pChild[i]->IsLeaf()) {
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		}
		else {
			//Else call this method on the child
			m_pChild[i]->KillBranches();
		}
	}
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//Check if the object has no children
	if (m_uChildren != 0) {
		return;
	}

	//Call subdivide
	Subdivide();

	//Loop through through the list of children
	for (int i = 0; i < m_uChildren; i++)
	{
		//Sets the parent of the child
		m_pChild[i]->m_pParent = this;

		//Sets the child's level
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		//Sets the root of child
		m_pChild[i]->m_pRoot = m_pRoot;

		//Checks if any entities are in its region
		for (int j = 0; j < m_pEntityMngr->GetEntityCount(); j++)
		{
			if (m_pChild[i]->IsColliding(j)) {
				m_pChild[i]->m_EntityList.push_back(j);

			}

		}

		//Call construct tree for each child
		m_pChild[i]->ConstructTree(a_nMaxLevel);
	}

}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//Checks if the object is a leaf, if so then add its id to its entities
	if (IsLeaf()) {
		for (int j = 0; j < m_EntityList.size(); j++)
		{
			m_pEntityMngr->AddDimension(m_EntityList[j], m_uID);
		}
	}
	//Call this method for its children
	else {
		for (int i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
	}
}

Simplex::uint Simplex::MyOctant::GetOctantCount(void)
{
	//Return the octant count
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
	//Call killbranches
	KillBranches();
}

void Simplex::MyOctant::Init(void) {}

void Simplex::MyOctant::ConstructList(void) {}
