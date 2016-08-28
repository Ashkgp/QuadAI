#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
/*****************
For Test Case Generator rand()
*****************/
#include <stdlib.h>
#include <time.h>

/***********************************
Constants for size of arena.
Centre of the arena as (0,0)
***********************************/

#define MAX_X 2000 //Width of arena from -MAX_X to MAX_X
#define MAX_Y 2000 //Length of arena from -MAX_Y to MAX_Y
#define INF 9999999999.0 //Time to assign if bot is moving in correct direction. Such that Quad ignores it.
#define QUAD_SPEED 5.0 //Speed of the Quad
#define ITER 3  //Check for next n iterations.
#define PENALTY 50

struct Point{
		float x;
		float y;
};

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
		this->speed_y = vel -> speed_y;
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

/**********************************
Comparison operator for groundBot 
objects based on timetoEdge()
**********************************/
bool operator<( groundBot &a, groundBot &b){
	float time_a = a.timetoEdge();
	float time_b = b.timetoEdge();
	return (a<b);
}

bool operator==( groundBot &a, groundBot &b){
	float time_a = a.timetoEdge();
	float time_b = b.timetoEdge();
	return (time_a==time_b);
}

//This doesn't make sense but this operator is required to run the code. Don't change it.
bool operator==( groundBot &a, const int i){
	return a.timetoEdge()==i;
}
/*********************************
Calculate score of specific step 
as number of bots went out bound.
*********************************/
int calcScore(std::vector<groundBot> v){
	int score = 0;
	for(int i = 0; i < v.size() ; i++){
		if(v[i].isOutbound())
			score -= PENALTY;
	}
	return score;
}
/*********************************
Algorithm to find best bot to go to 
minimizing the penalty and time.
*********************************/
int minmax(std::vector<groundBot> botList,int pos,int depth,Point quadPosition){
	std::vector<int> score;
	int iter = 0;
	if(depth == ITER)
		return calcScore(botList);
	float time = botList[pos].timetoBot(quadPosition);
	//std::cout<<time<<std::endl;
	for(int i = 0; i < botList.size(); i++){
		botList[i].updatePosition(time);
	}
	quadPosition = botList[pos].getBotPosition();
	botList.erase(std::remove(botList.begin(),botList.end(),pos),botList.end());
	for(int i = 0; i < botList.size(); i++){
		if(botList[i].complete()){
			//??? score += 100;
			//botList.erase(std::remove(botList.begin(),botList.end(),i),botList.end());
		}
		score.push_back(minmax(botList,i,depth+1,quadPosition) + depth*5 );
		//std::cout<<"\nFor : "<<i<<std::endl;
	}
	std::sort(score.begin(),score.end()+iter);
	//std::cout<<score.back()<<std::endl;
	return score.back();
}

/***********************************
Test Case Generator
***********************************/
std::vector<groundBot> populate(){
	std::vector<groundBot> v;
	Point p;
	srand(time(NULL));

	for(int i = 0 ; i < 10; i++){
		p.x = rand()%2000 + 1;
		p.y = rand() % 2000 + 1;
		Velocity vel(rand()%2 + 1,rand()%2 + 1);
		groundBot bot(p,vel);
		v.push_back(bot);
	}
	return v;
}

int main(int argc, char **argv){
	std::vector<groundBot> botList = populate();
	//std::cout<<botList[0].timetoEdge()<<std::endl;
	Point quadPosition;
	quadPosition.x = 0;
	quadPosition.y = 0;
	// botList.push(bots);//Detect all bots and push them to botList

	//while(!botList.empty()){
		//scanbots(); //Scans the bots in visible area and updates list
		int score;
		int bestScore = -9999;
		groundBot bestchoice;
		int i,best;
		for(i = 0; i < botList.size(); i++){
			if(botList[i].complete())
				continue;
			std::cout<<i<<". ("<<botList[i].getBotPosition().x<<","<<botList[i].getBotPosition().y<<") "
					<<botList[i].timetoEdge()<<" "<<botList[i].timetoBot(quadPosition)<<" "<<botList[i].timetoEdge() + botList[i].timetoBot(quadPosition)<<std::endl;
			score = minmax(botList,i,0,quadPosition);    //Getting the score if Quad goes to ith bot
			std::cout<<i<<". "<<score<<std::endl;
			if(score >= bestScore){
				std::cout<<"\nBest changed from "<<bestScore<<" to "<<score<<std::endl;
				bestScore = score;
				bestchoice = botList[i];
				best = i;
			}
		}
		std::cout<<best<<" "<<bestchoice.getBotPosition().x<<" "<<bestchoice.getBotPosition().y<<std::endl;
		//moveQuad(bestchoice);   //Moves the bot to the best option
	//}
}


