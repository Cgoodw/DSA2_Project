#include "pch.h"
//Keyboard
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(GLFWApp::m_pSelfPointer)
		GLFWApp::m_pSelfPointer->ProcessKeyboard(key, scancode, action, mods);
}
void GLFWApp::ProcessKeyboard(int key, int scancode, int action, int mods)
{
	//GLFW_PRESS
	//GLFW_REPEAT
	//GLFW_RELEASED

	

	//keysPressed.push_back(GLFW_PRESS);
	//std::cout << "Key: " << GLFW_RELEASE << std::endl;
	//keysPressed.pop_back();
#pragma region Camera Positioning
	float fSpeed = 0.25f;
	if (sprinting == true)
	{
		fSpeed = 1.0f;
	}
	bool moveForward = false;
	vector3 fwd = m_pCameraMngr->GetForward();
	fwd = glm::normalize(fwd);
	fwd.y = 0;
	vector3 right = m_pCameraMngr->GetRightward();
	right = glm::normalize(right);
	right.y = 0;
	prevOffset = worldOffset;
	if (key == GLFW_KEY_W)
	{
		if (moveForward) {
			std::cout << "pp";
		}
		worldOffset -= (fwd*fSpeed);
			
	}
		//m_pCameraMngr->MoveForward(fSpeed);

	if (key == GLFW_KEY_S)
	{
		worldOffset += (fwd * fSpeed);

	}
		//m_pCameraMngr->MoveForward(-fSpeed);

	if (key == GLFW_KEY_A)
	{
			worldOffset += (right * fSpeed);

	}
		//m_pCameraMngr->MoveSideways(-fSpeed);

	if (key == GLFW_KEY_D)
	{
		worldOffset -= (right * fSpeed);

	}

	//Up Right
	if (key == GLFW_KEY_W && key == GLFW_KEY_D)
	{
		worldOffset -= (fwd * fSpeed);
		worldOffset -= (right * fSpeed);

	}

	//Up Left
	if (key == GLFW_KEY_W && key == GLFW_KEY_A)
	{
		worldOffset -= (fwd * fSpeed);
		worldOffset -= (right * fSpeed);

	}

	//Down Right
	if (key == GLFW_KEY_S && key == GLFW_KEY_D)
	{
		worldOffset -= (fwd * fSpeed);
		worldOffset -= (right * fSpeed);

	}

	//Down Left
	if (key == GLFW_KEY_S && key == GLFW_KEY_A)
	{
		worldOffset -= (fwd * fSpeed);
		worldOffset -= (right * fSpeed);

	}

	if (key == GLFW_KEY_LEFT_SHIFT)
	{
		sprinting = !sprinting;
	}
		//m_pCameraMngr->MoveSideways(fSpeed);
	//
	//if (key == GLFW_KEY_Q)
	//	m_pCameraMngr->MoveVertical(-fSpeed);
	//
	//if (key == GLFW_KEY_E)
	//	m_pCameraMngr->MoveVertical(fSpeed);
#pragma endregion
}
//Mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (GLFWApp::m_pSelfPointer)
		GLFWApp::m_pSelfPointer->ProcessMouse(button, action, mods);
}
void GLFWApp::ProcessMouse(int button, int action, int mods)
{
	m_bArcBall = false;
	//sf::Vector2i mouse = sf::Mouse::getPosition();
	

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (sceneNum == 1)
		{
			//shoot bullets
			SpawnBullet(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward());
		}		
	}
}
//Joystick
void GLFWApp::ProcessJoystick(void) {}
