#include "pch.h"
void GLFWApp::InitVariables(void)
{
	//setup camera position
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What I'm looking at
		AXIS_Y);//What is up

	m_bFPC = true;
	//init variables
	//m_pModel = new Simplex::Model();
	bullet = new Simplex::Model();
	//load model
	//m_pModel->Load("Lego\\Unikitty.BTO");
	bullet->Load("bullet.FBX");

	building = new Simplex::Model();

	//building->Load("Models\\groundModel.OBJ");
	//building->Load("Models\\SceneTextured.OBJ");
	building->Load("interior.FBX");

	
	crate = new Simplex::Model();
	crate->Load("crate.FBX");

	crateB = new Simplex::Model();
	crateB->Load("crate.FBX");

	crateC = new Simplex::Model();
	crateC->Load("crate.FBX");

	crateD = new Simplex::Model();
	crateD->Load("crate.FBX");


}
void GLFWApp::Update(void)
{
	//Take screen changes in account
	Reshape();

	//attempting to get the camera to lock its y position
	//m_pCameraMngr->SetPosition(vector3(m_pCameraMngr->GetPosition().x, 2.0f, m_pCameraMngr->GetPosition().z));

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), C_YELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), C_RED);
}
void GLFWApp::Display(void)
{
	// Clear the screen
	ClearScreen();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	m_pMeshMngr->AddSkyboxToRenderList();
	//set the model matrix of the model
	//m_pModel->SetModelMatrix(ToMatrix4(m_qArcBall));
	//play the default sequence of the model
	//m_pModel->PlaySequence();

	if (renderBullet)
	{
		/*POINT pt;
		GetCursorPos(&pt);
		UINT MouseX = pt.x;
		UINT MouseY = pt.y;
		matrix4 position = glm::translate(IDENTITY_M4, vector3(MouseX, MouseY, -1));*/
		
		if (!addedBullet)
		{	
			bullet->AddToRenderList();
			addedBullet = true;
		}
			


		bullet->SetModelMatrix(ToMatrix4(m_qArcBall));
		//bullet->SetModelMatrix(position);
		bullet->PlaySequence();
	}

	building->AddToRenderList();
	building->SetModelMatrix(ToMatrix4(m_qArcBall));
	building->PlaySequence();


	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, .5f, .5f));

	matrix4 m4Translate;
	m4Translate = glm::translate(IDENTITY_M4, vector3(0, 1 , 5));
	

	crate->AddToRenderList();
	crate->SetModelMatrix(m4Scale * m4Translate);
	crate->PlaySequence();

	m4Translate = glm::translate(IDENTITY_M4, vector3(15, 1, 20));
	crateB->AddToRenderList();
	crateB->SetModelMatrix(m4Scale * m4Translate);
	crateB->PlaySequence();

	m4Translate = glm::translate(IDENTITY_M4, vector3(-20, 1, 15));

	crateC->AddToRenderList();
	crateC->SetModelMatrix(m4Scale * m4Translate);
	crateC->PlaySequence();
	m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1, -40));

	crateD->AddToRenderList();
	crateD->SetModelMatrix(m4Scale * m4Translate);
	crateD->PlaySequence();




	//render list call
	m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	// Swap buffers
	glfwSwapBuffers(m_pWindow);
	//poll events in glfw
	glfwPollEvents();
}

void GLFWApp::Release(void)
{
	//release variables
	//SafeDelete(m_pModel);
	SafeDelete(bullet);
	SafeDelete(building);
	SafeDelete(crate);
}