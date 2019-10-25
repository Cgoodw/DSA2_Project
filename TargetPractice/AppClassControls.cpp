#include "AppClass.h"
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
		m_pCameraMngr->MoveForward(fSpeed);

	if (key == GLFW_KEY_S)
		m_pCameraMngr->MoveForward(-fSpeed);

	if (key == GLFW_KEY_A)
		m_pCameraMngr->MoveSideways(-fSpeed);

	if (key == GLFW_KEY_D)
		m_pCameraMngr->MoveSideways(fSpeed);

	if (key == GLFW_KEY_Q)
		m_pCameraMngr->MoveVertical(-fSpeed);

	if (key == GLFW_KEY_E)
		m_pCameraMngr->MoveVertical(fSpeed);
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

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//TODO: add shooting here
	}
}
//Joystick
void GLFWApp::ProcessJoystick(void) {}
