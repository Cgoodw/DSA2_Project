#include "pch.h"
#include <iostream>
using namespace std;
void GLFWApp::InitVariables(void)
{
	//setup camera position
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 4.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What I'm looking at
		AXIS_Y);//What is up

	//FPS Camera is true
	m_bFPC = true;

	menuBG = new Simplex::Model();
	menuBG->Load("MainMenuBG.FBX");

	building = new Entity("interiorUpdated.fbx", "building");
	buildingRB = building->GetRigidBody();


	//m_pEntityManager->AddEntity("crate.fbx", "Cube_Player");
	//m_pEntityManager->UsePhysicsSolver();

	gun= new Entity("gunTest.fbx", "gun");


	playerE = new Entity("crate.fbx", "PlayerBox");
	playerRB = playerE->GetRigidBody();

	for (size_t i = 0; i < numCrates; i++)
	{
		//m_pEntityManager->AddEntity("crate.fbx", "Cube_" + std::to_string(i));
		//
		//matrix4 m4Position = glm::translate(vector3((i*5)-5, (i * 5) + 5, (i * 5) + 5));
		//m_pEntityManager->SetModelMatrix(m4Position * glm::scale(vector3(1)));
		//m_pEntityManager->UsePhysicsSolver();
		crate = new Entity("crate.fbx", "crate");
		crates.push_back(crate);
		crateRB = crate->GetRigidBody();
		crateRBs.push_back(crateRB);
	}

	for (size_t i = 0; i < numBarrels; i++)
	{
		barrel = new Entity("barrel.fbx", "barrel");
		barrels.push_back(barrel);
		barrelRB = barrel->GetRigidBody();
		barrelRBs.push_back(barrelRB);
	}

	for (size_t i = 0; i < numTargets; i++)
	{
		target = new Entity("targetVerticalTextured.fbx", "target");
		targets.push_back(target);
		targetRB = target->GetRigidBody();
		targetRBs.push_back(targetRB);
	}
	//add target positions
	targetVectors.push_back(vector3(5, 15, -50.25));
	targetVectors.push_back(vector3(-25, 10, -50.25));
	targetVectors.push_back(vector3(25, 11, -50.25));
	targetVectors.push_back(vector3(5, 11, -16.5));
	targetVectors.push_back(vector3(-15, 11, 25));
	targetVectors.push_back(vector3(-15, 11, 22));//5: rotate 180
	targetVectors.push_back(vector3(13.69, 11, 5));
	targetVectors.push_back(vector3(45, 2, 15));//6: rotate +90
	targetVectors.push_back(vector3(45, 20, -22));
	targetVectors.push_back(vector3(-47, 2, 15));//9: rotate -90
	targetVectors.push_back(vector3(-47, 20, -22));
	//targetVectors.push_back();
		
	for (size_t i = 0; i < numAmmoPacks; i++)
	{
		ammoPack = new Entity("ammo.fbx", "ammoPack");
		ammoPacks.push_back(ammoPack);
		ammoPackRB = ammoPack->GetRigidBody();
		ammoPackRBs.push_back(ammoPackRB);
	}

	for (size_t i = 0; i < numWalls; i++)
	{
		wall = new Entity("wall.fbx", "wall");
		walls.push_back(wall);
		wallRB = wall->GetRigidBody();
		wallRBs.push_back(wallRB);
	}

	//current Scene number
	int sceneNum = 0;

	for (int i = 0; i < 3; i++) {
		tarX.push_back(Random(5, 40));
		//cout << tarX[i] << endl;
	}

	for (int i = 0; i < 3; i++) {
		tarZ.push_back(Random(5, 40));
		//cout << tarZ[i] << endl;
	}
}
void GLFWApp::Update(void)
{
	//Take screen changes in account
	Reshape();

	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Main Menu Scene
	if (sceneNum == 0) {
		timeRemaining = 6000;
		//reset camera view to center
		m_pCameraMngr->SetPositionTargetAndUpward(vector3(0, 4.5, 15), vector3(0, 2.5, 0), vector3(0, 1, 0), 0);

		//reset Variables
		ammo = 30;
		score = 0;

		glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_FALSE);
		m_pMeshMngr->Print("\n\n\n\n\n");
		m_pMeshMngr->Print("\n\n\n\n\n\n");
		m_pMeshMngr->Print("\n\n\n\n\n\n");
		m_pMeshMngr->Print("\n\n\n\n");
		m_pMeshMngr->PrintLine("                  Use WASD to move and Click to shoot!", C_GREEN);
		m_pMeshMngr->PrintLine("                          Push P to start", C_BLUE);

		//start the game on a key press of P
		if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS) {
			ChangeScene(1);
		}
	}

	//Main Game Scene
	if (sceneNum == 1) {

		////Update Entity Manager
		//m_pEntityManager->Update();
		//
		//
		//m_pEntityManager->SetModelMatrix(matrix4(-1,0,0,0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0),"Cube_Player");
		//
		//for (size_t i = 0; i < numCrates; i++)
		//{
		//	int x = m_pEntityManager->GetEntityIndex("Cube_" + std::to_string(i));
		//	m_pEntityManager->SetPosition(vector4(worldOffset,0), x);
		//	m_pEntityManager->AddEntityToRenderList(x, true);
		//}
		//

		timeRemaining -= 1;
		if (timeRemaining <= 0) {
			ChangeScene(2);
		}
		glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);

		//Is the arcball active?
		ArcBall();

		//Is the first person camera active?
		CameraRotation();

		SetCursor(LoadCursor(NULL, IDC_CROSS));
		//print info into the console
		printf("FPS: %d            \r", nFPS);//print the Frames per Second
		//Print info on the screen
		m_pMeshMngr->Print(m_pSystem->GetAppName(), C_YELLOW);
		m_pMeshMngr->PrintLine("         Shoot the Targets!", C_GREEN);
		m_pMeshMngr->Print("FPS:");
		m_pMeshMngr->Print(std::to_string(nFPS), C_YELLOW);

		currPos += 0.5f;

		//Everything below this is garbage
		//x: 23
		//+z: 9.7
		//-z: 40
		if (worldOffset.x > 23)
		{
			worldOffset.x = 23;
		}
		else if (worldOffset.x < -22)
		{
			worldOffset.x = -22;
		}

		if (worldOffset.z > 40)
		{
			worldOffset.z = 40;
		}
		else if (worldOffset.z < -9.7)
		{
			worldOffset.z = -9.7;
		}

		//absolutely degenerate way to do this but idk how else
		m_pMeshMngr->Print("\n\n\n\n\n\n\n\n\n");

		if (ammo < 1) {
			m_pMeshMngr->Print("                                OUT OF AMMO", C_RED);
		}

		m_pMeshMngr->Print("\n\n\n\n\n\n\n\n\n\n\n");
		m_pMeshMngr->Print("   AMMO: ", C_RED);
		m_pMeshMngr->Print(std::to_string(ammo), C_RED);

		m_pMeshMngr->Print("                                           Score: ", C_RED);
		m_pMeshMngr->PrintLine(std::to_string(score), C_RED);

		m_pMeshMngr->Print("Time Remaining: ", C_RED);
		m_pMeshMngr->Print(std::to_string(timeRemaining/6000), C_WHITE);
		m_pMeshMngr->Print(":");

		if (timeRemaining / 100 < 10) {
			m_pMeshMngr->Print(std::to_string(0), C_WHITE);
		}
		m_pMeshMngr->Print(std::to_string(timeRemaining / 100), C_WHITE);
		//m_pMeshMngr->Print("", C_RED);
		//cout << timeRemaining << endl;

		//garbage ended

		
		//check for all player collisions

		//using AABB to check and then lock the normal of movement
		for (int j = 0; j < crateRBs.size(); j++)
		{
			if (IsColliding(playerRB, crateRBs[j])) {

				//get collision direction
				if (crateRBs[j]->GetMinGlobal().x > playerRB->GetCenterGlobal().x) {
					//cout << "crate collision PLAYER x>" << endl;
					if (worldOffset.x < prevOffset.x) {
						worldOffset.x = prevOffset.x;

						//potentially move the object after collision
						//crateRBs[j]->SetModelMatrix(crateRBs[j]->GetModelMatrix());
					}
				}
				if (crateRBs[j]->GetMaxGlobal().x < playerRB->GetCenterGlobal().x) {
					//cout << "crate collision PLAYER x<" << endl;

					if (worldOffset.x > prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (crateRBs[j]->GetMinGlobal().z < playerRB->GetCenterGlobal().z) {
					//cout << "crate collision PLAYER z<" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z > prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}
					
				}
				if (crateRBs[j]->GetMaxGlobal().z > playerRB->GetCenterGlobal().z) {
					//cout << "crate collision PLAYER z>" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z < prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}
				}
			}
		}
		for (int j = 0; j < barrelRBs.size(); j++)
		{
			if (IsColliding(playerRB, barrelRBs[j])) {

				//get collision direction
				if (barrelRBs[j]->GetMinGlobal().x > playerRB->GetCenterGlobal().x) {
					//cout << "barrel collision PLAYER x>" << endl;
					if (worldOffset.x < prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (barrelRBs[j]->GetMaxGlobal().x < playerRB->GetCenterGlobal().x) {
					//cout << "barrel collision PLAYER x<" << endl;

					if (worldOffset.x > prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (barrelRBs[j]->GetMinGlobal().z < playerRB->GetCenterGlobal().z) {
					//cout << "barrel collision PLAYER z<" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z > prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}

				}
				if (barrelRBs[j]->GetMaxGlobal().z > playerRB->GetCenterGlobal().z) {
					//cout << "barrel collision PLAYER z>" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z < prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}
				}
			}

		}
		for (int j = 0; j < ammoPackRBs.size(); j++)
		{
			if (IsColliding(playerRB, ammoPackRBs[j])) {

				//get collision direction
				if (ammoPackRBs[j]->GetMinGlobal().x > playerRB->GetCenterGlobal().x) {
					//cout << "ammoPack collision PLAYER x>" << endl;
					if (worldOffset.x < prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (ammoPackRBs[j]->GetMaxGlobal().x < playerRB->GetCenterGlobal().x) {
					//cout << "ammoPack collision PLAYER x<" << endl;

					if (worldOffset.x > prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (ammoPackRBs[j]->GetMinGlobal().z < playerRB->GetCenterGlobal().z) {
					//cout << "ammoPack collision PLAYER z<" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z > prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}

				}
				if (ammoPackRBs[j]->GetMaxGlobal().z > playerRB->GetCenterGlobal().z) {
					//cout << "ammoPack collision PLAYER z>" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z < prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}
				}
			}
		}
		for (int j = 0; j < wallRBs.size(); j++)
		{
			if (IsColliding(playerRB, wallRBs[j])) {

				//get collision direction
				if (wallRBs[j]->GetMinGlobal().x > playerRB->GetCenterGlobal().x) {
					//cout << "wall collision PLAYER x>" << endl;
					if (worldOffset.x < prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (wallRBs[j]->GetMaxGlobal().x < playerRB->GetCenterGlobal().x) {
					//cout << "wall collision PLAYER x<" << endl;

					if (worldOffset.x > prevOffset.x) {
						worldOffset.x = prevOffset.x;
					}
				}
				if (wallRBs[j]->GetMinGlobal().z < playerRB->GetCenterGlobal().z) {
					//cout << "wall collision PLAYER z<" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z > prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}

				}
				if (wallRBs[j]->GetMaxGlobal().z > playerRB->GetCenterGlobal().z) {
					//cout << "wall collision PLAYER z>" << endl;

					//if the player is moving in that direction, stop them
					if (worldOffset.z < prevOffset.z) {
						worldOffset.z = prevOffset.z;
					}
				}
			}
		}
		//list of bullets
			for (int i = 0; i < bullets.size(); i++)
			{
				//check for collisions with all objects and if so destroy  
				
				for (int j = 0; j < targetRBs.size(); j++)
				{
					if (bulletRBs.size() > i && IsColliding(bulletRBs[i], targetRBs[j]))
					{
						cout << "target collision" << endl;
						//cycle to next target in list, and check if targets should loop back to 0
						currentTarget++;
						if (currentTarget > targetVectors.size() - 1)
						{
							currentTarget = 0;
						}

						score++;

						RemoveBullet(i);

						break;
					}

					

				}
				
				for (int j = 0; j < crateRBs.size(); j++)
				{
					if (bulletRBs.size() > i && IsColliding(bulletRBs[i], crateRBs[j]))
					{
						cout << "crate collision" << endl;

						RemoveBullet(i);

						break;
					}

					if (IsColliding(playerRB, crateRBs[j])) {
						cout << "crate collision PLAYER" << endl;
					}
				}

				for (int j = 0; j < barrelRBs.size(); j++)
				{
					
					if (bulletRBs.size() > i && IsColliding(bulletRBs[i], barrelRBs[j]))
					{
						cout << "barrel collision" << endl;
						RemoveBullet(i);

						break;
						//add bounce back?
					}

					if (IsColliding(playerRB, barrelRBs[j])) {
						cout << "barrel collision PLAYER" << endl;
					}
					
				}
				for (int j = 0; j < ammoPackRBs.size(); j++)
				{
					if (bulletRBs.size() > i && IsColliding(bulletRBs[i], ammoPackRBs[j]))
					{
						cout << "ammo pack collision" << endl;

						ammo += 10;
						RemoveAmmoPack(j);
						RemoveBullet(i);
						
						break;
					}

					if (IsColliding(playerRB,ammoPackRBs[j])) {
						cout << "ammoPack collision PLAYER" << endl;
					}
				}
				for (int j = 0; j < wallRBs.size(); j++)
				{
					if (bulletRBs.size() > i&& IsColliding(bulletRBs[i], wallRBs[j]))
					{

						RemoveBullet(i);

						break;
					}

					if (IsColliding(playerRB, wallRBs[j])) {
						cout << "wall collision PLAYER" << endl;
					}
				}
			}

		//if there are still bullets left, move them
		if (bullets.size() > 0)
		{
			for (int i = 0; i < bullets.size(); i++)
			{
				matrix4 position;

				position = glm::translate(bullets[i]->GetModelMatrix(), bulletFwdVecs[i]); //-4 would be exact camera position for y
				bullets[i]->SetModelMatrix(position);

				//delete bullet if out of world
				if (bullets[i]->GetPosition().x > 50 || bullets[i]->GetPosition().x < -50 || bullets[i]->GetPosition().z > 50 || bullets[i]->GetPosition().z < -50 || bullets[i]->GetPosition().y > 50 || bullets[i]->GetPosition().y < -10)
				{
					cout << "bullet out of bounds" << endl;
					RemoveBullet(i);
				}
			}
		}
	}

	//Game Over Scene
	if (sceneNum == 2) {
		glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_FALSE);
		m_pMeshMngr->Print("\n\n\n\n\n");
		m_pMeshMngr->PrintLine("                                 GAME OVER", C_RED);
		m_pMeshMngr->Print("\n\n\n\n\n");
		m_pMeshMngr->Print("                              FINAL SCORE:", C_RED);
		m_pMeshMngr->Print(std::to_string(score), C_RED);
		m_pMeshMngr->Print("\n\n");
		m_pMeshMngr->PrintLine("                       Push M to go back to the main menu", C_BLUE);
		m_pMeshMngr->Print("\n\n");
		m_pMeshMngr->PrintLine("                               Push ESC to Quit", C_BLUE);

		//move back to the main menu
		if (glfwGetKey(m_pWindow, GLFW_KEY_M) == GLFW_PRESS) {
			ChangeScene(0);
		}
	}
}
void GLFWApp::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Main Menu
	if (sceneNum == 0) {
		//set background to black
		glClearColor(C_BLACK.r, C_BLACK.r, C_BLACK.r, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

		//set the 'image' to the right position
		matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(1.2f, -2.7f, 0));
		matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.61f, .61f, .61f));

		menuBG->SetModelMatrix(m4Scale * m4Translate);
		menuBG->PlaySequence();
	}

	//Game Over Screen
	if (sceneNum == 2) {
		glClearColor(C_BLACK.r, C_BLACK.r, C_BLACK.r, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	}

	//Main Scene
	if (sceneNum == 1) {
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
		m_pMeshMngr->AddSkyboxToRenderList();

		if (bullets.size() > 0)
		{
			for each (Entity * b in bullets)
			{
				b->AddToRenderList();
			}
		}

		building->AddToRenderList();
		building->SetModelMatrix(glm::translate(IDENTITY_M4, worldOffset));

		


		//set player RB

		playerE->AddToRenderList();
		playerE->SetModelMatrix(glm::scale(IDENTITY_M4, vector3(0.5f, .5f, .5f))*glm::translate(IDENTITY_M4, vector3(0,5, 29)));


		//set the scale matrix to shrink all the models
		matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, .5f, .5f));

		matrix4 m4Translate;

		matrix4 m4Rotate;

		m4Translate = glm::translate(IDENTITY_M4, vector3(0, 3 , 15));

		//m4Rotate = glm::rotate(quaternion(IDENTITY_M4),m_pCameraMngr->GetForward());
		
		//gun->SetModelMatrix(m4Scale *m4Translate);
		//gun->AddToRenderList();

		for (size_t i = 0; i < crates.size(); i++)
		{
			//change position for each one
			if (i == 0)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(0, 1.25, 5));
			}
			else if (i == 1)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(15, 1.25, 20));
			}
			else if (i == 2)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(-20, 1.25, 15));
			}
			else if (i == 3)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1.25, -40));
			}

			m4Translate = glm::translate(m4Translate, worldOffset * 2); //adjust for world offset
			crates[i]->AddToRenderList();
			crates[i]->SetModelMatrix(m4Scale * m4Translate);
		}

		for (size_t i = 0; i < barrels.size(); i++)
		{
			//scale barrels even more
			m4Scale = glm::scale(IDENTITY_M4, vector3(0.25f, .25f, .25f));

			//change position for each one
			if (i == 0)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(-30, 1.25, 40));
			}
			else if (i == 1)
			{
				m4Translate = glm::translate(IDENTITY_M4, vector3(30, 1.25, 20));
			}

			m4Translate = glm::translate(m4Translate, worldOffset * 4); //adjust for world offset
			barrels[i]->AddToRenderList();
			barrels[i]->SetModelMatrix(m4Scale * m4Translate);
		}

		/*
		for (size_t i = 0; i < targets.size(); i++)
		{
			//change position for each one
			if (i == 0)
			{
				//Translate target
				m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
				m4Translate = glm::translate(IDENTITY_M4, vector3(5, 15, -50.25));
				//m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[0], 20, tarZ[0]));
				//m4Rotate = glm::rotateY(vector3(0, 0, 0), 90.0);
			}
			else if (i == 1)
			{
				//Translate target1
				//m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[1], 20, tarZ[1]));
				m4Translate = glm::translate(IDENTITY_M4, vector3(-25, 10, -50.25));
			}
			else if (i == 2)
			{
				//Translate target2
				//m4Translate = glm::translate(IDENTITY_M4, vector3(tarX[2], 20, tarZ[2]));
				m4Translate = glm::translate(IDENTITY_M4, vector3(25, 11, -50.25));
			}
			else if (i == 3)
			{
				//Translate target lerp
				m4Translate = glm::translate(IDENTITY_M4, vector3(25, 11, currPos));
			}

			m4Translate = glm::translate(m4Translate, worldOffset * 2); //adjust for world offset
			targets[i]->AddToRenderList();
			targets[i]->SetModelMatrix(m4Scale * m4Translate);
		}
		*/
		//process single target
		m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
		m4Translate = glm::translate(IDENTITY_M4, targetVectors[currentTarget]);
		m4Translate = glm::translate(m4Translate, worldOffset * 2); //adjust for world offset
		m4Rotate = IDENTITY_M4;
		if (currentTarget > 4 && currentTarget < 7)
		{
			m4Rotate = glm::rotate(IDENTITY_M4, 3.14159f, vector3(0, 1, 0));
		}
		else if (currentTarget >= 7 && currentTarget < 9)
		{
			m4Rotate = glm::rotate(IDENTITY_M4, -1.5708f, vector3(0, 1, 0));
		}
		else if(currentTarget >= 9)
		{
			m4Rotate = glm::rotate(IDENTITY_M4, 1.5708f, vector3(0, 1, 0));
		}
		
		targets[0]->AddToRenderList();
		targets[0]->SetModelMatrix(m4Scale* m4Translate * m4Rotate);

		for (size_t i = 0; i < ammoPacks.size(); i++)
		{
			//change position for each one
			if (i == 0)
			{
				//Translate ammoPack
				m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f, 0.5f, 0.5f));
				m4Translate = glm::translate(IDENTITY_M4, vector3(0.0f, 5, 0.0f));
				m4Translate = glm::translate(m4Translate, worldOffset * 2);
			}
			ammoPacks[i]->AddToRenderList();
			ammoPacks[i]->SetModelMatrix(m4Scale * m4Translate);
		}

		for (size_t i = 0; i < walls.size(); i++)
		{
			//change position for each one
			if (i == 0)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(5.31, 0, -10));
			}
			if (i == 1)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(0, 0, -10));
			}
			if (i == 2)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(-19, 0, 0));
			}
			if (i == 3)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(-15, 0, 11));
			}
			if (i == 4)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(-9.69, 0, 11));
			}
			if (i == 5)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(-4.38, 0, 11));
			}
			if (i == 6)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(0.93, 0, 11));
			}
			if (i == 7)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(13.69,0, 2));
			}
			if (i == 8)
			{
				//Translate wall
				m4Translate = glm::translate(IDENTITY_M4, vector3(8.38, 0, 2));
				
			}
			m4Scale = glm::scale(IDENTITY_M4, vector3(1));
			m4Translate = glm::translate(m4Translate, worldOffset);
			walls[i]->AddToRenderList();
			walls[i]->SetModelMatrix(m4Scale * m4Translate);
		}

		/*for (size_t i = 0; i < m_pMeshMngr->GetMeshCount(); i++)
		{
			Mesh* current = m_pMeshMngr->GetMesh(i);
			current.
		}*/
	}

	//render list call
	m_pMeshMngr->Render();
	//clear the render list
	m_pMeshMngr->ClearRenderList();
	// Swap buffers
	glfwSwapBuffers(m_pWindow);
	//poll events in glfw
	glfwPollEvents();
}

