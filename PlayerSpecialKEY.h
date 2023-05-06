//
// Created by hashe on 11/30/2022.
//

#ifndef MAIN23A1_CPP_PLAYERSPECIALKEY_H
#define MAIN23A1_CPP_PLAYERSPECIALKEY_H


class PlayerSpecialKey{
private:
    int goalsScored;//#1
    int cardsReceived;//more cards means he is less ,#2
    int playerID;//#3
public:
    PlayerSpecialKey() : goalsScored(0), cardsReceived(0),playerID(0) {};
    PlayerSpecialKey(int goalsScored, int cardsReceived, int playerID):
            goalsScored(goalsScored),
            cardsReceived(cardsReceived),
            playerID(playerID)
            {};

    int getGoals() const{return this->goalsScored;}
    int getCards() const{return this->cardsReceived;}
    int getPlayerID() const{return this->playerID;}

    void setGoals(int goals){this->goalsScored = goals;}
    void setCards(int cards){this->cardsReceived = cards;}
    void setPlayerID(int ID){this->playerID = ID;}


    bool operator>(PlayerSpecialKey& key) const
    {
        if(this->getGoals() > key.getGoals())
            return true;
        if(this->getGoals() == key.getGoals())
        {
            if(this->getCards() < key.getCards())
                return true;
            if(this->getCards() == key.getCards())
            {
                if(this->getPlayerID() > key.getPlayerID())
                    return true;
            }
            return false;
        }

        return false;
    }

    bool operator==(PlayerSpecialKey& key) const
    {
        return (this->getGoals() == key.getGoals()
                && this->getCards() == key.getCards()
                && this->getPlayerID() ==key.getPlayerID());
    }

    bool operator<(PlayerSpecialKey& key)const
    {
        return !((*this)>key) && !(*this==key);
    }

    bool operator<=(PlayerSpecialKey& key)const
    {
        return (*this < key || *this == key);
    }

    bool operator>=(PlayerSpecialKey& key)const
    {
        return (*this > key || *this == key);
    }


    bool operator!=(PlayerSpecialKey& key)const
    {
        return !(*this==key);
    }

    void updateKey(PlayerSpecialKey& key)
    {
        this->goalsScored = key.getGoals();
        this->cardsReceived = key.getCards();
        this->playerID = key.getPlayerID();
    }



};

#endif //MAIN23A1_CPP_PLAYERSPECIALKEY_H
