//
// Created by hashe on 11/29/2022.
//

#ifndef WET_TEAMSIDKEY_H
#define WET_TEAMSIDKEY_H
#include "Tree.h"
#include "playerData.h"



class TeamsData{
private:
    int points;
    int goalsMinusCards;
    int gameTeamPlayed;
    int topScorerID;
    int topScorerGoalsNum;
    int playersNumber;
    int GoalKeepers;
    int teamID;
    Tree<int, PlayersData*>* headTeamByID;
    Tree<PlayerSpecialKey,PlayersData*>* headTeamByEquality;
public:
    TeamsData():points(0),
                goalsMinusCards(0),
                gameTeamPlayed(0),
                topScorerID(0),
                topScorerGoalsNum(0),
                playersNumber(0),
                GoalKeepers(0),
                teamID(0),
                headTeamByID(nullptr),
                headTeamByEquality(nullptr){}

    TeamsData(int Points,int ID):points(Points),
                goalsMinusCards(0),
                gameTeamPlayed(0),
                topScorerID(0),
                topScorerGoalsNum(0),
                playersNumber(0),
                GoalKeepers(0),
                teamID(ID),
                headTeamByID(nullptr),
                headTeamByEquality(nullptr){}


    int getTeamId()const{return this->teamID;}
    int getPoints() const {return this->points;}
    int getGoalsMinusCards() const {return this->goalsMinusCards;}
    int getGameTeamPlayed() const {return this->gameTeamPlayed;}
    int getTopScorerID() const {return this->topScorerID;}
    int getTopScorerGoalsNum() const {return this->topScorerGoalsNum;}
    int getPlayersNumber() const {return this->playersNumber;}
    int getIsTeamGoalKeeperNum() const {return this->GoalKeepers;}
    Tree<int,PlayersData*>* getHeadTeamById() const {return this->headTeamByID;}
    Tree<PlayerSpecialKey,PlayersData*>* getHeadTeamByEquality()const {return this->headTeamByEquality;}


    void setPoints(int Points) { this->points = Points;}
    void setGoalsMinusCards(int GoalsMinusCards) { this->goalsMinusCards = GoalsMinusCards;}
    void setGameTeamPlayed(int GameTeamPlayed) { this->gameTeamPlayed= GameTeamPlayed;}
    void setTopScorerID(int TopScorerID) { this->topScorerID = TopScorerID;}
    void setTopScorerGoalsNum(int TopScorerGoalsNum) {  this->topScorerGoalsNum = TopScorerGoalsNum;}
    void setPlayersNumber(int PlayersNumber) { this->playersNumber = PlayersNumber;}
    void  setTeamGoalKeeperNum(int goalKeeper) { this->GoalKeepers = goalKeeper;}
    void setHeadTeamById(Tree<int,PlayersData*>* head){ this->headTeamByID =head; }
    void setHeadTeamByEquality(Tree<PlayerSpecialKey,PlayersData*>* head) { this->headTeamByEquality = head;}

    void removePlayerFromTeam(){
        this->playersNumber--;
    };
    void removeGoalKeeper(){
        this->GoalKeepers--;
    };

    void teamWon() {
        this->points = this->points +3;
    }

    void draw(){
        this->points = this->points +1;
    }

    void teamPlayed(){
        this->gameTeamPlayed = this->gameTeamPlayed+1;
    }

    void operator+=(TeamsData* t)
    {
        this->points += t->getPoints();
        this->playersNumber += t->getPlayersNumber();
        this->goalsMinusCards +=t->getGoalsMinusCards();
        this->GoalKeepers += t->getIsTeamGoalKeeperNum();
    }

};



#endif //WET_TEAMSIDKEY_H
