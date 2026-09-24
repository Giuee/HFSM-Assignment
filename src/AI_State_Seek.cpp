#include "AI_State_Seek.h"
#include "raylib.h"
#include "raymath.h"	//Vector2Distance, Vector2Subtract, Vector2Normalize (raylib.h alone doesn't have these)//

//Constants for the state//
const char* SEEK_STATE_NAME = "Seek";

AI_State_Seek::AI_State_Seek() : AI_State(e_AI_StateID::Seek, SEEK_STATE_NAME)
{
}

void AI_State_Seek::OnEnter(body& _agent)
{
}

e_AI_StateID AI_State_Seek::OnUpdate(body& _agent, body& _target, float dt)
{
	//position is the top-left corner, so measure between the centres of the boxes//
	Vector2 agentCentre = { _agent.position.x + _agent.size.x * 0.5f, _agent.position.y + _agent.size.y * 0.5f };
	Vector2 targetCentre = { _target.position.x + _target.size.x * 0.5f, _target.position.y + _target.size.y * 0.5f };

	//if the target escapes our range, switch to wander state//
	float distanceToTarget = Vector2Distance(targetCentre, agentCentre);
	if (distanceToTarget >= maxSeekDistance)
	{
		return e_AI_StateID::Wander;
	}
	else if (distanceToTarget <= minSeekDistance)
	{
		//close enough to catch the player, later this is what starts combat//
		return e_AI_StateID::Attack;
	}

	//arrow from the enemy to the player, shrunk to length 1 (same idea as wanderDir)//
	Vector2 dir = Vector2Normalize(Vector2Subtract(targetCentre, agentCentre));

	//same push as the WASD and wander code, the direction just points at the player//
	_agent.velocity.x += dir.x * _agent.acceleration * dt;
	_agent.velocity.y += dir.y * _agent.acceleration * dt;

	return e_AI_StateID::Seek;	//returns its own ID = stay in this state, the machine expects this instead of None//
}

void AI_State_Seek::OnExit(body& _agent)
{
}