//spawns bullet and adds it to appropriate lists
//decreases ammo count
void GLFWApp::SpawnBullet(vector3 pos, vector3 fwd)
{
	if (sceneNum == 1 && ammo > 0)
	{		
		//create bullet
		Simplex::Entity* bullet = new Entity("bulletSphere.fbx", "bullet");
		Simplex::RigidBody* bulletRB = bullet->GetRigidBody();

		//at current mouse position
		matrix4 position = glm::translate(IDENTITY_M4, vector3(pos.x, pos.y - 4, pos.z)); //-4 would be exact camera position for y

		bullet->SetModelMatrix(position);
		bullet->AddToRenderList();

		//add to lists
		bullets.push_back(bullet);
		bulletRBs.push_back(bulletRB);
		bulletFwdVecs.push_back(fwd);

		//subtract Ammo
		ammo -= 1;
	}
	/*else
	{
		ChangeScene(2);
	}*/
}

float GLFWApp::Random(int min, int max)
{
	int randNum = rand() % (max - min + 1) + min;
	float rand = (float)randNum;
	return rand;
}

//FSM Scenes

//runs the main scene
void  GLFWApp::MainScene(void) {
	do {
		Update();
		Display();
		Idle();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_pWindow) == 0);// Check if the ESC key was pressed or the window was closed
}

