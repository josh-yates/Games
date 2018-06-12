#include "Physics.h"

Vector2D::Vector2D(const double xVecIn, const double yVecIn) :xVec(xVecIn), yVec(yVecIn) {}
Vector2D::Vector2D(const Vector2D& VecIn) : xVec(VecIn.xVec), yVec(VecIn.yVec) {}
Vector2D::~Vector2D() {}
double Vector2D::getXVec()const {
	return xVec;
}
double Vector2D::getYVec()const {
	return yVec;
}
double Vector2D::getMagnitude()const {
	return sqrt((pow(xVec, 2)) + (pow(yVec, 2)));
}
void Vector2D::setXVec(const double xVecIn) {
	xVec = xVecIn;
}
void Vector2D::setYVec(const double yVecIn) {
	yVec = yVecIn;
}
double Vector2D::dot(const Vector2D& VecIn)const {
	return (xVec*VecIn.xVec) + (yVec*VecIn.yVec);
}
Vector2D Vector2D::operator+(const Vector2D& VecIn)const {
	Vector2D temp(xVec + VecIn.xVec, yVec + VecIn.yVec);
	return temp;
}
Vector2D Vector2D::operator-(const Vector2D& VecIn)const {
	Vector2D temp(xVec - VecIn.xVec, yVec - VecIn.yVec);
	return temp;
}
Vector2D Vector2D::operator*(const double ScalarIn)const {
	Vector2D temp(ScalarIn*xVec, ScalarIn*yVec);
	return temp;
}

PhysicsObject::PhysicsObject(const double massIn, const double xPosIn, const double yPosIn, const Vector2D& VelocityIn) :Mass(massIn), xPos(xPosIn), yPos(yPosIn), Velocity(VelocityIn) {}
PhysicsObject::~PhysicsObject() {}
double PhysicsObject::getMass()const {
	return Mass;
}
double PhysicsObject::getXPos()const {
	return xPos;
}
double PhysicsObject::getYPos()const {
	return yPos;
}
Vector2D PhysicsObject::getVelocity()const {
	return Velocity;
}
void PhysicsObject::setXPos(const double xPosIn) {
	xPos = xPosIn;
}
void PhysicsObject::setYPos(const double yPosIn) {
	yPos = yPosIn;
}
void PhysicsObject::setVelocity(const Vector2D& VelocityIn) {
	Velocity = VelocityIn;
}

Ball::Ball(const double massIn, const double xPosIn, const double yPosIn, const double RadiusIn, const Vector2D& VelocityIn) :PhysicsObject(massIn, xPosIn, yPosIn, VelocityIn), radius(sqrt(pow(RadiusIn, 2))) {}
Ball::~Ball() {}
ObjectEdges Ball::getEdges()const {
	ObjectEdges tempEdges;
	tempEdges.LowerX = xPos - radius;
	tempEdges.UpperX = xPos + radius;
	tempEdges.LowerY = yPos - radius;
	tempEdges.UpperY = yPos + radius;
	return tempEdges;
}
double Ball::getRadius()const {
	return radius;
}

PhysicsEngine::PhysicsEngine(const double UpperXEdge, const double LowerXEdge, const double UpperYEdge, const double LowerYEdge, const Vector2D& GravityIn):Gravity(GravityIn) {
	if (UpperXEdge == LowerXEdge || UpperYEdge == LowerYEdge) {
		throw std::invalid_argument("World edges cannot be equal");
	}
	ObjectEdges tempEdges;
	//Auto-find the upper and lower values - prevents misuse
	if (UpperXEdge > LowerXEdge) {
		tempEdges.LowerX = LowerXEdge;
		tempEdges.UpperX = UpperXEdge;
	}
	else {
		tempEdges.LowerX = UpperXEdge;
		tempEdges.UpperX = LowerXEdge;
	}
	if (UpperYEdge > LowerYEdge) {
		tempEdges.LowerY = LowerYEdge;
		tempEdges.UpperY = UpperYEdge;
	}
	else {
		tempEdges.LowerY = UpperYEdge;
		tempEdges.UpperY = LowerYEdge;
	}
	WorldEdges = tempEdges;
}
PhysicsEngine::~PhysicsEngine() {}
ObjectEdges PhysicsEngine::getWorldedges()const {
	return WorldEdges;
}
void PhysicsEngine::AddObject(PhysicsObject* ObjectIn) {
	WorldObjects.push_back(ObjectIn);
}
void PhysicsEngine::CheckBoundaries(PhysicsObject& objectIn) {
	//if clipped, shift so edges touch and reverse that respective velocity as if elastic collision
	if (objectIn.getEdges().LowerX < WorldEdges.LowerX) {
		objectIn.setXPos(objectIn.getXPos() + WorldEdges.LowerX - objectIn.getEdges().LowerX);
		//if trying to go further into lower x edge, reverse direction -> should prevent sticking
		if (objectIn.getVelocity().getXVec() < 0.0) {
			Vector2D newVelocity(objectIn.getVelocity().getXVec()*-1, objectIn.getVelocity().getYVec());
			objectIn.setVelocity(newVelocity);
		}
	}
	if (objectIn.getEdges().UpperX > WorldEdges.UpperX) {
		objectIn.setXPos(objectIn.getXPos() - (objectIn.getEdges().UpperX - WorldEdges.UpperX));
		//if trying to go further into upper x edge, reverse direction -> should prevent sticking
		if (objectIn.getVelocity().getXVec() > 0.0) {
			Vector2D newVelocity(objectIn.getVelocity().getXVec()*-1, objectIn.getVelocity().getYVec());
			objectIn.setVelocity(newVelocity);
		}
	}
	if (objectIn.getEdges().LowerY < WorldEdges.LowerY) {
		objectIn.setYPos(objectIn.getYPos() + WorldEdges.LowerY - objectIn.getEdges().LowerY);
		//if trying to go further into lower y edge, reverse direction -> should prevent sticking
		if (objectIn.getVelocity().getYVec() < 0.0) {
			Vector2D newVelocity(objectIn.getVelocity().getXVec(), objectIn.getVelocity().getYVec()*-1);
			objectIn.setVelocity(newVelocity);
		}
	}
	if (objectIn.getEdges().UpperY > WorldEdges.UpperY) {
		objectIn.setYPos(objectIn.getYPos() - (objectIn.getEdges().UpperY - WorldEdges.UpperY));
		//if trying to go further into upper y edge, reverse direction -> should prevent sticking
		if (objectIn.getVelocity().getYVec() > 0.0) {
			Vector2D newVelocity(objectIn.getVelocity().getXVec(), objectIn.getVelocity().getYVec()*-1);
			objectIn.setVelocity(newVelocity);
		}
	}
}

void PhysicsEngine::Update(const double TimeChange) {
	for (auto iter{ WorldObjects.begin() }; iter != WorldObjects.end(); iter++) {
		CheckBoundaries(**iter);
		//calculate new velocity and position
		(*iter)->setXPos(((*iter)->getVelocity().getXVec()*TimeChange) + (*iter)->getXPos());
		(*iter)->setYPos(((*iter)->getVelocity().getYVec()*TimeChange) + (*iter)->getYPos());
		Vector2D newVelocity{ (Gravity*TimeChange) + (*iter)->getVelocity() };
		(*iter)->setVelocity(newVelocity);
	}
}