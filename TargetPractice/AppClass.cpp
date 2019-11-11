#include "pch.h"
void GLFWApp::InitVariables(void)
{
	//setup camera position
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What I'm looking at
		AXIS_Y);//What is up

	m_bFPC = true;

	//load models

	building = new Simplex::Model();

	//building->Load("Models\\groundModel.OBJ");
	//building->Load("Models\\SceneTextured.OBJ");
	building->Load("interior.FBX");


	//TODO: Make these into lists and position them
	
	crate = new Simplex::Model();
	crate->Load("crate.FBX");

	crateB = new Simplex::Model();
	crateB->Load("crate.FBX");

	crateC = new Simplex::Model();
	crateC->Load("crate.FBX");

	crateD = new Simplex::Model();
	crateD->Load("crate.FBX");


	barrel = new Simplex::Model();
	barrel->Load("barrel.FBX");

	barrelB = new Simplex::Model();
	barrelB->Load("barrel.FBX");


	int ammo = 30;
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
	m_pMeshMngr->Print(m_pSystem->GetAppName(), C_YELLOW);
	m_pMeshMngr->PrintLine("         Shoot the Targets!", C_GREEN);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), C_YELLOW);

	//Everything below this is garbage
	

	//absolutely degenerate way to do this but idk how else
	m_pMeshMngr->Print("\n\n\n\n\n\n\n\n\n");

	if (ammo < 1) {
		m_pMeshMngr->Print("                                OUT OF AMMO", C_RED);
	}

	m_pMeshMngr->Print("\n\n\n\n\n\n\n\n\n\n\n");
	m_pMeshMngr->Print( "   AMMO: ", C_RED);
	m_pMeshMngr->Print(std::to_string(ammo), C_RED);

	//garbage ended



	//list of bullets
	if (bullets.size()>0)
	{
		for (int i = 0; i< bullets.size(); i++)
		{
			//b->ApplyForce(vector3(0, gravity, 0));
			matrix4 position = glm::translate(bullets[i]->GetModelMatrix(), bulletFwdVecs[i]); //-4 would be exact camera position for y
			bullets[i]->SetModelMatrix(position);
		}
	}
}
void GLFWApp::Display(void)
{
	// Clear the screen
	ClearScreen();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	m_pMeshMngr->AddSkyboxToRenderList();

	if (bullets.size()>0)
	{
		for each (Entity* b in bullets)
		{
			b->AddToRenderList();
		}
	}

	building->AddToRenderList();
	building->SetModelMatrix(ToMatrix4(m_qArcBall));
	building->PlaySequence();


	
	//set the scale matrix to shrink all the models
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, .5f, .5f));

	matrix4 m4Translate;

	//change position for each one
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

	//scale barrels even more
	m4Scale = glm::scale(IDENTITY_M4, vector3(0.35f, .35f, .35f));

	m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1, 40));

	barrel->AddToRenderList();
	barrel->SetModelMatrix(m4Scale * m4Translate);
	barrel->PlaySequence();


	m4Translate = glm::translate(IDENTITY_M4, vector3( 30, 1, 20));

	barrelB->AddToRenderList();
	barrelB->SetModelMatrix(m4Scale * m4Translate);
	barrelB->PlaySequence();


	//render list call
	m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	// Swap buffers
	glfwSwapBuffers(m_pWindow);
	//poll events in glfw
	glfwPollEvents();
}

void GLFWApp::SpawnBullet(vector3 pos, vector3 fwd) 
{
	if (ammo > 0) {
		//create bullet
		Simplex::Entity* bullet = new Entity("bullet.fbx", "bullet");

		//at current mouse position
		matrix4 position = glm::translate(IDENTITY_M4, vector3(pos.x, pos.y - 4, pos.z)); //-4 would be exact camera position for y

		bullet->SetModelMatrix(position);
		bullet->AddToRenderList();

		//add to list
		bullets.push_back(bullet);

		//add fwd vec to list
		bulletFwdVecs.push_back(fwd);


		//subtract Ammo
		ammo -= 1;
	}
}

void GLFWApp::Release(void)
{
	//release variables
	
	//delete all bullets
	for each(Entity* b in bullets)
	{
		SafeDelete(b);
	}
	SafeDelete(building);
	SafeDelete(crate);
	SafeDelete(crateB);
	SafeDelete(crateC);
	SafeDelete(crateD);
	SafeDelete(barrel);
	SafeDelete(barrelB);
}