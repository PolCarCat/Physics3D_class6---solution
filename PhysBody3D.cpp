#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

// ---------------------------------------------------------
void PhysBody3D::GetPos(float& x, float& y, float& z) const {
	btVector3 t = body->getWorldTransform().getOrigin();
	x = t.m_floats[0];
	y = t.m_floats[1];
	z = t.m_floats[2];
}

// ---------------------------------------------------------
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if(this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if(is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~ btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

// ---------------------------------------------------------
bool PhysBody3D::IsSensor() const
{
	return is_sensor;
}

void PhysBody3D::SetEnabled(bool enable)
{
	enabled = enable;	
	body->setActivationState(enabled ? ACTIVE_TAG : WANTS_DEACTIVATION);
}

bool PhysBody3D::isEnabled() const
{
	return enabled;
}

void PhysBody3D::SetVisible(bool _visible)
{
	visible = _visible;
}

bool PhysBody3D::isVisible() const
{
	return visible;
}

void PhysBody3D::Destroy()
{
	to_be_destroyed = true;
}

bool PhysBody3D::ToDestroy() const
{
	return to_be_destroyed;
}
