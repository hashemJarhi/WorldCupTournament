#include "worldcup23a1.h"

////////////////////////////////////add the messing data to tree for teams by power



static void update_closest(PlayersData* playerToFix, Tree<PlayerSpecialKey, PlayersData*>* allPlayersSortedBySpecialId)
{

    PlayerSpecialKey playerByEqualityKey =  PlayerSpecialKey(playerToFix->getGoals(),playerToFix->getCards(),playerToFix->getPlayerId());
    NodeInTree<PlayerSpecialKey,PlayersData*>* playerToUpdateClosest = allPlayersSortedBySpecialId->find(playerByEqualityKey);
    if(playerToUpdateClosest == nullptr) return;

    bool gotBigger = false;
    bool gotSmaller = false;

    if (playerToUpdateClosest->hasTwoSons())
    {
        NodeInTree<PlayerSpecialKey,PlayersData*>* leftSon = playerToUpdateClosest->leftSon;//to get lowest
        NodeInTree<PlayerSpecialKey,PlayersData*>* rightSon = playerToUpdateClosest->rightSon;//to get higher
        while(leftSon->rightSon != nullptr)
        {
            leftSon =leftSon->rightSon;
        }
        while(rightSon->leftSon != nullptr){
            rightSon =rightSon->leftSon;
        }
        (*(playerToUpdateClosest->data))->setNextInOrder(*rightSon->data);
        (*(rightSon->data))->setPreviousInOrder(*playerToUpdateClosest->data);
        (*(playerToUpdateClosest->data))->setPreviousInOrder((*leftSon->data));
        (*(leftSon->data))->setNextInOrder(*playerToUpdateClosest->data);
        return;
    }

    if(playerToUpdateClosest->hasOneSon())
    {
        if(playerToUpdateClosest->hasLeftSon())
        {
            NodeInTree<PlayerSpecialKey,PlayersData*>* leftSon = playerToUpdateClosest->leftSon;
            while(leftSon->rightSon != nullptr)
            {
                leftSon =leftSon->rightSon;
            }
            (*(playerToUpdateClosest->data))->setPreviousInOrder((*leftSon->data));
            (*(leftSon->data))->setNextInOrder(*playerToUpdateClosest->data);
            NodeInTree<PlayerSpecialKey,PlayersData*>* father = playerToUpdateClosest->father;
            //if(father == nullptr)  (*(playerToUpdateClosest->data))->setNextInOrder((nullptr));
            //else{
                while (father != nullptr && !gotBigger)
                {
                    if(*father->key > *playerToUpdateClosest->key)
                    {
                        (*(playerToUpdateClosest->data))->setNextInOrder(*father->data);
                        (*(father->data))->setPreviousInOrder(*playerToUpdateClosest->data);
                        gotBigger = true;
                        break;
                    }
                    father = father->father;
                }
           // }
            if(!gotBigger) (*(playerToUpdateClosest->data))->setNextInOrder(nullptr);
            return;
        }else {
            NodeInTree<PlayerSpecialKey,PlayersData*>* rightSon = playerToUpdateClosest->rightSon;
            while (rightSon->leftSon != nullptr)
            {
                rightSon = rightSon->leftSon;
            }
            (*(playerToUpdateClosest->data))->setNextInOrder((*rightSon->data));
            (*(rightSon->data))->setPreviousInOrder(*playerToUpdateClosest->data);
            NodeInTree<PlayerSpecialKey,PlayersData*>* father = playerToUpdateClosest->father;
            //if (father == nullptr) (*(playerToUpdateClosest->data))->setPreviousInOrder((nullptr));
            //else {
                while (father != nullptr && !gotSmaller)
                {
                    if (*father->key < *playerToUpdateClosest->key)
                    {
                        (*(playerToUpdateClosest->data))->setPreviousInOrder(*father->data);
                        (*(father->data))->setNextInOrder(*playerToUpdateClosest->data);
                        gotSmaller = true;
                        break;
                    }
                    father = father->father;
                }
           // }
            if(!gotSmaller) (*(playerToUpdateClosest->data))->setPreviousInOrder(nullptr);
            return;
        }
    }

    if(playerToUpdateClosest->isLeaf())
    {
        NodeInTree<PlayerSpecialKey,PlayersData*>* father = playerToUpdateClosest->father;
        while (father != nullptr)
        {
            if (*father->key > *playerToUpdateClosest->key && !gotBigger)
            {
                (*(playerToUpdateClosest->data))->setNextInOrder(*father->data);
                (*(father->data))->setPreviousInOrder(*playerToUpdateClosest->data);
                gotBigger = true;
            }
            if (*father->key < *playerToUpdateClosest->key && !gotSmaller)
            {
                (*(playerToUpdateClosest->data))->setPreviousInOrder(*father->data);
                (*(father->data))->setNextInOrder(*playerToUpdateClosest->data);
                gotSmaller = true;
            }
            father = father->father;
        }
    }
    if(!gotBigger)
    {
        (*(playerToUpdateClosest->data))->setNextInOrder(nullptr);

    }
    if(!gotSmaller)
    {
        (*(playerToUpdateClosest->data))->setPreviousInOrder(nullptr);
    }
}





world_cup_t::world_cup_t()
{
    this->allPlayersSortedById = new Tree<int,PlayersData*>();
    this->allPlayersSortedBySpecialId = new Tree<PlayerSpecialKey,PlayersData*>();
    this->allTeamsSortedById = new Tree<int, TeamsData*>();
    this->teamsToChallenge = new Tree<int,int>();
    this->globalData = new GlobalData();
}

void postOrderDelete(NodeInTree<int,TeamsData*>* node)
{
    if (node == nullptr)
        return;
    postOrderDelete(node->leftSon);
    postOrderDelete(node->rightSon);
    Tree<int,PlayersData*>* toDeleteById = (*node->data)->getHeadTeamById();
    if(toDeleteById != nullptr )
    {
        toDeleteById->treeClear();
        delete toDeleteById;
    }
    Tree<PlayerSpecialKey,PlayersData*>* toDeleteTreeByQuality = (*node->data)->getHeadTeamByEquality();
    if(toDeleteTreeByQuality != nullptr)
    {
        toDeleteTreeByQuality->treeClear();
        delete toDeleteTreeByQuality;
    }
    TeamsData* deleteData = *node->data;
    delete deleteData;
}

