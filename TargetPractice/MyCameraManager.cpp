#include "MyCameraManager.h"
using namespace BasicX;
//  MyCameraSingleton
MyCameraManager* MyCameraManager::m_pInstance = nullptr;

void MyCameraManager::Init(void)
{
	m_uActiveCamera = 0;
	AddCamera(vector3(0.0f, 0.0f, 1.0f), vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 1.0f, 0.0f));
	m_uCameraCount = m_lCamera.size();
}
void MyCameraManager::Release(void)
{
	for (uint nCamera = 0; nCamera < m_lCamera.size(); nCamera++)
	{
		MyCamera* pCamera = m_lCamera[nCamera];
		if (pCamera != nullptr)
		{
			delete pCamera;
			pCamera = nullptr;
		}
	}
	m_uCameraCount = m_lCamera.size();
}
MyCameraManager* MyCameraManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyCameraManager();
	}
	return m_pInstance;
}
void MyCameraManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//The big 3
MyCameraManager::MyCameraManager()
{
	Init();
}
MyCameraManager::MyCameraManager(MyCameraManager const& other){ }
MyCameraManager& MyCameraManager::operator=(MyCameraManager const& other) { return *this; }
MyCameraManager::~MyCameraManager(){ Release(); };
//Accessors
void MyCameraManager::SetActiveCamera(uint a_nIndex){ if (a_nIndex < m_uCameraCount) m_uActiveCamera = a_nIndex; return; }
void MyCameraManager::SetPosition(vector3 a_v3Position, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetPosition(a_v3Position);
	}
	return m_lCamera[m_uActiveCamera]->SetPosition(a_v3Position);
}
void MyCameraManager::SetTarget(vector3 a_v3Target, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetTarget(a_v3Target);
	}
	return m_lCamera[m_uActiveCamera]->SetTarget(a_v3Target);
}

void MyCameraManager::SetViewMatrix(matrix4 input, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetViewMatrix(input);
	}
	return m_lCamera[m_uActiveCamera]->SetViewMatrix(input);
}

