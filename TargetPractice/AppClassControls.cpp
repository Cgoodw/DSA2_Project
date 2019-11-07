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
#pragma region Camera Positioning
	float fSpeed = 0.25f;
	
	if (key == GLFW_KEY_W)
	{
		m_pCameraMngr->MoveForward(fSpeed, -1);
	}

	if (key == GLFW_KEY_S)
		m_pCameraMngr->MoveForward(-fSpeed);

	if (key == GLFW_KEY_A)
		m_pCameraMngr->MoveSideways(-fSpeed);

	if (key == GLFW_KEY_D)
		m_pCameraMngr->MoveSideways(fSpeed);

	//if (key == GLFW_KEY_Q)
		//m_pCameraMngr->MoveVertical(-fSpeed);

	//if (key == GLFW_KEY_E)
		//m_pCameraMngr->MoveVertical(fSpeed);
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
		//TODO: add shooting here

		//create object and render on screen
		if(!renderBullet) 
			renderBullet = true; //will now add to render list and display on screen

		//shoot at current mouse location

		//when hits target or ground - don't render object anymore / destroy object
		//renderBullet = false;
		//addedBullet = false;
		
	}
}
//Joystick
void GLFWApp::ProcessJoystick(void) {}
