#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	cube = new MyMesh();
	cube->GenerateCube(1.0f, C_CYAN);
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

	m_m4cube = glm::translate(vector3(fPos,0.0f,0.0f));

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
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, -2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 0.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 1.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 2.0f, 0.0f))*m_m4cube);
	cube->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(-1.0f, 3.0f, 0.0f))*m_m4cube);

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