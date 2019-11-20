#include "pch.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		int axis = SAT(a_pOther);
		if (axis != eSATResults::SAT_NONE)
		{
			bColliding = false;// reset to false
		}

		switch (axis)
		{
		case eSATResults::SAT_AX:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(1, 0, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AY:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(0, 1, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AZ:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(0, 0, 1)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_BX:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(1, 0, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_BY:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(0, 1, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_BZ:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(0, 0, 1)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AXxBX:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(1, 0, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AXxBY:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld, vector3(1, 0, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AXxBZ:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(1, 0, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AYxBX:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 1, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AYxBY:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 1, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AYxBZ:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 1, 0)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AZxBX:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 0, 1)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AZxBY:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 0, 1)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		case eSATResults::SAT_AZxBZ:m_pMeshMngr->AddPlaneToRenderList(glm::translate(m_m4ToWorld,vector3(0, 0, 1)), C_BLUE_CORNFLOWER, RENDER_SOLID); break;
		}				 
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}
void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

struct OBB
{
	glm::vec3 corners[8];
};

//get the normals of the object passed in
std::vector<glm::vec3> GetNormals(MyRigidBody* obj)
{
	std::vector<glm::vec3> normals;

	normals.push_back(vector3(obj->GetModelMatrix() * vector4(AXIS_X, 0))); //RIGHT
	normals.push_back(vector3(obj->GetModelMatrix() * vector4(AXIS_Y, 0))); //UP
	normals.push_back(vector3(obj->GetModelMatrix() * vector4(AXIS_Z, 0)));  //FORWARD

	return normals;
}

//gets edge normals of the two objects' points
std::vector<glm::vec3> GetEdgeNormals(std::vector<glm::vec3> first, std::vector<glm::vec3> second)
{
	std::vector<glm::vec3> normals;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (first[i] == second[j])
			{
				normals.push_back(glm::cross(first[i], second[j]));
			}
			else
			{
				normals.push_back(glm::normalize(glm::cross(first[i], second[j])));
			}
		}
	}

	return normals;
}

//finds the min and max of the bounding box passed in
void GetMinMax(OBB obj, glm::vec3 axis, float& min, float& max)
{
	min = glm::dot(obj.corners[0], axis);
	max = min;

	for (int i = 1; i < 8; i++)
	{
		float currProj = glm::dot(obj.corners[i], axis);

		if (min > currProj)
		{
			min = currProj;
		}

		if (currProj > max)
		{
			max = currProj;
		}
	}
}

//gets the corners of the object
std::vector<vector3> MyRigidBody::GetCorners(MyRigidBody* obj)
{
	std::vector<vector3> points;

	//front
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MinL.x, obj->m_v3MinL.y, obj->m_v3MinL.z, 1))); //top left
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MaxL.x, obj->m_v3MinL.y, obj->m_v3MinL.z, 1))); //top right
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MinL.x, obj->m_v3MaxL.y, obj->m_v3MinL.z, 1))); //bottom left
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MaxL.x, obj->m_v3MaxL.y, obj->m_v3MinL.z, 1))); //bottom right

	
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MinL.x, obj->m_v3MinL.y, obj->m_v3MaxL.z, 1))); //top left
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MaxL.x, obj->m_v3MinL.y, obj->m_v3MaxL.z, 1)));//top right
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MinL.x, obj->m_v3MaxL.y, obj->m_v3MaxL.z, 1))); //bottom left
	points.push_back(vector3(obj->GetModelMatrix() * vector4(obj->m_v3MaxL.x, obj->m_v3MaxL.y, obj->m_v3MaxL.z, 1))); //bottom right

	return points;
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	//find projection of each corner onto each axis
	//take projections and find min and max of each and compare to min and max of each for the other object

	//find corners of each object
	OBB obj1;
	std::vector<vector3> pts = GetCorners(this);
	for (int i = 0; i < pts.size(); i++)
	{
		obj1.corners[i] = pts[i];
	}

	OBB obj2;
	pts = GetCorners(a_pOther);
	for (int i = 0; i < pts.size(); i++)
	{
		obj2.corners[i] = pts[i];
	}

	//get all normals
	std::vector<glm::vec3> oneNormals = GetNormals(this);
	std::vector<glm::vec3> twoNormals = GetNormals(a_pOther);
	std::vector<glm::vec3> edgeNormals = GetEdgeNormals(oneNormals, twoNormals);

	int axes[3];
	axes[0] = eSATResults::SAT_AX;
	axes[1] = eSATResults::SAT_AY;
	axes[2] = eSATResults::SAT_AZ;
	for (int i = 0; i < oneNormals.size(); i++)
	{
		// Get the Min and Max projections for each object along the normal.
		float oneMin, oneMax;
		GetMinMax(obj1, oneNormals[i], oneMin, oneMax);

		float twoMin, twoMax;
		GetMinMax(obj2, oneNormals[i], twoMin, twoMax);

		// If the maximum projection of one of the objects is less than the minimum projection of the other object, then we can determine that there is a separation 
		if (oneMax < twoMin || twoMax < oneMin)
		{
			return axes[i];
		}
	}
	axes[0] = eSATResults::SAT_BX;
	axes[1] = eSATResults::SAT_BY;
	axes[2] = eSATResults::SAT_BZ;
	for (int i = 0; i < twoNormals.size(); i++)
	{
		float oneMin, oneMax;
		GetMinMax(obj1, twoNormals[i], oneMin, oneMax);

		float twoMin, twoMax;
		GetMinMax(obj2, twoNormals[i], twoMin, twoMax);

		if (oneMax < twoMin || twoMax < oneMin)
		{
			return axes[i];
		}
	}

	int axis[9];
	axis[0] = eSATResults::SAT_AXxBX;
	axis[1] = eSATResults::SAT_AXxBY;
	axis[2] = eSATResults::SAT_AXxBZ;
	axis[3] = eSATResults::SAT_AYxBX;
	axis[4] = eSATResults::SAT_AYxBY;
	axis[5] = eSATResults::SAT_AYxBZ;
	axis[6] = eSATResults::SAT_AZxBX;
	axis[7] = eSATResults::SAT_AZxBY;
	axis[8] = eSATResults::SAT_AZxBZ;

	for (int i = 0; i < edgeNormals.size(); i++)
	{
		float oneMin, oneMax;
		GetMinMax(obj1, edgeNormals[i], oneMin, oneMax);

		float twoMin, twoMax;
		GetMinMax(obj2, edgeNormals[i], twoMin, twoMax);

		if (oneMax < twoMin || twoMax < oneMin)
		{
			return axis[i];
		}
	}

	//there is no axis test that separates this two objects - i.e. collision detected
	return eSATResults::SAT_NONE;
}