# WorldCupTournament
World Cup tournament

at this tournament support the following functions:

K represents teams number
N represents players number

1)world_cup_t() - start the tournament with zero players and zero teams O(1)

2)virtual ~world_cup_t() - frees the memory O(N+K)

3)add_team(int teamId, int points) - add new team to the tournament with special team ID O(log(K))

4)remove_team(int teamId) - removes team from team O(log(K))

5)add_player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper)- O(logK + logN)

6)remove_player(int playerId)- remove Player from the tournament O(logN)

7)update_player_stats(int playerId, int gamesPlayed, int scoredGoals, int cardsReceived) - after the player has played he gained experince so his stats should be updated O(logN)

8) play_match(int teamId1, int teamId2)- two teams plays togther the winner gets 3 points, if draw both team get 1 point. O(logK)

9)get_num_played_games(int playerId)- return the number of played games of the player O(logN)

10)get_team_points(int teamId)-return team points O(logK)

11)unite_teams(int teamId1, int teamId2, int newTeamId)- O(logK +N(TEAM1)+N(TEAM2))

12)get_top_scorer(int teamId)- if teamId<0 return top scorer in the tournament with O(1) else return the top scorer in the team with the given teamId with O(logK)

13)get_all_players(int teamId, int ∗ const output)- here we waant to sort players by 3 speical keys: 1)by goals, 2)by less cards the player got, 3) by playerId. we start sorting by the first key in equlity case we move to the second. if teamId< 0 we sort all players with O(N).e
else we sort just the players in the given team with O(logK + N(team))

14)get_closest_player(int playerId, int teamId)- with given teamID and player ID return the closeset player  in the  tournament to the player with O(logK +Log(N(TEAM))

15)knockout_winner(int minTeamId, int maxTeamId)- we try to predict which team will win if a comppetion has got in between teams that thier ID is between the two arguments in the function with complexity of O(log(min{N,K}+r) that r represent the number of the team in the competetion.
