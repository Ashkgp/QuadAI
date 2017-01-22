#ifndef VELOCITY_H
#define VELOCITY_H

/**********************************
Object to define velocity
**********************************/
class Velocity{

	float speed_x;  //Magnitude  of velocity in X direction
	float speed_y;  //Magnitude of velocity in Y direction
	public :
	//Constructors

	Velocity();
	Velocity(float speed_x,float speed_y){
		this->speed_x = speed_x;
		this->speed_y = speed_y;
	}

	Velocity(Velocity *vel){
		this->speed_x = vel->getVelocity('x');
		this->speed_y = vel ->getVelocity('y');
	}
	/*********************************************************
	Function returns value of the velocity component of 
	bot in $direction direction passed as parameter
	*********************************************************/
	float getVelocity(char direction){ 
 		return (direction == 'X' || direction == 'x')?speed_x:
			(direction == 'Y' || direction == 'y')?speed_y:0.00;
	}

	/********************************************************
	Function to change the velocity values of bot after Quad
	gave the stimulus to them.
	********************************************************/
	void changeVel(float speed_x, float speed_y){
		this->speed_x = speed_x;
		this->speed_y = speed_y;
	}

};

#endif
