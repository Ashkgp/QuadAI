#include "AI.h"

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
			//std::cout<<i<<". "<<score<<std::endl;
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