void postOrderDeleteData(NodeInTree<int, PlayersData*>* node)
{
    if (node == nullptr)
        return;
    postOrderDeleteData(node->leftSon);
    postOrderDeleteData(node->rightSon);
    PlayersData* deleteData = *node->data;
    delete deleteData;
}

world_cup_t::~world_cup_t()
{
    NodeInTree<int,TeamsData*>* rootTeam = allTeamsSortedById->getRoot();
    postOrderDelete(rootTeam);

    NodeInTree<int,PlayersData*>* root = allPlayersSortedById->getRoot();
    postOrderDeleteData(root);

    delete this->teamsToChallenge;
	delete this->allTeamsSortedById;
    delete this->allPlayersSortedBySpecialId;
    delete this->allPlayersSortedById;
    delete this->globalData;
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId<=0 || points<0) return StatusType::INVALID_INPUT;
   NodeInTree<int,TeamsData*>* team = allTeamsSortedById->find(teamId);
   if(team != nullptr) return StatusType::FAILURE;
    try{
        TeamsData* newTeam =  new TeamsData(points,teamId);
        allTeamsSortedById->insert(teamId,newTeam);
    }
    catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0 )return StatusType::INVALID_INPUT;
    NodeInTree<int,TeamsData*>* newTeam = allTeamsSortedById->find(teamId);
    if(newTeam == nullptr || (*(newTeam->data))->getPlayersNumber() != 0) return StatusType::FAILURE;
    static const int LEGAL_PLAYER_NUM_TO_CHALLENGE = 11;
    if((*newTeam->data)->getPlayersNumber() >=LEGAL_PLAYER_NUM_TO_CHALLENGE && (*newTeam->data)->getIsTeamGoalKeeperNum() >0 )
    {
        teamsToChallenge->remove(teamId);
    }
    delete *newTeam->data;
    allTeamsSortedById->remove(teamId);
	return StatusType::SUCCESS;
}


