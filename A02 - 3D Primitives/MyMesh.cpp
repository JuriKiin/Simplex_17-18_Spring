#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	vector3 top = vector3(0,0,0);
	std::vector<vector3> verts;

	for (int i = 0; i < a_nSubdivisions; i++) {	//Get all of the points on the 2D circle.
		double angle = 2 * PI * i / a_nSubdivisions;	//Angle between vertices (based on subdivisions)
		double x = cosf(angle) * a_fRadius;	//Get xPosition
		double z = sinf(angle) * a_fRadius;	//Get zPosition
		verts.push_back(vector3(x,top.y-a_fHeight,z));	//Add the bottom face center point.
	}

	for (int ii = 0; ii < a_nSubdivisions; ii++) {	//Crate the face triangles

		//THis is better than having the if statements below!!!!!
		//AddTri(Origin,PointList[i],PointList[(i+1) % a_nSubdivisions]);

		if (ii == a_nSubdivisions-1) {	//This is to wrap the sides back around to the front again.
			AddTri(verts[ii],top, verts[0]);	//Side faces
			AddTri(verts[ii], verts[0], vector3(0, top.y - a_fHeight, 0));	//Bottom face
		}
		else {
			AddTri(verts[ii], top, verts[ii + 1]);	//Side Faces
			AddTri(verts[ii], verts[ii+1], vector3(0, top.y - a_fHeight, 0));	//Bottom face
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	vector3 top = vector3(0, 0, 0);
	std::vector<vector3> bottomVerts;
	std::vector<vector3> topVerts;

	for (int i = 0; i < a_nSubdivisions; i++) {	//Get all of the points on the 2D circle.
		double angle = 2 * PI * i / a_nSubdivisions;	//Angle between vertices (based on subdivions)
		double x = cosf(angle) * a_fRadius;	//Get xPosition
		double z = sinf(angle) * a_fRadius;	//Get zPosition

		topVerts.push_back(vector3(x,top.y, z));	//Add top cap point.
		bottomVerts.push_back(vector3(x, top.y - a_fHeight, z));	//Add bottom cap point.
	}

	for (int ii = 0; ii < a_nSubdivisions; ii++) {	//Crate the face triangles
		if (ii == a_nSubdivisions - 1) {	//This is to wrap the sides back around to the front again.
			AddQuad(bottomVerts[0], bottomVerts[ii], topVerts[0], topVerts[ii]);	//Side face
			AddTri(topVerts[ii],top, topVerts[0]);	//top face
			AddTri(bottomVerts[ii], bottomVerts[0], vector3(top.x, top.y - a_fHeight, top.z));	//bottom face
		}
		else {
			AddQuad(bottomVerts[ii+1], bottomVerts[ii], topVerts[ii+1], topVerts[ii]);	//Side face
			AddTri(topVerts[ii], top, topVerts[ii + 1]);	//top face
			AddTri(bottomVerts[ii], bottomVerts[ii + 1], vector3(top.x, top.y - a_fHeight, top.z));	//bottom face
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// Replace this with your code
	vector3 top = vector3(0, 0, 0);
	std::vector<vector3> bottomInnerVerts;
	std::vector<vector3> topInnerVerts;
	std::vector<vector3> bottomOuterVerts;
	std::vector<vector3> topOuterVerts;

	for (int i = 0; i < a_nSubdivisions; i++) {	//Get all of the points on the 2D circle.
		double angle = 2 * PI * i / a_nSubdivisions;	//Angle in between vertices (based on subdivisions)
		double xOuter = cosf(angle) * a_fOuterRadius;	//Get our xPosition of the outer cylinder
		double zOuter = sinf(angle) * a_fOuterRadius;	//Get our zPosition of the outer cylinder
		double xInner = cosf(angle) * a_fInnerRadius;	//Get xPosition of the inner cylinder
		double zInner = sinf(angle) * a_fInnerRadius;	//Get yPosition of the inner cylinder

		//Add starting positions for the cylinder caps.
		topInnerVerts.push_back(vector3(xInner, top.y, zInner));
		bottomInnerVerts.push_back(vector3(xInner, top.y - a_fHeight, zInner));
		topOuterVerts.push_back(vector3(xOuter, top.y, zOuter));
		bottomOuterVerts.push_back(vector3(xOuter, top.y - a_fHeight, zOuter));
	}

	for (int ii = 0; ii < a_nSubdivisions; ii++) {	//Crate the face triangles
		if (ii == a_nSubdivisions - 1) {	//This is to wrap the sides back around to the front again.
			AddQuad(bottomOuterVerts[0], bottomOuterVerts[ii], topOuterVerts[0], topOuterVerts[ii]);	//Outer Face
			AddQuad(bottomInnerVerts[ii], bottomInnerVerts[0], topInnerVerts[ii], topInnerVerts[0]);	//Inner Face
			
			AddQuad(topOuterVerts[0], topOuterVerts[ii],topInnerVerts[0],topInnerVerts[ii]);	//Top Quad
			AddQuad(bottomOuterVerts[ii], bottomOuterVerts[0], bottomInnerVerts[ii], bottomInnerVerts[0]);	//Bottom Quad.
		}
		else {
			AddQuad(bottomOuterVerts[ii+1], bottomOuterVerts[ii], topOuterVerts[ii+1], topOuterVerts[ii]);	//Outer Face
			AddQuad(bottomInnerVerts[ii], bottomInnerVerts[ii+1], topInnerVerts[ii], topInnerVerts[ii+1]);	//Inner Face

			AddQuad(topOuterVerts[ii+1], topOuterVerts[ii], topInnerVerts[ii+1], topInnerVerts[ii]);	//Top Quad
			AddQuad(bottomOuterVerts[ii], bottomOuterVerts[ii+1], bottomInnerVerts[ii], bottomInnerVerts[ii+1]);	//Bottom Quad.
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	vector3 bottomVert(0, a_fRadius * -1, 0);	//Center point at bottom of sphere
	vector3 topVert(0, a_fRadius, 0);			//Center point at top of sphere

	double angle = 2 * PI / a_nSubdivisions;			//Angle between points (based on subdivisions)
	double height = (a_fRadius * 2) / (a_nSubdivisions);//height of the sphere
	double curHeight = 0;								//Keep track of current height.
	double radius = 0;									//Keep track of the radis at that height.

	std::vector<vector3> verts;

	for (int j = 0; j < a_nSubdivisions - 1; j++)	//Calculate vertices
	{
		radius = a_fRadius;						//Default set the radius to the param.
		curHeight += height;					//Increment the height.

		radius = sqrt(pow(radius, 2) - pow(curHeight + bottomVert.y, 2));	//Calc radius

		for (int i = 0; i <= a_nSubdivisions; i++)	//Create the points for this height (circle)
		{
			verts.push_back(vector3(radius * cos(angle * i), curHeight + bottomVert.y, radius * sin(angle * i)));
		}
	}

	for (int h = 0; h < a_nSubdivisions; h++)	//Draw sphere caps
	{
		verts.push_back(vector3(radius * cos(angle * h), curHeight + bottomVert.y, radius * sin(angle * h)));
		AddTri(verts[h], verts[h + 1], bottomVert);	//Bottom faces
		AddTri(verts[h + 1] + vector3(0, height * (a_nSubdivisions - 2), 0), verts[h] + vector3(0, height * (a_nSubdivisions - 2), 0), topVert);	//Top faces
	}

	for (int k = 0; k < verts.size() - a_nSubdivisions - 2; k++)	//Generace the faces
	{
		AddQuad(verts[k + 1], verts[k], verts[k + a_nSubdivisions + 2], verts[k + a_nSubdivisions + 1]);
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}