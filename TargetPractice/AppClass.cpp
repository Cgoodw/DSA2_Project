#include "pch.h"
#include <iostream>
using namespace std;
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

	target = new Simplex::Model();
 	target->Load("targetVerticalTextured.FBX");

	targetB = new Simplex::Model();
	targetB->Load("targetVerticalTextured.FBX");

	targetC = new Simplex::Model();
	targetC->Load("targetVerticalTextured.FBX");

	ammoPak = new Simplex::Model();
	ammoPak->Load("ammo.FBX");



	for (int i = 0; i < 3; i++) {
		tarX.push_back(Random(5, 40));
		cout << tarX[i] << endl;
	}
	
	for (int i = 0; i < 3; i++) {
		tarZ.push_back(Random(5, 40));
		cout << tarZ[i] << endl;
	}

	int ammo = 30;
}
void GLFWApp::Update(void)
{
	SetCursor(LoadCursor(NULL, IDC_CROSS));

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

	//See current position

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
	m4Translate = glm::translate(IDENTITY_M4, vector3(0, 1.25, 5));
	

	crate->AddToRenderList();
	crate->SetModelMatrix(m4Scale * m4Translate);
	crate->PlaySequence();

	m4Translate = glm::translate(IDENTITY_M4, vector3(15, 1.25, 20));
	crateB->AddToRenderList();
	crateB->SetModelMatrix(m4Scale * m4Translate);
	crateB->PlaySequence();

	m4Translate = glm::translate(IDENTITY_M4, vector3(-20, 1.25, 15));

	crateC->AddToRenderList();
	crateC->SetModelMatrix(m4Scale * m4Translate);
	crateC->PlaySequence();
	m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1.25, -40));

	crateD->AddToRenderList();
	crateD->SetModelMatrix(m4Scale * m4Translate);
	crateD->PlaySequence();

	//scale barrels even more
	m4Scale = glm::scale(IDENTITY_M4, vector3(0.25f, .25f, .25f));

	m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1.25, 40));

	barrel->AddToRenderList();
	barrel->SetModelMatrix(m4Scale * m4Translate);
	barrel->PlaySequence();


	m4Translate = glm::translate(IDENTITY_M4, vector3( 30, 1.25, 20));

	barrelB->AddToRenderList();
	barrelB->SetModelMatrix(m4Scale * m4Translate);
	barrelB->PlaySequence();

	//glm::mat4  m4Rotate(1); // Creates a identity matrix

	//rotate the object by         this angle    over    this axis
	//m4Rotate = glm::rotate(m4Rotate, 180.0f, glm::vec3(0.0, 0.0, 1.0));


	//Translate target
	m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
	m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[0], 20, tarZ[0]));
	//m4Rotate = glm::rotateY(vector3(0, 0, 0), 90.0);


	target->AddToRenderList();
	target->SetModelMatrix(m4Scale * m4Translate);
	target->PlaySequence();

	//Translate target1
	//m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
	m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[1], 20, tarZ[1]));

	targetB->AddToRenderList();
	targetB->SetModelMatrix(m4Scale * m4Translate);
	targetB->PlaySequence();

	//Translate target2
	//m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
	m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[2], 20, tarZ[2]));


	targetC->AddToRenderList();
	targetC->SetModelMatrix(m4Scale * m4Translate);
	targetC->PlaySequence();

	//Translate ammoPak
	m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
	m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, 5, 0.0f));


	ammoPak->AddToRenderList();
	ammoPak->SetModelMatrix(m4Scale * m4Translate);
	ammoPak->PlaySequence();






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
		Simplex::Entity* bullet = new Entity("bulletSphere.fbx", "bullet");

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

float GLFWApp::Random(int min, int max)
{
	int randNum = rand() % (max - min + 1) + min;
	float rand = (float)randNum;
	return rand;
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
	SafeDelete(target);
	SafeDelete(targetB);
	SafeDelete(targetC);
}