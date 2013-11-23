#include "GameObject.hpp"

using namespace Ai;
using namespace Math;

/*GameObject::GameObject(Vec2 _position,Vec2 _direction,Vec2 _goal,float _radius,float _energy,int _shapeIndex) : m_position(_position), m_direction(_direction), m_goal(_goal),m_radius(_radius),m_energy(_energy),m_shapeIndex(_shapeIndex){
}

GameObject::~GameObject(){
}*/

void GameObject::setDirection(Vec2 _newDirection){
	m_direction = _newDirection;
}

void GameObject::setEnergy(float _newEnergy){
	m_energy = _newEnergy;
}

void GameObject::setGoal(Vec2 _newGoal){
	m_goal = _newGoal;
}

void GameObject::setPosition(Vec2 _newPosition){
	m_position = _newPosition;
}

void GameObject::setRadius(float _newRadius){
	m_radius = _newRadius;
}

Vec2 GameObject::getDiretion(){
	return m_direction;
}

Vec2 GameObject::getGoal(){
	return m_goal;
}

Vec2 GameObject::getPosition(){
	return m_position;
}

float GameObject::getEnergy(){
	return m_energy;
}

float GameObject::getRadius(){
	return m_radius;
}

float GameObject::getKineticEnegy(){
	return m_direction.length()*m_energy;
}

void GameObject::update(){
	m_position = m_position + m_direction;
}

float GameObject::getShinyRadius(){
	return m_radius*0.5f*m_energy;
}

int GameObject::getShapeIndex(){
	return m_shapeIndex;
}

void GameObject::setShapeIndex(int _shapeIndex){
	m_shapeIndex = _shapeIndex;
}