//runs the main menu
void  GLFWApp::MainMenuScene(void) {

	do {
		Update();
		Display();
		Idle();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_pWindow) == 0);// Check if the ESC key was pressed or the window was closed
	

}

//runs the game over scene
void  GLFWApp::GameOverScene(void) {

	do {
		Update();
		Display();
		Idle();
	} while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_pWindow) == 0);// Check if the ESC key was pressed or the window was closed

	Release();
}

//chanes the scene (input scene number)
void  GLFWApp::ChangeScene(int x) {
	sceneNum = x;
}

//removes and deletes bullet at position
void GLFWApp::RemoveBullet(int position)
{
	//destroy bullet
	SafeDelete(bullets[position]);

	//remove from lists
	bulletRBs.erase(bulletRBs.begin() + position);
	bullets.erase(bullets.begin() + position);
	bulletFwdVecs.erase(bulletFwdVecs.begin() + position);
}

void GLFWApp::RemoveAmmoPack(int position)
{
	SafeDelete(ammoPacks[position]);

	ammoPackRBs.erase(ammoPackRBs.begin() + position);
	ammoPacks.erase(ammoPacks.begin() + position);
}

bool GLFWApp::IsColliding(RigidBody* rb, RigidBody* otherRB)
{
	vector3 min = rb->GetMinGlobal();
	vector3 max = rb->GetMaxGlobal();

	vector3 otherMin = otherRB->GetMinGlobal();
	vector3 otherMax = otherRB->GetMaxGlobal();

	if (otherMax.x < min.x) 
		return false;
	if (otherMin.x > max.x) 
		return false;

	if (otherMax.y < min.y) 
		return false;
	if (otherMin.y > max.y) 
		return false;

	if (otherMax.z < min.z) 
		return false;
	if (otherMin.z > max.z) 
		return false;

	return true;
}


void GLFWApp::Release(void)
{
	//release variables

	SafeDelete(building);

	//delete all bullets
	for each (Entity * b in bullets)
	{
		SafeDelete(b);
	}

	//delete all crates
	for each (Entity * c in crates)
	{
		SafeDelete(c);
	}

	//delete all barrels
	for each (Entity * b in barrels)
	{
		SafeDelete(b);
	}

	//delete all targets
	for each (Entity * t in targets)
	{
		SafeDelete(t);
	}

	//delete all ammoPacks
	for each (Entity * a in ammoPacks)
	{
		SafeDelete(a);
	}

	//delete all barrels
	for each (Entity * w in walls)
	{
		SafeDelete(w);
	}

	SafeDelete(menuBG);
}