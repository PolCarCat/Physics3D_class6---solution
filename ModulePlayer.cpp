#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;


	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 6);
	car.chassis_offset.Set(0, 0, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 50.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 4000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float front_wheel_radius = 0.8f;
	float back_wheel_radius = 1.2f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float front_width = car.chassis_size.x;
	float back_width = car.chassis_size.x*1.8f;
	float half_length = car.chassis_size.z;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(front_width - 0.3f * wheel_width, connection_height, half_length - front_wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = front_wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = false;
	car.wheels[0].brake = false;
	car.wheels[0].steering = false;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-front_width + 0.3f * wheel_width, connection_height, half_length - front_wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = front_wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = false;
	car.wheels[1].brake = false;
	car.wheels[1].steering = false;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(back_width - 0.3f * wheel_width, connection_height, -half_length + back_wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = back_wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = true;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-back_width + 0.3f * wheel_width, connection_height, -half_length + back_wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = back_wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = true;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 5, 10);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (max_sp_timer.Read() > 500) {
		max_sp = MAX_SPEED;
		acc = 1;
		max_sp_timer.Stop();
	}

	if (in_intro)
	{

		App->window->Output(2.5f, 5, 5, 0, 0, 0, "PRESS ENTER TO START");
		App->window->Output(2.5, 8, 8, 0, 0, 0, "RECORD: %d m", App->scene_intro->record * (int)App->scene_intro->segment_distance);
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT)

		{
			in_intro = false;
			App->scene_intro->countdown.Start();
		}
	}
	else {
		App->window->Output(pos.x - 2.5f, 5, pos.z + 5, 0, 0, 0, "Spd: %.0f km/h", vehicle->GetKmh());
		App->window->Output(pos.x - 2.5f, 7, pos.z + 5, 0, 0, 0, "Time left: %.2f s", App->scene_intro->curr_time);
		App->window->Output(pos.x - 2.5f, 3, pos.z + 5, 0, 0, 0, "Distance: %d m", App->scene_intro->segments_completed * (int)App->scene_intro->segment_distance);
		App->window->Output(pos.x + 2.5f + 12 * 0.3f, 7, pos.z + 5, 0, 0, 0, "R -> Restart");
		App->window->Output(pos.x + 2.5f + 19 * 0.3f, 5, pos.z + 5, 0, 0, 0, "SPACE -> Reset car");
		App->window->Output(pos.x + 5, 3, pos.z + 5, 0, 0, 0, "Record: %d m", App->scene_intro->record * (int)App->scene_intro->segment_distance);

		Runinputs();
	}

	if (vehicle->GetKmh() > max_sp)
		acceleration = -MAX_ACCELERATION;


	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(-turn);
	vehicle->Brake(brake);

	vehicle->Render();


	btTransform trans = vehicle->vehicle->getChassisWorldTransform();
	btVector3 btpos = trans.getOrigin();
	pos = { btpos.getX(), btpos.getY(), btpos.getZ() };
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->IsSensor())
	{
		if (body2->s_type == SPEED)
		{
			vehicle->ApplyEngineForce(4000);
		}
	}
	else if (body1->IsSensor())
	{
		if (body1->s_type == SPEED)
		{
			vehicle->ApplyEngineForce(8000000);
			max_sp = 600;
			acc = 5;
			max_sp_timer.Start();
			body1->Destroy();
		}
		else
		{
			App->scene_intro->added_time += 5;
			body1->Destroy();
		}
	}
}

void ModulePlayer::Reset() {
	float x, y, z;
	btTransform transform;
	vehicle->GetPos(x, y, z);
	vehicle->SetPos(0, 3, z);
	vehicle->SetRotation(0, 0, 0);
	vehicle->StopAll();


}

void ModulePlayer::Runinputs()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION * acc;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0)
			brake = BRAKE_POWER;
		else acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Reset();
	}
}