//
// Created by hashem on 11/29/2022.
//

#ifndef WET_PLAYERIDDATA_H
#define WET_PLAYERIDDATA_H
#include "Tree.h"
#include "PlayerSpecialKEY.h"


//////////////////////////////////////FOR NODES IN TREE NUM1


class TeamsData;


class PlayersData{
private:
    int gamesPlayed;
    int goalsScored;
    int cardsReceived;
    bool goalKeeper;
    bool headInTeam;
    int playerId;
    Tree<int,PlayersData*>* playerSortedById;///maybe  no need
    Tree<PlayerSpecialKey,PlayersData*>* playerSortedByEquality;///maybe no need
   // NodeInTree<int,TeamsData*>* teamBelongTO;
    TeamsData* teamBelongTO;
    //NodeInTree<PlayerSpecialKey,PlayersData*>* previousInOrder;
   // NodeInTree<PlayerSpecialKey,PlayersData*>* nextInOrder;
    PlayersData* previousInOrder;
    PlayersData* nextInOrder;
public:
    PlayersData():gamesPlayed(0),
                  goalsScored(0),
                  cardsReceived(0),
                  goalKeeper(false),
                  headInTeam(false),
                  playerId(0),
                  playerSortedById(nullptr),
                  playerSortedByEquality(nullptr),
                  teamBelongTO(nullptr),
                  previousInOrder(nullptr),
                  nextInOrder(nullptr){}
    PlayersData(int games_Played,int goals, int cards,int id, bool goalKeeper = false):
                    gamesPlayed(games_Played),
                    goalsScored(goals),
                    cardsReceived(cards),
                    goalKeeper(goalKeeper),
                    headInTeam(false),
                    playerId(id),
                    playerSortedById(nullptr),
                    playerSortedByEquality(nullptr),
                    teamBelongTO(nullptr),
                    previousInOrder(nullptr),
                    nextInOrder(nullptr){}

    int getPlayerId() const { return this->playerId;}
    int getGamesPlayed() const {return this->gamesPlayed;}
    int getGoals() const {return this->goalsScored;}
    int getCards() const {return this->cardsReceived;}
    bool isGoalKeeper() const {return  this->goalKeeper;}
    bool isHeadInTeam() const {return this->headInTeam;}
    Tree<int,PlayersData*>*  getPlayerSortedById() const {return this->playerSortedById;}
    Tree<PlayerSpecialKey,PlayersData*>* getPlayerSortedByEquality() const {return this->playerSortedByEquality;}
    //NodeInTree<int,TeamsData*>* getTeamBelongTO() const {return this->teamBelongTO;}
    TeamsData* getTeamBelongTO() const {return this->teamBelongTO;}
   // NodeInTree<PlayerSpecialKey,PlayersData*>* getPreviousInOrder() const {return this->previousInOrder;}
   // NodeInTree<PlayerSpecialKey,PlayersData*>* getNextInOrder() const { return this->nextInOrder;}
   PlayersData* getPreviousInOrder() const {return this->previousInOrder;}
   PlayersData* getNextInOrder() const { return this->nextInOrder;}


    void setGamesPlayed(int games_Played) { this->gamesPlayed = games_Played;}
    void setGoals(int goals_Scored) { this->goalsScored = goals_Scored;}
    void setCards(int cards_Received) { this->cardsReceived = cards_Received;}
    void setGoalKeeper(bool goal_Keeper) {  this->goalKeeper = goal_Keeper;}
    void setHeadInTeam(bool headIn_Team) { this->headInTeam = headIn_Team;}
    void setPlayerSortedById(Tree<int,PlayersData*>*  PlayerSortedBy_Id) { this->playerSortedById = PlayerSortedBy_Id;}
    void setPlayerSortedByEquality(Tree<PlayerSpecialKey,PlayersData*>* playerSortedBy_Equality){
        this->playerSortedByEquality =playerSortedBy_Equality; }
    void setTeamBelongTO(TeamsData* teamBelong_TO) { this->teamBelongTO = teamBelong_TO;}
   // void setPreviousInOrder(NodeInTree<PlayerSpecialKey,PlayersData*>* PreviousInOrder){ this->previousInOrder = PreviousInOrder;}
   // void setNextInOrder(NodeInTree<PlayerSpecialKey,PlayersData*>* NextInOrder){ this->nextInOrder = NextInOrder;}
     void setPreviousInOrder(PlayersData* PreviousInOrder){ this->previousInOrder = PreviousInOrder;}
     void setNextInOrder(PlayersData* NextInOrder){ this->nextInOrder = NextInOrder;}

};


#endif //WET_PLAYERIDDATA_H
