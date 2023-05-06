//
// Created by hashe on 12/3/2022.
//

#ifndef MAIN23A1_CPP_GLOBALDATAINSYSTEM_H
#define MAIN23A1_CPP_GLOBALDATAINSYSTEM_H
class GlobalData{
private:
    int topScorerId;
    int goalScored;
    int numOfAllPlayer;
public:
    GlobalData():topScorerId(-1),goalScored(-1),numOfAllPlayer(0){}

    int getTopScorerId(){return this->topScorerId;}
    int getGoalScored(){return  this->goalScored;}
    int getNumOfAllPlayer(){return this->numOfAllPlayer;}

    void setTopScorerId(int id){this->topScorerId=id;};
    void setGoalScored(int goals){this->goalScored = goals;};
    void setNumOfAllPlayer(int num){this->numOfAllPlayer=num;};
};

#endif //MAIN23A1_CPP_GLOBALDATAINSYSTEM_H
