#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_CYAN);

	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_CYAN);

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	static float fPos = 0.0f;
	float fovy = 45.0f;
	float ratio = m_pSystem->GetWindowWidth() / static_cast<float>(m_pSystem->GetWindowHeight());
	float fNear = 0.01f;
	float fFar = 1000.0f;

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	m4Projection = glm::perspective(fovy, ratio, fNear, fFar);
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	vector3 v3Position(0, 0, 15);
	vector3 v3Target(0, 0, 14);
	vector3 v3Up(0, 1, 0);

	m4View = glm::lookAt(v3Position, v3Target, v3Up);

	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qArcBall));
	m_pMesh1->Render(m4Projection, m4View, glm::translate(vector3(3.0f, 0.0f, 0.0f)));
	//Need center, eye, and up




	//SPACE INVADER EXERCISE
	/*
	//Position the cubes

	//Center
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(0.0f, 3.0f, 0.0f))*m_m4cube);

	//X+1
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, -2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(1.0f, 3.0f, 0.0f))*m_m4cube);

	//X-2
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, -2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 3.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-2.0f, 4.0f, 0.0f))*m_m4cube);

	//X+2
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, -2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 3.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(2.0f, 4.0f, 0.0f))*m_m4cube);

	//X-1
	//cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, -2.0f, 0.0f))*m_m4cube);
	//cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 0.0f, 0.0f))*m_m4cube);
	//cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 1.0f, 0.0f))*m_m4cube);
	//cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 2.0f, 0.0f))*m_m4cube);
	//cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 3.0f, 0.0f))*m_m4cube);

	//X-3
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, -1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 3.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-3.0f, 5.0f, 0.0f))*m_m4cube);

	//X+3
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, -1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 3.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(3.0f, 5.0f, 0.0f))*m_m4cube);

	//X-4
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-4.0f, 2.0f, 0.0f))*m_m4cube);

	//X+4
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(4.0f, 2.0f, 0.0f))*m_m4cube);

	//X-5
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, -1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-5.0f, 1.0f, 0.0f))*m_m4cube);

	//X+5
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f, -1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(5.0f, 1.0f, 0.0f))*m_m4cube);


		
	fPos += 0.05f;
	*/
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}