//////////////////////add player to the global counter
StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(playerId<=0 || teamId<=0 || gamesPlayed <0|| goals<0 || cards<0) return StatusType::INVALID_INPUT;
    if(gamesPlayed==0 && (goals >0 || cards >0)) return StatusType::INVALID_INPUT;

    NodeInTree<int,TeamsData*>* teamNode = allTeamsSortedById->find(teamId);
    if(teamNode == nullptr || allPlayersSortedById->find(playerId) != nullptr ) return StatusType::FAILURE;
    TeamsData* currTeam = *teamNode->data;
    gamesPlayed -= currTeam->getGameTeamPlayed();


    try{
        PlayersData* player = new PlayersData(gamesPlayed,goals,cards,playerId,goalKeeper);
        player->setTeamBelongTO(currTeam);
        //player->setGamesPlayed(gamesPlayed - (*(currTeam->data))->getGameTeamPlayed());
        PlayerSpecialKey* playerByEqualityKey = new PlayerSpecialKey(goals, cards, playerId);
        if((allPlayersSortedById->find(playerId)) != nullptr) return StatusType::FAILURE;
        allPlayersSortedById->insert(playerId,player);
        allPlayersSortedBySpecialId->insert(*playerByEqualityKey,player);
        if(currTeam->getPlayersNumber() == 0)
        {
            currTeam->setPlayersNumber(1);
            if(goalKeeper)
                currTeam->setTeamGoalKeeperNum(1);
            currTeam->setGoalsMinusCards(goals - cards);
            currTeam->setTopScorerGoalsNum(goals);
            currTeam->setTopScorerID(playerId);

            Tree<int, PlayersData*>* treeById = new Tree<int,PlayersData*>();
            treeById->insert(playerId, player);
            currTeam->setHeadTeamById(treeById);

            Tree<PlayerSpecialKey, PlayersData*>* treeByEquality = new Tree<PlayerSpecialKey,PlayersData*>();
            treeByEquality->insert(*playerByEqualityKey, player);
            currTeam->setHeadTeamByEquality(treeByEquality);

        }
        else{
            currTeam->setPlayersNumber(currTeam->getPlayersNumber() + 1);
            if (goalKeeper) currTeam->setTeamGoalKeeperNum(currTeam->getIsTeamGoalKeeperNum() + 1);
            currTeam->setGoalsMinusCards(currTeam->getGoalsMinusCards() + goals - cards);

            static const int LEGAL_PLAYER_NUM_TO_CHALLENGE = 11;
            if(currTeam->getPlayersNumber() >=LEGAL_PLAYER_NUM_TO_CHALLENGE && currTeam->getIsTeamGoalKeeperNum()>0)
            {
                int power = currTeam->getPoints() + currTeam->getGoalsMinusCards();
                if(teamsToChallenge->find(teamId) != nullptr)
                {
                    teamsToChallenge->remove(teamId);
                }
                teamsToChallenge->insert(teamId,power);
            }


            currTeam->getHeadTeamById()->insert(playerId,player);
            currTeam->getHeadTeamByEquality()->insert(*playerByEqualityKey,player);
            if(currTeam->getTopScorerGoalsNum() < goals)
            {
                currTeam->setTopScorerGoalsNum(goals);
                currTeam->setTopScorerID(playerId);
            }else if(currTeam->getTopScorerGoalsNum() == goals)
            {
                int tmp = currTeam->getTopScorerID();
                if(cards < (*allPlayersSortedById->find(tmp)->data)->getCards())
                {
                    currTeam->setTopScorerGoalsNum(goals);
                    currTeam->setTopScorerID(playerId);
                }else if (cards == (*allPlayersSortedById->find(tmp)->data)->getCards()){
                    if(tmp < playerId)
                    {
                        currTeam->setTopScorerGoalsNum(goals);
                        currTeam->setTopScorerID(playerId);
                    }
                }
            }
        }
        /////update for the closest
        update_closest(player,allPlayersSortedBySpecialId);
        //playerByEqualityKey = nullptr;
        delete playerByEqualityKey;
    }
    catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

    if(globalData->getGoalScored() < goals)
    {
        globalData->setGoalScored(goals);
        globalData->setTopScorerId(playerId);
    }

    globalData->setNumOfAllPlayer(globalData->getNumOfAllPlayer() +1);
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if (playerId<= 0) return  StatusType::INVALID_INPUT;
    NodeInTree<int,PlayersData*>* playerNode = allPlayersSortedById->find(playerId);
    if( playerNode == nullptr) return StatusType::FAILURE;
    PlayersData* player = *playerNode->data;
    try{
        PlayerSpecialKey playerKey = PlayerSpecialKey(player->getGoals(),
                                          player->getCards(),
                                              playerId);

        PlayersData* playerForLeft =player->getPreviousInOrder();
        PlayersData* playerForRight = player->getNextInOrder();
        if(playerForLeft != nullptr && playerForRight != nullptr){
            playerForLeft->setNextInOrder(playerForRight);
            playerForRight->setPreviousInOrder(playerForLeft);
        }else if(playerForLeft == nullptr && playerForRight != nullptr){
            playerForRight->setPreviousInOrder(nullptr);
        }else if(playerForLeft != nullptr && playerForRight == nullptr){
            playerForLeft->setNextInOrder(nullptr);
        }


        player->setPreviousInOrder(nullptr);
        player->setNextInOrder(nullptr);

        if(globalData->getTopScorerId() == playerId)
        {
            if (playerForLeft != nullptr)
            {
                globalData->setTopScorerId(playerForLeft->getPlayerId());
                globalData->setGoalScored(playerForLeft->getGoals());
            }
        }

        TeamsData* teamThatBelong = player->getTeamBelongTO();
        player->setTeamBelongTO(nullptr);
        teamThatBelong->removePlayerFromTeam();
        if(player->isGoalKeeper()) teamThatBelong->removeGoalKeeper();

        int oldGoalMinusCards= teamThatBelong->getGoalsMinusCards();

       teamThatBelong->
        setGoalsMinusCards(oldGoalMinusCards-playerKey.getGoals()+playerKey.getCards());
        Tree<PlayerSpecialKey,PlayersData*>* qualityTree =  teamThatBelong->getHeadTeamByEquality();
        Tree<int,PlayersData*>* iDTree = teamThatBelong->getHeadTeamById();
        qualityTree->remove(playerKey);
        iDTree->remove(playerId);
        if(teamThatBelong->getPlayersNumber() <=0)
        {
            delete qualityTree;
            delete iDTree;
            teamThatBelong->setHeadTeamByEquality(nullptr);
            teamThatBelong->setHeadTeamById(nullptr);
        }

        int tmpTeamId = teamThatBelong->getTeamId();
        if(teamThatBelong->getPlayersNumber() < 11||teamThatBelong->getIsTeamGoalKeeperNum() <1 )
        {
            teamsToChallenge->remove(tmpTeamId);
        }else{
            int power = teamThatBelong->getPoints() + teamThatBelong->getGoalsMinusCards();
            teamsToChallenge->remove(tmpTeamId);
            teamsToChallenge->insert(tmpTeamId,power);
        }

        if(teamThatBelong->getPlayersNumber() > 0)
        {
            if(teamThatBelong->getTopScorerID() == playerId)
            {
                NodeInTree<PlayerSpecialKey,PlayersData*>* nextMax =teamThatBelong->getHeadTeamByEquality()->getRoot();
                while (nextMax->rightSon != nullptr)
                {
                    nextMax = nextMax->rightSon;
                }
                teamThatBelong->setTopScorerID((*nextMax->data)->getPlayerId());
                teamThatBelong->setTopScorerGoalsNum((*nextMax->data)->getGoals());
            }
        }
        //PlayerSpecialKey* keyToDelete = allPlayersSortedBySpecialId->find(playerKey)->key;
        allPlayersSortedBySpecialId->remove(playerKey);
        allPlayersSortedById->remove(playerId);
        globalData->setNumOfAllPlayer(globalData->getNumOfAllPlayer() - 1);
        delete player;
       //8 delete keyToDelete;

    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
    if(playerId <=0 || gamesPlayed <0 || scoredGoals <0 || cardsReceived <0 ) return StatusType::INVALID_INPUT;
    try{
        NodeInTree<int,PlayersData*>* playerNode = allPlayersSortedById->find(playerId);
        if( playerNode == nullptr) return StatusType::FAILURE;
        //NodeInTree<int,PlayersData*>* oldPlayerStats = allPlayersSortedById->find(playerId);
        PlayersData* player = *playerNode->data;

        PlayerSpecialKey* oldKey = new PlayerSpecialKey( player->getGoals(),player->getCards(),playerId);
        if(globalData->getTopScorerId() == playerId)
        {
            globalData->setGoalScored(player->getGoals() + scoredGoals);
        }else{
            if(globalData->getGoalScored() < scoredGoals + player->getGoals())
            {
                globalData->setGoalScored(scoredGoals + player->getGoals());
                globalData->setTopScorerId(playerId);
            }
        }
        TeamsData* teamBe = player->getTeamBelongTO();

        int tmpId = teamBe->getTopScorerID();
        PlayersData* topPlayerInTheTeam = *allPlayersSortedById->find(tmpId)->data;
        if(topPlayerInTheTeam->getPlayerId() == playerId)
        {
            teamBe->setTopScorerGoalsNum(scoredGoals + oldKey->getGoals());
        }else{
            if(topPlayerInTheTeam->getGoals()< scoredGoals + oldKey->getGoals())
            {
                teamBe->setTopScorerGoalsNum(scoredGoals + oldKey->getGoals());
                teamBe->setTopScorerID(playerId);
            }else if(teamBe->getTopScorerID() == scoredGoals + oldKey->getGoals()){
                if(topPlayerInTheTeam->getCards() > cardsReceived + oldKey->getCards())
                {
                    teamBe->setTopScorerGoalsNum(scoredGoals + oldKey->getGoals());
                    teamBe->setTopScorerID(playerId);
                }else if (topPlayerInTheTeam->getCards() == cardsReceived + oldKey->getCards()){
                    if (topPlayerInTheTeam->getPlayerId() < playerId)
                    {
                        teamBe->setTopScorerGoalsNum(scoredGoals + oldKey->getGoals());
                        teamBe->setTopScorerID(playerId);
                    }
                }
            }
        }

        int oldGoalMinusCards= teamBe->getGoalsMinusCards();

        teamBe->setGoalsMinusCards(oldGoalMinusCards-cardsReceived + scoredGoals);//////////////////////HEREEEEEEEEE
        int tmpID = teamBe->getTeamId();
        if(teamsToChallenge->find(tmpID) != nullptr)
        {
            teamsToChallenge->remove(tmpID);
            int newPower = teamBe->getPoints() + teamBe->getGoalsMinusCards();
            teamsToChallenge->insert(tmpID,newPower);
        }




        PlayersData*  oldLower = player->getPreviousInOrder();
        PlayersData*  oldBigger = player->getNextInOrder();

        teamBe->getHeadTeamByEquality()->remove(*oldKey);
        allPlayersSortedBySpecialId->remove(*oldKey);
        player->setGamesPlayed( gamesPlayed + player->getGamesPlayed());
        player->setCards( player->getCards() + cardsReceived);
        player->setGoals( player->getGoals() + scoredGoals);
        PlayerSpecialKey* newKeyToUpdate = new PlayerSpecialKey( player->getGoals(),
                                                                 player->getCards(),
                                                                 playerId);


        allPlayersSortedBySpecialId->insert((*newKeyToUpdate),player);
        teamBe->getHeadTeamByEquality()->insert(*newKeyToUpdate,player);

        update_closest(player,allPlayersSortedBySpecialId);
        if(oldLower != nullptr ) update_closest(oldLower,allPlayersSortedBySpecialId);
        if(oldBigger != nullptr ) update_closest(oldBigger,allPlayersSortedBySpecialId);

        //delete newKeyToUpdate;
        delete oldKey;
        //newKeyToUpdate= nullptr;
        delete newKeyToUpdate;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <=0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;

    try{
        NodeInTree<int,TeamsData*>* team1 = allTeamsSortedById->find(teamId1);
        NodeInTree<int,TeamsData*>*  team2 = allTeamsSortedById->find(teamId2);

        if(team1 == nullptr || team2 == nullptr) return StatusType::FAILURE;
        static const int  LEGAL_PLAYER_NUM = 11;
        if((*(team1->data))->getPlayersNumber() < LEGAL_PLAYER_NUM
           ||  (*(team1->data))->getIsTeamGoalKeeperNum() <= 0) return StatusType::FAILURE;

        if((*(team2->data))->getPlayersNumber() < LEGAL_PLAYER_NUM
           ||  (*(team2->data))->getIsTeamGoalKeeperNum() <= 0) return StatusType::FAILURE;

        int powerOfTeam1 = (*(team1->data))->getGoalsMinusCards() + (*(team1->data))->getPoints();
        int powerOfTeam2 = (*(team2->data))->getGoalsMinusCards() + (*(team2->data))->getPoints();

        if(powerOfTeam1 > powerOfTeam2)
        {
            (*(team1->data))->teamWon();
        }else if (powerOfTeam1 == powerOfTeam2 ){
            (*(team1->data))->draw();
            (*(team2->data))->draw();
        }else{
            (*(team2->data))->teamWon();
        }

        (*(team1->data))->teamPlayed();
        (*(team2->data))->teamPlayed();
        if(teamsToChallenge->find(*(team1->key)) != nullptr)
        {
            teamsToChallenge->remove(*(team1->key));
            int newPower = (*team1->data)->getPoints() + (*team1->data)->getGoalsMinusCards();
            teamsToChallenge->insert(*(team1->key),newPower);
        }
        if(teamsToChallenge->find(*(team2->key)) != nullptr)
        {
            teamsToChallenge->remove(*(team2->key));
            int newPower = (*team2->data)->getPoints() + (*team2->data)->getGoalsMinusCards();
            teamsToChallenge->insert(*(team2->key),newPower);
        }

    }catch (bad_alloc& e){
    return StatusType::ALLOCATION_ERROR;
    }


	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0 )
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }
    try{
        NodeInTree<int,PlayersData*>* player = allPlayersSortedById->find(playerId);
        if(player == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        int gamesHavePlayed = (*(player->data))->getGamesPlayed() + (*(player->data))->getTeamBelongTO()->getGameTeamPlayed();
        output_t<int> out = output_t<int>(gamesHavePlayed);
        return out;
    }catch (bad_alloc& e){
        output_t<int> out = output_t<int>(StatusType::ALLOCATION_ERROR);
        return out;
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    try {
        NodeInTree<int,TeamsData*>* team = allTeamsSortedById->find(teamId);
        if (team == nullptr) return  StatusType::FAILURE;
        output_t<int> out = output_t<int>((*(team->data))->getPoints());
        return out;
    }catch (bad_alloc& e){
        output_t<int> out = output_t<int>(StatusType::ALLOCATION_ERROR);
        return out;
    }
}

void updatePlayerTeamInUnite(NodeInTree<int,PlayersData*>* player, TeamsData* newTeam, TeamsData* oldTeam )
{
    if(player == nullptr) return;
    updatePlayerTeamInUnite(player->leftSon,newTeam,oldTeam);
    (*player->data)->setTeamBelongTO(newTeam);
    int tmp = (*player->data)->getGamesPlayed() + oldTeam->getGameTeamPlayed();
    (*player->data)->setGamesPlayed(tmp);
    updatePlayerTeamInUnite(player->rightSon,newTeam,oldTeam);

}


StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if(teamId2 <= 0 || teamId1 <= 0 || teamId2 == teamId1 || newTeamId <=0) return StatusType::INVALID_INPUT;
    if(newTeamId != teamId1 && newTeamId != teamId2)
    {
        if(allTeamsSortedById->find(newTeamId)!= nullptr) return StatusType::FAILURE;
    }

    try {
        NodeInTree<int,TeamsData*>* team1Node = allTeamsSortedById->find(teamId1);
        NodeInTree<int,TeamsData*>* team2Node = allTeamsSortedById->find(teamId2);
        if (team1Node == nullptr || team2Node == nullptr) return  StatusType::FAILURE;

        TeamsData* team1 = *team1Node->data;
        TeamsData* team2 = *team2Node->data;

        if (team1 == nullptr || team2 == nullptr) return  StatusType::FAILURE;
        ///merge
        int sizeTeam1 = team1->getPlayersNumber();
        int sizeTeam2 = team2->getPlayersNumber();

        TeamsData* newData = new TeamsData(0, newTeamId);


        Tree<PlayerSpecialKey,PlayersData*>* delete1 = team1->getHeadTeamByEquality();
        Tree<int,PlayersData*>* delete2 = team1->getHeadTeamById();
        Tree<PlayerSpecialKey,PlayersData*>* delete3 =team2->getHeadTeamByEquality();
        Tree<int,PlayersData*>* delete4 = team2->getHeadTeamById();
        *newData += team1;
        *newData += team2;


        int temp = team1->getTeamId();
        if(teamsToChallenge->find(temp) != nullptr)
        {
            teamsToChallenge->remove(temp);
        }
        temp = team2->getTeamId();
        if(teamsToChallenge->find(temp) != nullptr)
        {
            teamsToChallenge->remove(temp);
        }

        if(newData->getPlayersNumber() >=11 && newData->getIsTeamGoalKeeperNum() >0)
        {
            int newPower = newData->getPoints() + newData->getGoalsMinusCards();
            teamsToChallenge->insert(newTeamId,newPower);
        }
        if(sizeTeam2 <= 0)
        {
            if(sizeTeam1 <= 0)
            {

                allTeamsSortedById->remove(teamId2);
                allTeamsSortedById->remove(teamId1);
                delete team1;
                delete team2;
                allTeamsSortedById->insert(newTeamId,newData);
                return StatusType::SUCCESS;
            }else{
                newData->setHeadTeamByEquality(team1->getHeadTeamByEquality());
                newData->setHeadTeamById(team1->getHeadTeamById());
                newData->setTopScorerID(team1->getTopScorerID());
                newData->setTopScorerGoalsNum(team1->getTopScorerGoalsNum());
                updatePlayerTeamInUnite(team1->getHeadTeamById()->getRoot(),newData,team1);

                team1->setHeadTeamById(nullptr);
                team1->setHeadTeamByEquality(nullptr);

                allTeamsSortedById->remove(teamId2);
                allTeamsSortedById->remove(teamId1);
                delete team1;
                delete team2;
                allTeamsSortedById->insert(newTeamId,newData);
                return StatusType::SUCCESS;
            }
        }else{
            if(sizeTeam1 <= 0)
            {
                //*newData += team2;
                newData->setHeadTeamByEquality(team2->getHeadTeamByEquality());
                newData->setHeadTeamById(team2->getHeadTeamById());
                updatePlayerTeamInUnite(team2->getHeadTeamById()->getRoot(),newData,team2);
                newData->setTopScorerID(team2->getTopScorerID());
                newData->setTopScorerGoalsNum(team2->getTopScorerGoalsNum());

                team2->setHeadTeamById(nullptr);
                team2->setHeadTeamByEquality(nullptr);
                delete team1;
                delete team2;
                allTeamsSortedById->remove(teamId2);
                allTeamsSortedById->remove(teamId1);
                allTeamsSortedById->insert(newTeamId,newData);
                return StatusType::SUCCESS;
            }
        }
        int topScorerTeam1 = team1->getTopScorerID();
        PlayersData* topTeam1 = *allPlayersSortedById->find(topScorerTeam1)->data;
        int topScorerTeam2 = team2->getTopScorerID();
        PlayersData* topTeam2 = *allPlayersSortedById->find(topScorerTeam2)->data;
        PlayerSpecialKey tmpKeyTop1 = PlayerSpecialKey(topTeam1->getGoals(),topTeam1->getCards(),topTeam1->getPlayerId());
        PlayerSpecialKey tmpKeyTop2 = PlayerSpecialKey(topTeam2->getGoals(),topTeam2->getCards(),topTeam2->getPlayerId());

        if(tmpKeyTop1>tmpKeyTop2)
        {
            newData->setTopScorerID(tmpKeyTop1.getPlayerID());
            newData->setTopScorerGoalsNum(tmpKeyTop1.getGoals());
        }else{
            newData->setTopScorerID(tmpKeyTop2.getPlayerID());
            newData->setTopScorerGoalsNum(tmpKeyTop2.getGoals());
        }



        updatePlayerTeamInUnite(team1->getHeadTeamById()->getRoot(),newData,team1);
        updatePlayerTeamInUnite(team2->getHeadTeamById()->getRoot(),newData,team2);

        ///merge for sorted by id

        //keys and data for id tree
        int* inOrderKeysTeam1 = new int[sizeTeam1];
        int* keyStart1 = inOrderKeysTeam1;



        int* inOrderKeysTeam2 = new int[sizeTeam2];
        int* keyStart2 = inOrderKeysTeam2;

        PlayersData** inOrderDataTeam1 = new PlayersData*[sizeTeam1];
        PlayersData** inOrderDataTeam2 = new PlayersData*[sizeTeam2];
        PlayersData** dataStart1 = inOrderDataTeam1;
        PlayersData** dataStart2 = inOrderDataTeam2;


        Tree<int,PlayersData*>* team1HeadBySortedByID = team1->getHeadTeamById();
        Tree<int,PlayersData*>* team2HeadBySortedByID = team2->getHeadTeamById();

        int a1=0,a2=0,a3=0,a4=0;

        team1HeadBySortedByID->TreeKey_to_array(team1HeadBySortedByID->getRoot(),inOrderKeysTeam1,&a1);
        team2HeadBySortedByID->TreeKey_to_array(team2HeadBySortedByID->getRoot(),inOrderKeysTeam2,&a2);
        team1HeadBySortedByID->TreeData_to_array(team1HeadBySortedByID->getRoot(),inOrderDataTeam1,&a3);
        team2HeadBySortedByID->TreeData_to_array(team2HeadBySortedByID->getRoot(),inOrderDataTeam2,&a4);


        int* keyDist = new int[sizeTeam1+sizeTeam2];
        PlayersData** dataDist = new PlayersData*[sizeTeam1+sizeTeam2];
        int* keyDistStart = keyDist;
        PlayersData** dataDistStart = dataDist;
        ////

        ////key and data for specail id tree
        PlayerSpecialKey* specialKeysTeam1 = new PlayerSpecialKey[sizeTeam1];
        PlayerSpecialKey* specialKeysTeam2 = new PlayerSpecialKey[sizeTeam2];
        PlayerSpecialKey* startKey1 = specialKeysTeam1;
        PlayerSpecialKey* startKey2 = specialKeysTeam2;

        PlayersData** specialDataTeam1 = new PlayersData*[sizeTeam1];
        PlayersData** specialDataTeam2 = new PlayersData*[sizeTeam2];
        PlayersData** data1Start = specialDataTeam1;
        PlayersData** data2Start =  specialDataTeam2;

        Tree<PlayerSpecialKey,PlayersData*>* team1TreeBySpecialID = team1->getHeadTeamByEquality();
        Tree<PlayerSpecialKey,PlayersData*>* team2TreeBySpecialID = team2->getHeadTeamByEquality();

        PlayerSpecialKey* keyDistSpecial = new PlayerSpecialKey[(sizeTeam1+sizeTeam2)];
        PlayersData** dataDistSpecial = new PlayersData*[sizeTeam1+sizeTeam2];

        PlayerSpecialKey* keyDistSpecialStart = keyDistSpecial;
        PlayersData** dataDistSpecialStart = dataDistSpecial;
        a1 =0,a2=0,a3=0,a4=0;

        team1TreeBySpecialID->TreeKey_to_array(team1TreeBySpecialID->getRoot(),startKey1,&a1);
        team2TreeBySpecialID->TreeKey_to_array(team2TreeBySpecialID->getRoot(),startKey2,&a2);
        team1TreeBySpecialID->TreeData_to_array(team1TreeBySpecialID->getRoot(),data1Start,&a3);
        team2TreeBySpecialID->TreeData_to_array(team2TreeBySpecialID->getRoot(),data2Start,&a4);
        //////
       /* delete1->treeClear();
        delete2->treeClear();
        delete3->treeClear();
        delete4->treeClear();
        delete delete1;
        delete delete2;
        delete delete3;
        delete delete4;
        delete *team1->data;
        delete *team2->data;*/


        int i=0, j=0;
        for (; i <sizeTeam1 && j<sizeTeam2 ;)
        {
            if(keyStart1[i]<keyStart2[j])
            {
                *keyDist = keyStart1[i];
                *dataDist = dataStart1[i];
                keyDist++;
                dataDist++;
                i++;
            }else{
                *keyDist = keyStart2[j];
                *dataDist = dataStart2[j];
                keyDist++;
                dataDist++;
                j++;
            }
        }
        while(i<sizeTeam1){
            *keyDist = keyStart1[i];
            *dataDist = dataStart1[i];
            i++;
            keyDist++;
            dataDist++;
        }
        while(j<sizeTeam2){
            *keyDist = keyStart2[j];
            *dataDist = dataStart2[j];
            j++;
            keyDist++;
            dataDist++;
        }

        int count =0;
        Tree<int, PlayersData*>* mergedTreeByID = new Tree<int, PlayersData*>();
        mergedTreeByID->arrayToTree(keyDistStart,dataDistStart,0,sizeTeam1+sizeTeam2 -1,&count);
        newData->setHeadTeamById(mergedTreeByID);

        ///merge for sorted by specialId
        for (i=0,j=0; i <sizeTeam1 && j<sizeTeam2 ; )
        {
            if(specialKeysTeam1[i]<specialKeysTeam2[j])
            {
                *keyDistSpecial = specialKeysTeam1[i];
                *dataDistSpecial = specialDataTeam1[i];
                keyDistSpecial++;
                dataDistSpecial++;
                i++;
            }else{
                *keyDistSpecial = specialKeysTeam2[j];
                *dataDistSpecial = specialDataTeam2[j];
                keyDistSpecial++;
                dataDistSpecial++;
                j++;
            }
        }
        while(i<sizeTeam1){
            *keyDistSpecial = specialKeysTeam1[i];
            *dataDistSpecial = specialDataTeam1[i];
            i++;
            keyDistSpecial++;
            dataDistSpecial++;
        }
        while(j<sizeTeam2){
            *keyDistSpecial = specialKeysTeam2[j];
            *dataDistSpecial = specialDataTeam2[j];
            j++;
            keyDistSpecial++;
            dataDistSpecial++;
        }

        count =0;
        Tree<PlayerSpecialKey, PlayersData*>* mergedTreeBySpecialID = new Tree<PlayerSpecialKey, PlayersData*>();
        mergedTreeBySpecialID->arrayToTree(keyDistSpecialStart,dataDistSpecialStart,0,sizeTeam1+sizeTeam2 -1,&count);
        newData->setHeadTeamByEquality(mergedTreeBySpecialID);

        delete1->treeClear();
        delete2->treeClear();
        delete3->treeClear();
        delete4->treeClear();
        delete delete1;
        delete delete2;
        delete delete3;
        delete delete4;

        allTeamsSortedById->remove(teamId2);
        allTeamsSortedById->remove(teamId1);
        allTeamsSortedById->insert(newTeamId,newData);
        delete team1;
        delete team2;
        /////////////////////////////////////////////////////////////////////////////
        delete[] keyStart1;
        delete[] keyStart2;
        delete[] dataStart1;
        delete[] dataStart2;
        delete[] keyDistStart;
        delete[] dataDistStart;

        delete[] specialKeysTeam1;
        delete[] specialDataTeam1;
        delete[] specialKeysTeam2;
        delete[] specialDataTeam2;
        delete[] dataDistSpecialStart;
        delete[] keyDistSpecialStart;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}


output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if(teamId == 0) return StatusType::INVALID_INPUT;
    if(teamId<0)
    {
        if(globalData->getNumOfAllPlayer() <= 0)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        output_t<int> out = output_t<int>(globalData->getTopScorerId());
        return out;
    } else{
        NodeInTree<int,TeamsData*>* teamNode = allTeamsSortedById->find(teamId);

        if( teamNode== nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        TeamsData* team = *teamNode->data;
        if(team->getPlayersNumber() <= 0)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }

        int id = team->getTopScorerID();
        if(id <=0 )
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        output_t<int> out = output_t<int>(id);
        return out;

    }
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId == 0)
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }
    if(teamId<0)
    {
        if(globalData->getNumOfAllPlayer() <= 0)
        {
            output_t<int> out = output_t<int>(0);
            return out;
        }
        output_t<int> out = output_t<int>(globalData->getNumOfAllPlayer());
        return out;
    } else{
        NodeInTree<int,TeamsData*>* team = allTeamsSortedById->find(teamId);
        if( team== nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }

        output_t<int> out = output_t<int>((*team->data)->getPlayersNumber());
        return out;
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(teamId ==0 || output == nullptr) return StatusType::INVALID_INPUT;
    if(teamId < 0)
    {

        if(globalData->getNumOfAllPlayer()<=0) return StatusType::FAILURE;
        PlayerSpecialKey* keyArr = new PlayerSpecialKey[globalData->getNumOfAllPlayer()];
        int cnt =0;
        allPlayersSortedBySpecialId->TreeKey_to_array(allPlayersSortedBySpecialId->getRoot(),keyArr,&cnt);
        for (int i = 0; i < globalData->getNumOfAllPlayer(); i++)
        {
            output[i] = keyArr[i].getPlayerID();
        }
        return StatusType::SUCCESS;
    }
    NodeInTree<int,TeamsData*>* team = allTeamsSortedById->find(teamId);
    if(team == nullptr) return StatusType::FAILURE;
    if((*team->data)->getPlayersNumber() <= 0) return StatusType::FAILURE;
    Tree<PlayerSpecialKey,PlayersData*>* teamToSort = (*team->data)->getHeadTeamByEquality();
    //auto keyArr = (PlayerSpecialKey**)malloc(sizeof(PlayerSpecialKey*)* (*team->data)->getPlayersNumber());//change malloc to new
    PlayerSpecialKey* keyArr = new PlayerSpecialKey[(*team->data)->getPlayersNumber()];

    int cntx =0;
    teamToSort->TreeKey_to_array(teamToSort->getRoot(),keyArr,&cntx);
    for (int i = 0; i < (*team->data)->getPlayersNumber(); i++)
    {
        output[i] = keyArr[i].getPlayerID();
    }
    delete[] keyArr;
    return StatusType::SUCCESS;

}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if(teamId <= 0 || playerId <=0)
    {
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }


    if(globalData->getNumOfAllPlayer() <=1 )
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }
    NodeInTree<int,TeamsData*>* team = allTeamsSortedById->find(teamId);
    if(team== nullptr)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }


    Tree<int,PlayersData*>* playerTree = ((*team->data)->getHeadTeamById());

    if(playerTree== nullptr)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }
    NodeInTree<int,PlayersData*>* playerNode = playerTree->find(playerId);
    if(playerNode== nullptr)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }
    PlayersData* player = (*playerNode->data);



    PlayersData* previousPlayer = player->getPreviousInOrder();
    PlayersData* nextPlayer = player->getNextInOrder();
    if((previousPlayer == nullptr) && (nextPlayer != nullptr)) return nextPlayer->getPlayerId();

    if((previousPlayer != nullptr)&& (nextPlayer == nullptr)) return previousPlayer->getPlayerId();


    if((previousPlayer == nullptr) && (nextPlayer == nullptr)) return playerId;
    int prePlayerGoalDist = previousPlayer->getGoals() - player->getGoals();
    if(prePlayerGoalDist <0) prePlayerGoalDist = 0- prePlayerGoalDist;
    int nextPlayerGoalDist = nextPlayer->getGoals() - player->getGoals();
    if(nextPlayerGoalDist < 0) nextPlayerGoalDist = 0-nextPlayerGoalDist;


    if(nextPlayerGoalDist < prePlayerGoalDist){
        output_t<int> out = output_t<int>(nextPlayer->getPlayerId());
        return out;
    }else if (nextPlayerGoalDist > prePlayerGoalDist){
        output_t<int> out = output_t<int>(previousPlayer->getPlayerId());
        return out;

    }else{
        int prePlayerCardDist = previousPlayer->getCards() - player->getCards();
        int nextPlayerCardDist = nextPlayer->getCards() - player->getCards();
        if(prePlayerCardDist <0) prePlayerCardDist *= -1;
        if(nextPlayerCardDist <0) nextPlayerCardDist *= -1;

        if(prePlayerCardDist < nextPlayerCardDist){
            output_t<int> out = output_t<int>(previousPlayer->getPlayerId());
            return out;

        }else if (prePlayerCardDist > nextPlayerCardDist){
            output_t<int> out = output_t<int>(nextPlayer->getPlayerId());
            return out;
        }else{
            int prePlayerIdDist = previousPlayer->getPlayerId() - playerId;
            int nextPlayerIdDist = nextPlayer->getPlayerId() - playerId;
            if(prePlayerIdDist < 0) prePlayerIdDist*=-1;
            if(nextPlayerIdDist < 0) nextPlayerIdDist*=-1;

            if(prePlayerIdDist < nextPlayerIdDist){
                output_t<int> out = output_t<int>(previousPlayer->getPlayerId());
                return out;
            }else if(prePlayerIdDist > nextPlayerIdDist){
                output_t<int> out = output_t<int>(nextPlayer->getPlayerId());
                return out;
            }else{
                if( nextPlayer->getPlayerId() > previousPlayer->getPlayerId()){
                    output_t<int> out = output_t<int>(nextPlayer->getPlayerId());
                    return out;
                }else{
                    output_t<int> out = output_t<int>(previousPlayer->getPlayerId());
                    return out;
                }
            }
        }

    }
}


