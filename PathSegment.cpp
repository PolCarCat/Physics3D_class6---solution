#include "PathSegment.h"



PathSegment::PathSegment(btRigidBody* main_body, const SegmentInfo& info) : PhysBody3D(main_body), info(info)
{
}


PathSegment::~PathSegment()
{
}

SegmentInfo::~SegmentInfo()
{
	delete[] obstacles;
}
