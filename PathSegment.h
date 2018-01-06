#ifndef __PathSegment_H__
#define __PathSegment_H__

#include "PhysBody3D.h"
#include "p2DynArray.h"
#include "glmath.h"

enum ObstacleType {
	WALL,
	BOULDER,
	CYLINDER,
	PENDULUM,
};

struct ObstacleInfo
{
	ObstacleType type; // type of obstacle
	vec3 pos; // position of the obstacle relative to the start of the segment
	vec3 rotation; // rotation of the obstacle relative to the segment
	vec3 dims; // dimensions of the object (width, height, depth)
	float radius;
	bool dynamic; // is the obstacle dynamic?

	const ObstacleInfo& operator = (const ObstacleInfo& other) {
		type = other.type;
		pos = other.pos;
		rotation = other.rotation;
		dims = other.dims;
		radius = other.radius;
		dynamic = other.dynamic;

		return *this;
	}
};

struct SegmentInfo
{
	~SegmentInfo();
	SegmentInfo();
	SegmentInfo(const SegmentInfo&);

	const SegmentInfo& operator =(const SegmentInfo&);

	ObstacleInfo* obstacles = nullptr;
	uint num_obstacles = 0;
	vec3 rotation;
};

class PathSegment : public PhysBody3D
{
public:
	PathSegment(btRigidBody* main_body, const SegmentInfo& info);
	~PathSegment();

	void Render();

private:
	SegmentInfo info;
	p2DynArray<PhysBody3D*> obstacles;
};

#endif