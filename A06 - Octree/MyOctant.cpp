#include "MyOctant.h"
using namespace Simplex;


Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//Get instances of each of the managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_uID = m_uOctantCount;	//Sets the ID.
	m_uOctantCount++;	//Increments octantCount.

	m_pRoot = this; 	//Set the root

	m_uMaxLevel = a_nMaxLevel;	//Sets the max level
	m_uIdealEntityCount = a_nIdealEntityCount; //Sets the entityCount.

	m_v3Center = vector3(0, 0, 0); 	//Set the center

	m_fSize = 68.0f;

	m_v3Min = m_v3Center - vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));	//Get the min
	m_v3Max = m_v3Center + vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));	//Get the max

	ConstructTree();	//Builds the tree.

	AssignIDtoEntity(); 	//Assigns the id
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//Get instances of each of the managers
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_uID = m_uOctantCount; 	//Set the id
	m_uOctantCount++;			//Increment the octantCount.

	m_v3Center = a_v3Center;	//Set the center.
	m_fSize = a_fSize;			//Set the size.

	m_v3Min = m_v3Center - vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));	//Get Min
	m_v3Max = m_v3Center + vector3((m_fSize / 2), (m_fSize / 2), (m_fSize / 2));	//Get Max

}

Simplex::MyOctant::MyOctant(MyOctant const & other) {}

Simplex::MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
{
	return *this;
}

Simplex::MyOctant::~MyOctant()
{
	Release(); 	//Calls release
}

void Simplex::MyOctant::Swap(MyOctant & other) {}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize; 	//Returns size
}

Simplex::vector3 Simplex::MyOctant::GetCenterGlobal(void)
{ 
	return m_v3Center; 	//Returns center
}

Simplex::vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min; 	//Returns min
}

Simplex::vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max; 	//Returns max
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	//Gets the min and max of the objects rigidbody
	vector3 max = m_pEntityMngr->GetRigidBody(a_uRBIndex)->GetMaxGlobal();
	vector3 min = m_pEntityMngr->GetRigidBody(a_uRBIndex)->GetMinGlobal();

	//Checks the objects bounding boxes to see if they are colliding
	return (min.x < m_v3Max.x && max.x > m_v3Min.x && min.y < m_v3Max.y && max.y > m_v3Min.y && min.z < m_v3Max.z && max.z > m_v3Min.z);
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	m_pChild[a_nIndex]->Display(a_v3Color); 	//Calls the display method on that child
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(IDENTITY_M4, vector3(m_fSize)), a_v3Color); 	//Render Wire Cube.

	//Displays the children.
	for (int i = 0; i < m_uChildren; i++) {
		m_pChild[i]->Display(a_v3Color);
	}
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	//Loops through the children
	for (int i = 0; i < m_uChildren; i++) {
		if (m_pChild[i]->IsLeaf()) { 		//If the child is a leaf then call its display method
			m_pChild[i]->Display();
		} else {
			m_pChild[i]->DisplayLeafs();		//Else recurse through the children.
		}
	}
}

void Simplex::MyOctant::ClearEntityList(void)
{
	//Loops through the children of this octant
	for (int i = 0; i < m_uChildren; i++) {
		if (m_pChild[i]->IsLeaf()) {			//If the child is a leaf then clear its entitylist
			m_EntityList.clear();
		} else { 								//Else recurse with the children.
			m_pChild[i]->ClearEntityList();
		}
	}
}

void Simplex::MyOctant::Subdivide(void)
{
	if (m_uLevel >= m_uMaxLevel) return; 	//Back out if we are at our maxLevel.

	//Creates the eight children
	m_pChild[0] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / -4, m_fSize / -4), m_fSize / 2);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / -4, m_fSize / -4), m_fSize / 2);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / -4, m_fSize / 4), m_fSize / 2);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / -4, m_fSize / 4), m_fSize / 2);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / 4, m_fSize / -4), m_fSize / 2);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, m_fSize / -4), m_fSize / 2);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(m_fSize / -4, m_fSize / 4, m_fSize / 4), m_fSize / 2);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(m_fSize / 4, m_fSize / 4, m_fSize / 4), m_fSize / 2);

	m_uChildren = 8; 	//Sets the number of children
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
	//Are we a leaf?
	return m_uChildren > 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	//Checks if the list has more entities than the number passed in
	return a_nEntities < m_EntityList.size();
}

void Simplex::MyOctant::KillBranches(void)
{
	//Loops through children
	for (int i = 0; i < m_uChildren; i++) {
		if (m_pChild[i]->IsLeaf()) {	//If were a leaf, delete.
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		} else {
			m_pChild[i]->KillBranches(); //Recursivly go through the children then.
		}
	}
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{

	if (!IsLeaf()) return; //Return if we have no children.

	Subdivide();	//Subdivide the tree.

	//Loop through children
	for (int i = 0; i < m_uChildren; i++) {
		m_pChild[i]->m_pParent = this;	//Set parent to us.
		m_pChild[i]->m_uLevel = m_uLevel + 1;	//Set the level
		m_pChild[i]->m_pRoot = m_pRoot;	//Set the root to our root.

		//Checks if any entities are in its region
		for (int j = 0; j < m_pEntityMngr->GetEntityCount(); j++) {
			if (m_pChild[i]->IsColliding(j)) {
				m_pChild[i]->m_EntityList.push_back(j);
			}
		}
		m_pChild[i]->ConstructTree(a_nMaxLevel);	//Construct tree for each child.
	}
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//Checks if the object is a leaf
	if (IsLeaf()) {
		for (int j = 0; j < m_EntityList.size(); j++) {
			m_pEntityMngr->AddDimension(m_EntityList[j], m_uID); //if so then add its id to its entities
		}
	} else {	//If not, recurse and call on its children.
		for (int i = 0; i < m_uChildren; i++) {
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