//void ultraInOrder(NodeInTree<int,int>* start,int min, int max, NodeInTree<int,int>* arr[],bool& maxFound,int* count)
void ultraInOrder(NodeInTree<int,int>* start, int min, int max, int* key, int* power, bool& maxFound, int* count)
{

    if (start == nullptr || maxFound)
    {
        return;
    }

    ultraInOrder(start->leftSon,min,max,key,power,maxFound,count);
    if(*start->key >= min &&  *start->key <= max)
    {
        //arr[*count]= start;
       //arr++;
       key[*count] = *start->key;
       power[*count] = *start->data;
        (*count)++;
    }
    if( *start->key == max)
    {
        maxFound = true;
        return;
    }
    ultraInOrder(start->rightSon,min,max,key,power,maxFound,count);

}

int getWinner(int power[], int key[], int *counter, int i){
    if(*counter == 1){
        return key[0];
    }
    if(i % 2 == 0) {
        for (int j = 0; j < *counter; j += 2) {
            int power1 = power[j];
            int power2 = power[j + 1];
            if (power1 > power2) {
                power[j] += power[j + 1];
                power[j] += 3;
                power[j - j / 2] = power[j];
                key[j - j / 2] = key[j];
            } else if(power1 < power2){
                power[j + 1] += power[j];
                power[j + 1] += 3;
                power[j - (j / 2)] = power[j + 1];
                key[j - (j / 2)] = key[j + 1];
            }
            else{
                if(key[j] > key[j + 1]){
                    power[j] += power[j + 1];
                    power[j] += 3;
                    power[j - j / 2] = power[j];
                    key[j - (j / 2)] = key[j];
                }
                else{
                    power[j + 1] += power[j];
                    power[j + 1] += 3;
                    power[j - (j / 2)] = power[j + 1];
                    key[j - (j / 2)] = key[j + 1];
                }
            }
        }
        *counter /= 2;
        i /= 2;
        return getWinner(power, key, counter, i);
    }
    else {
        for (int k = 0; k < *counter - 1; k += 2) {
            int power1 = power[k];
            int power2 = power[k+1];
            if (power1 > power2) {
                power[k] += power[k+1];
                power[k] += 3;
                power[k - k / 2] = power[k];
                key[k - k / 2] = key[k];
            } else if(power1 < power2){
                power[k+1] += power[k];
                power[k+1] += 3;
                power[k - k / 2] = power[k + 1];
                key[k - k / 2] = key[k + 1];
            }
            else{
                if(key[k] > key[k + 1]){
                    power[k] += power[k+1];
                    power[k] += 3;
                    power[k - k / 2] = power[k];
                    key[k - k / 2] = key[k];
                }
                else{
                    power[k+1] += power[k];
                    power[k+1] += 3;
                    power[k - k / 2] = power[k + 1];
                    key[k - k / 2] = key[k + 1];
                }
            }
        }
        power[(*counter - 1) - (*counter - 1)/2] = power[*counter-1];
        key[(*counter - 1) - (*counter - 1) / 2] = key[*counter - 1];
        *counter /= 2;
        (*counter)++;
        i /= 2;
        i++;
        return getWinner(power, key, counter, i);
    }
}


