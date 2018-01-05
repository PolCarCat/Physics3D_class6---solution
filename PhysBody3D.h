#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Primitive;
class Module;

// =================================================

enum Sensor_type
{
	NONE,
	SPEED,
	TIME,

};

struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void GetPos(float& x, float& y, float& z) const;
	void SetAsSensor(bool is_sensor);
	bool IsSensor() const;
	void SetEnabled(bool enable);
	bool isEnabled() const;
	void SetVisible(bool _visible);
	bool isVisible() const;
	void Destroy();
	bool ToDestroy() const;

	Sensor_type s_type = NONE;
private:
	btRigidBody* body = nullptr;
	Primitive* shape = nullptr;
	bool is_sensor = false;
	bool enabled = true;
	bool visible = true;
	bool to_be_destroyed = false;


public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__