void MyCameraManager::SetProjectionMatrix(matrix4 input, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetProjectionMatrix(input);
	}
	return m_lCamera[m_uActiveCamera]->SetProjectionMatrix(input);
}
void MyCameraManager::ResetCamera(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->ResetCamera();
	}
	return m_lCamera[m_uActiveCamera]->ResetCamera();
}
void MyCameraManager::SetNearFarPlanes(float a_fNear, float a_fFar, int a_nIndex)
{
	if (a_nIndex = -1)
		m_lCamera[m_uActiveCamera]->SetNearFarPlanes(a_fNear, a_fFar);
	else if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->SetNearFarPlanes(a_fNear, a_fFar);
	}
}
void MyCameraManager::SetFOV(float a_fFOV, int a_nIndex)
{
	if (a_nIndex = -1)
		m_lCamera[m_uActiveCamera]->SetFOV(a_fFOV);
	else if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->SetFOV(a_fFOV);
	}
}
uint MyCameraManager::GetCameraCount(void) { return m_uCameraCount; }
matrix4 MyCameraManager::GetViewMatrix(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetViewMatrix();
	}
	return m_lCamera[m_uActiveCamera]->GetViewMatrix();
}
matrix4 MyCameraManager::GetProjectionMatrix(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetProjectionMatrix();
	}
	return m_lCamera[m_uActiveCamera]->GetProjectionMatrix();
}
matrix4 MyCameraManager::GetMVP(matrix4 a_m4ModelToWorld, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetMVP(a_m4ModelToWorld);
	}
	return m_lCamera[m_uActiveCamera]->GetMVP(a_m4ModelToWorld);
}
matrix4 MyCameraManager::GetVP(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetVP();
	}
	return m_lCamera[m_uActiveCamera]->GetVP();
}
vector3 MyCameraManager::GetPosition(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetPosition();
	}
	return m_lCamera[m_uActiveCamera]->GetPosition();
}
MyCamera* MyCameraManager::GetCamera(int a_nIndex)
{
	if (a_nIndex >= static_cast<int>(m_uCameraCount))
		return nullptr;
	if (a_nIndex < 0)
		return m_lCamera[m_uActiveCamera];
	return m_lCamera[a_nIndex];
}
vector3 MyCameraManager::GetForward(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetForward();
	}
	return m_lCamera[m_uActiveCamera]->GetForward();
}
vector3 MyCameraManager::GetUpward(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetUpward();
	}
	return m_lCamera[m_uActiveCamera]->GetUpward();
}
vector3 MyCameraManager::GetRightward(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetRightward();
	}
	return m_lCamera[m_uActiveCamera]->GetRightward();
}
void MyCameraManager::SetForward(vector3 a_v3Input, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->SetForward(a_v3Input);
	}
	m_lCamera[m_uActiveCamera]->SetForward(a_v3Input);
}
void MyCameraManager::SetUpward(vector3 a_v3Input, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->SetUpward(a_v3Input);
	}
	m_lCamera[m_uActiveCamera]->SetUpward(a_v3Input);
}
void MyCameraManager::SetRightward(vector3 a_v3Input, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->SetRightward(a_v3Input);
	}
	m_lCamera[m_uActiveCamera]->SetRightward(a_v3Input);
}
//--- Non Standard Singleton Methods
uint MyCameraManager::AddCamera(vector3 a_v3Eye, vector3 a_v3Target, vector3 a_v3Up)
{
	MyCamera* pCamera = new MyCamera(a_v3Eye, a_v3Target, a_v3Up);
	m_lCamera.push_back(pCamera);
	m_uCameraCount = m_lCamera.size();
	return m_uCameraCount - 1;
}
void MyCameraManager::CalculateProjection(int a_nIndex)
{
	if (a_nIndex = -1)
		m_lCamera[m_uActiveCamera]->CalculateProjection();
	else if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->CalculateProjection();
	}
}
void MyCameraManager::CalculateView(int a_nIndex)
{
	if (a_nIndex = -1)
		m_lCamera[m_uActiveCamera]->CalculateView();
	else if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		m_lCamera[a_nIndex]->CalculateView();
	}
}
void MyCameraManager::MoveForward(float a_fDistance, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->MoveForward(a_fDistance);
	}
	return m_lCamera[m_uActiveCamera]->MoveForward(a_fDistance);
}
void MyCameraManager::MoveVertical(float a_fDistance, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->MoveVertical(a_fDistance);
	}
	return m_lCamera[m_uActiveCamera]->MoveVertical(a_fDistance);
}
void MyCameraManager::MoveSideways(float a_fDistance, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->MoveSideways(a_fDistance);
	}
	return m_lCamera[m_uActiveCamera]->MoveSideways(a_fDistance);
}
void MyCameraManager::ChangePitch(float a_fDegree, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->ChangePitch(a_fDegree);
	}
	return m_lCamera[m_uActiveCamera]->ChangePitch(a_fDegree);
}
void MyCameraManager::ChangeYaw(float a_fDegree, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->ChangeYaw(a_fDegree);
	}
	return m_lCamera[m_uActiveCamera]->ChangeYaw(a_fDegree);
}
void MyCameraManager::ChangeRoll(float a_fDegree, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->ChangeRoll(a_fDegree);
	}
	return m_lCamera[m_uActiveCamera]->ChangeRoll(a_fDegree);
}
void MyCameraManager::SetPositionTargetAndUpward(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward);
	}
	return m_lCamera[m_uActiveCamera]->SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward);
}
vector3 MyCameraManager::GetClickOnWorldSpace(uint a_nMouseX, uint a_nMouseY, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetClickOnWorldSpace(a_nMouseX, a_nMouseY);
	}
	return m_lCamera[m_uActiveCamera]->GetClickOnWorldSpace(a_nMouseX, a_nMouseY);
}
std::pair<vector3, vector3> MyCameraManager::GetClickAndDirectionOnWorldSpace(uint a_nMouseX, uint a_nMouseY, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetClickAndDirectionOnWorldSpace(a_nMouseX, a_nMouseY);
	}
	return m_lCamera[m_uActiveCamera]->GetClickAndDirectionOnWorldSpace(a_nMouseX, a_nMouseY);
}
matrix4 MyCameraManager::GetCameraSpace(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetCameraSpace();
	}
	return m_lCamera[m_uActiveCamera]->GetCameraSpace();
}
matrix4 MyCameraManager::GetCameraSpaceAdjusted(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetCameraSpaceAdjusted();
	}
	return m_lCamera[m_uActiveCamera]->GetCameraSpaceAdjusted();
}
matrix4 MyCameraManager::GetSpaceUnderMouse(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetSpaceUnderMouse();
	}
	return m_lCamera[m_uActiveCamera]->GetSpaceUnderMouse();
}
matrix4 MyCameraManager::GetCameraPlane(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetCameraPlane();
	}
	return m_lCamera[m_uActiveCamera]->GetCameraPlane();
}
void MyCameraManager::SetFPS(bool a_bFPS, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetFPS(a_bFPS);
	}
	return m_lCamera[m_uActiveCamera]->SetFPS(a_bFPS);
}
void MyCameraManager::SetCameraMode(BTO_CAMERAMODE a_nMode, int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->SetCameraMode(a_nMode);
	}
	return m_lCamera[m_uActiveCamera]->SetCameraMode(a_nMode);
}
BTO_CAMERAMODE MyCameraManager::GetCameraMode(int a_nIndex)
{
	if (static_cast<uint>(a_nIndex) <= m_uCameraCount)
	{
		return m_lCamera[a_nIndex]->GetCameraMode();
	}
	return m_lCamera[m_uActiveCamera]->GetCameraMode();
}