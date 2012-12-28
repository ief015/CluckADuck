#pragma once
#include "../vec2.hpp"

#include <SFML/Graphics.hpp>

class Entity
{
protected:
	vec2 pos;
	vec2 vel;
	double boundsRadius; // radius of collision

public:
	Entity()
	{
		pos.x = 0.; pos.y = 0.;
		vel.x = 0.; vel.y = 0.;
		boundsRadius = 0.;
	}
	virtual ~Entity()
	{}

	inline const vec2& getPos() const { return pos; }
	inline void setPos(const vec2& v) { pos.x = v.x; pos.y = v.y; }
	inline void setPos(double x, double y) { pos.x = x; pos.y = y; }
	inline void setPosX(double x) { pos.x = x; }
	inline void setPosY(double y) {pos.y = y; }

	inline const vec2& getVel() const { return vel; }
	inline void setVel(const vec2& v) { vel.x = v.x; vel.y = v.y; }
	inline void setVel(double x, double y) { vel.x = x; vel.y = y; }
	inline void setVelX(double x) { vel.x = x; }
	inline void setVelY(double y) {vel.y = y; }

	inline double getBoundsRadius() const { return boundsRadius; }

	bool checkAABBCollision(const Entity& otherEnt) const
	{
		return ((otherEnt.pos.x + otherEnt.boundsRadius) >= (this->pos.x - this->boundsRadius) &&
				(otherEnt.pos.x - otherEnt.boundsRadius) <= (this->pos.x + this->boundsRadius) &&
				(otherEnt.pos.y + otherEnt.boundsRadius) >= (this->pos.y - this->boundsRadius) &&
				(otherEnt.pos.y - otherEnt.boundsRadius) <= (this->pos.y + this->boundsRadius) );
	}

	bool checkRadiusCollision(const Entity& otherEnt) const
	{
		double diffx = this->pos.x - otherEnt.pos.x;
		double diffy = this->pos.y - otherEnt.pos.y;
		double radii = this->boundsRadius + otherEnt.boundsRadius;
		return ((diffx*diffx + diffy*diffy) <= (radii*radii));
	}

	virtual void update(float dt)
	{
		pos.x += vel.x;
		pos.y += vel.y;
	}
	virtual void draw(sf::RenderTarget& rt)
	{}
};
