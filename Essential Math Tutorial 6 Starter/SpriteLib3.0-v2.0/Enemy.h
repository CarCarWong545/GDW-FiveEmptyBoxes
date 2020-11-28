#pragma once
#include "BackEnd.h"

class Enemy
{
public:
	Enemy();
	Enemy(std::string& fileName, std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform, CanDamage* damage, bool hasPhys = false, PhysicsBody* body = nullptr);

	void InitEnemy(std::string& fileName, std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform, CanDamage* damage, bool hasPhys = false, PhysicsBody* body = nullptr);

	void Update();
	void MovementUpdate();
	void AnimationUpdate();
	

	int m_facing = 0; //left
	//Have we locked the player from moving during this animation?
	bool m_locked = false;

private:
	void SetActiveAnimation(int anim);

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//Are you currently attacking?????
	bool m_attacking = false;
	

	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;

	CanDamage* m_damage = nullptr;

	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;
	//Does this player have physics?
	bool m_hasPhysics = false;

};