output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    if(maxTeamId < 0 || minTeamId < 0 || maxTeamId < minTeamId){
        output_t<int> out = output_t<int>(StatusType::INVALID_INPUT);
        return out;
    }
    if(teamsToChallenge->getSize() <= 0){
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }


    NodeInTree<int,int>* root = teamsToChallenge->getRoot();
    NodeInTree<int,int>* min = root;
    NodeInTree<int,int>* max = root;

    while (min)
    {
        if(*min->key == minTeamId )
        {
            break;
        }
        else if(*min->key < minTeamId)
        {
            if(min->rightSon == nullptr)
            {
                break;
            }
            min= min->rightSon;
        }else{
            if(min->leftSon == nullptr)
            {
                break;
            }
            min= min->leftSon;
        }
    }

    if(*min->key > maxTeamId)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }

    while (max)
    {
        if(*max->key == maxTeamId)
        {
            break;
        }
        else if(*max->key < maxTeamId)
        {
            if(max->rightSon == nullptr)
            {
                break;
            }
            max= max->rightSon;
        }else{
            if(max->leftSon == nullptr)
            {
                break;
            }
            max= max->leftSon;
        }
    }

    if( *max->key < minTeamId)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }

    if(*max->key < *min->key)
    {
        output_t<int> out = output_t<int>(StatusType::FAILURE);
        return out;
    }


    if(*max->key == *min->key)
    {
        if(*max->key < minTeamId || *min->key > maxTeamId)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        NodeInTree<int,int>* winner = teamsToChallenge->find(*min->key);
        if(winner == nullptr || winner->data == nullptr)
        {
            output_t<int> out = output_t<int>(StatusType::FAILURE);
            return out;
        }
        output_t<int> out = output_t<int>(*max->key);
        return out;
    }

    try{
        bool done = false;
        int size = teamsToChallenge->getSize();
        int* key = new int[size];
        int* power = new int[size];
        int count =0;
        if(*min->key >= minTeamId && *min->key <= maxTeamId)
        {
            key[count] = *min->key;
            power[count] = *min->data;
            count++;
        }

        if(min->hasRightSon())
        {
            NodeInTree<int,int>* rightSon = min->rightSon;
            ultraInOrder(rightSon,minTeamId,maxTeamId,key,power,done,&count);
        }

        while(min != nullptr && !done)
            {
                while( min->father != nullptr && *min->father->key < *min->key)
                {
                    min = min->father;
                }
                min = min->father;
                if(min != nullptr)
                {
                    if(*min->key >= minTeamId && *min->key <= maxTeamId)
                    {
                        key[count] = *min->key;
                        power[count] = *min->data;
                        count++;
                        if(*min->key >= maxTeamId)
                        {
                            done = true;
                            break;
                        }
                    }
                    if(min->hasRightSon())
                    {
                        ultraInOrder(min->rightSon,minTeamId,maxTeamId,key,power,done,&count);
                    }
                }
            }

       int ans = getWinner(power,key,&count,count);
        output_t<int> out = output_t<int>(ans);
        delete[] key;
        delete[] power;
        return out;
    }catch (bad_alloc& e){
        return StatusType::ALLOCATION_ERROR;
    }

}




