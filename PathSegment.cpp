#include "PathSegment.h"
#include "Primitive.h"


SegmentInfo::~SegmentInfo()
{
	/*if (obstacles != nullptr)
		delete[] (obstacles);*/
}

SegmentInfo::SegmentInfo()
{
}

SegmentInfo::SegmentInfo(const SegmentInfo & other) : rotation(other.rotation), num_obstacles(other.num_obstacles)
{
	obstacles = (ObstacleInfo*)malloc(num_obstacles * sizeof ObstacleInfo);
	for (uint i = 0; i < num_obstacles ; i++)
		obstacles[i] = other.obstacles[i];
}

const SegmentInfo& SegmentInfo::operator =(const SegmentInfo & other)
{
	rotation = other.rotation;
	num_obstacles = other.num_obstacles;
	obstacles = (ObstacleInfo*)malloc(num_obstacles * sizeof ObstacleInfo);
	for (uint i = 0; i < num_obstacles; i++)
		obstacles[i] = other.obstacles[i];

	return *this;
}

PathSegment::PathSegment(btRigidBody* main_body, const SegmentInfo& info) : PhysBody3D(main_body), info(info)
{
}


PathSegment::~PathSegment()
{
}

void PathSegment::Render()
{
	for (uint i = 0; i < info.num_obstacles; i++)
	{
		Cube wall;
		Sphere ball;
		Primitive* p;
		wall.color = Green;
		ball.color = Red;

		switch (info.obstacles[i].type)
		{
		case WALL:
			p = &wall;
			break;
		case BOULDER:
			p = &ball;
			break;
		default:
			break;
		}

		obstacles[i]->GetTransform(&p->transform);
		p->Render();
	}

	/*for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	chassis.Render();*/
}
