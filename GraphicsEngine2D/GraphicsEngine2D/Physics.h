#ifndef PHYSICS_H
#define PHYSICS_H

#include <cmath>
#include <stdexcept>
#include <vector>
#include <memory>

//TODO Generalise this to nvector
class Vector2D {
protected:
	double xVec;
	double yVec;
public:
	Vector2D(const double xVecIn, const double yVecIn);
	Vector2D(const Vector2D& VecIn);
	~Vector2D();
	double getXVec()const;
	double getYVec()const;
	double getMagnitude()const;
	void setXVec(const double xVecIn);
	void setYVec(const double yVecIn);
	double dot(const Vector2D& VecIn)const;
	Vector2D operator+(const Vector2D& VecIn)const;
	Vector2D operator-(const Vector2D& VecIn)const;
	Vector2D operator*(const double ScalarIn)const;
};

struct Position {
	double X;
	double Y;
};

struct ObjectEdges {
	double UpperX;
	double LowerX;
	double UpperY;
	double LowerY;
};

class PhysicsObject {
protected:
	double Mass;
	double xPos;
	double yPos;
	Vector2D Velocity;
public:
	PhysicsObject(const double massIn, const double xPosIn, const double yPosIn, const Vector2D& VelocityIn);
	~PhysicsObject();
	double getMass()const;
	double getXPos()const;
	double getYPos()const;
	Vector2D getVelocity()const;
	virtual ObjectEdges getEdges()const = 0;
	void setXPos(const double xPosIn);
	void setYPos(const double yPosIn);
	void setVelocity(const Vector2D& VelocityIn);
};

class Ball :public PhysicsObject {
private:
	double radius;
public:
	Ball(const double massIn, const double xPosIn, const double yPosIn, const double RadiusIn, const Vector2D& VelocityIn);
	~Ball();
	ObjectEdges getEdges()const;
	double getRadius()const;
};

class PhysicsEngine {
private:
	std::vector<PhysicsObject*> WorldObjects;
	Vector2D Gravity;
	ObjectEdges WorldEdges;										//define the boundaries of the world (screen)
	void CheckBoundaries(PhysicsObject& objectIn);				//check whether an object clips outside the world
public:
	PhysicsEngine(const double UpperXEdge, const double LowerXEdge, const double UpperYEdge, const double LowerYEdge, const Vector2D& GravityIn);
	~PhysicsEngine();
	ObjectEdges getWorldedges()const;
	void AddObject(PhysicsObject* ObjectIn);
	void Update(const double TimeChange);
};

#endif