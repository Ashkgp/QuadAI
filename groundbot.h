#ifndef GROUNDBOT_H
#define GROUNDBOT_H

#include "arena.h"
#include "velocity.h"
#include <math.h>
#include <algorithm>

struct Point{
		float x;
		float y;
};


/***********************************
Object for ground bots
***********************************/
class groundBot{
	Point p; //Relative position of ground bot with respect to the center
	Velocity *vel;
	public :

	//Constructors
	groundBot(){

	}
	groundBot(Point p, Velocity vel){
		this->p = p;
		this->vel = new Velocity(vel);
	}

	//Public Funtions 

	float timetoEdge(){ //Time before bot goes out of bounds
		float time_x , time_y; 
		float dist_x,dist_y;  //Distance in X and Y from the bound

		if(vel->getVelocity('x') < 0.0)
			dist_x = p.x + MAX_X;
		else
			dist_x = MAX_X - p.x;
		if(vel->getVelocity('x') == 0.0)
			time_x = INF;
		else
			time_x = dist_x/(abs(vel->getVelocity('x')));
		if(vel->getVelocity('y') < 0.0){
			dist_y = p.y + MAX_Y;
			time_y = dist_y/(abs(vel->getVelocity('y')));
		}
		else
			time_y = INF;

		return std::min(time_y,time_x);	
	} 

	/***********************************
	Returns the time taken by the Quad 
	to reach a specific bot on the ground.
	***********************************/
	float timetoBot(Point quadPosition){
		float dist_x = quadPosition.x - p.x;
		float dist_y = quadPosition.y - p.y;
		float dist = sqrt(dist_x*dist_x + dist_y*dist_y);
		return dist/QUAD_SPEED;
	}

	//Function to update position of bots.
	void updatePosition(float time){
		p.x += time*vel->getVelocity('x');
		p.y += time*vel->getVelocity('y');
		//std::cout<<"Done\n";
	}

	//Gets current bot position
	Point getBotPosition(){
		return p;
	}

	//Checks if bot is out of bound
	bool isOutbound(){
		return (p.x > MAX_X || p.x < -MAX_X || p.y < -MAX_Y);
	}

	//Check if bot cleared finish line
	bool complete(){
		return (!isOutbound() && p.y > MAX_Y);
	}

};

#endif