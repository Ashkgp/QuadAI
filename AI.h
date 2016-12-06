#ifndef AI_H
#define AI_H

#include "groundbot.h"
#include "velocity.h"
#include <iostream>
#include <vector>
/*****************
For Test Case Generator rand()
*****************/
#include <stdlib.h>
#include <time.h>


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

//This doesn't make sense but this operator is required to run the code. Don't change it unless you like bugs.
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
			//i--;
			//continue;
		}
		score.push_back(minmax(botList,i,depth+1,quadPosition));
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

#endif