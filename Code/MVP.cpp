/**
 * MVP
 *
 * @author Kevin Mathisen, Torje Bliksvær
 * @file MVP.cpp
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "LesData2.h"
using namespace std;


/*
 * Enum tournamentType
 */
enum bracketType    {SingleElimination, RoundRobin, undecided};

/*
* Team class
* Includes:
* Team name, (team stats, Players, description)
*
*
*/
class Team {
private:
    string  name;
    vector <string> members;
public:
    Team()  { name = ' '; }
    Team(ifstream & inn);
    void readData();
    void writeResults();
    void writeInfo();
    void writeToFile(ofstream & ut);
    bool isTeam(const string n)    { return (name == n); }
    string getName()    { return name; }
};

/*
* (Player class)
* Includes:
* Name, role, (player stats)
*
*
*/

/*
* Results class?
*/
class Match {
private:
    int pointsTeam1,                    //  Points for each team
        pointsTeam2,
        teamWon,                        //  Key of team that won
        team1,                          //  Map key value for teams in match
        team2;

public:
    Match() { pointsTeam1 = pointsTeam2 = teamWon = 0; }
    Match(int cTeam1, int cTeam2) {
        team1 = cTeam1; team2 = cTeam2; pointsTeam1 = pointsTeam2 = teamWon = 0;}
    Match(ifstream & inn);

    void playMatch();
    int getWinner()    { return teamWon; }
    void writeScore();
    void writeToFile(ofstream & ut);
};


/*
* Tournament class
* Includes:
* bracket type, size of tournament, teams and (players), description.
*
*
*/
class Tournament {
private:
    string  name,
            description;
    int     numTeams,           //  Number of total teams in tournament
            currentStage;       //  Which stage the tournament is on.
    bracketType type;
    bool    finished;           //  If the tournament is finished or ongoing.

    vector <int>    teamsNotPlayed;    //  Contains vectors for all teams key values on each stage:
                                //  index 0 has all teams, index 1 has winners from
                                //      stage 1, etc.

    vector <vector <Match*> >   matches;  //  Vectors with the matches from
                                          //    each stage. index 0 are the
                                          //    matches stage 1, etc

public:
    Tournament()    { name = description = ' ';     teamsNotPlayed = {};
        numTeams = currentStage = 0; finished = false; }
    Tournament(ifstream & inn, string nme);
    void initiateTournament();
    void readData();        //  Reads the name, desc, type, and num teams.
    void nextStage();       //  Goes to the next stage.
    void playNextMatch();   //  Plays the next match
    bool addTeam();         //  Adds a team to the tournament
    void writeResults();    //  Writes the results of matches
    void writeSchedule();   //  Writes the schedule
    void writeInfo();       //  Writes information about the tournament
    void writeToFile(ofstream & ut);
    string getName();
    bool teamInTournament(const int i) const;
};

void getFromFile();
void gatherTeams(ifstream & inn);
void gatherTournaments(ifstream & inn);
void saveState();
void teamWriteToFile(ofstream & ut);
void tournamentWriteToFile(ofstream & ut);
int findTeam(const string navn);
Tournament* findTournament(const string & navn);
void writeMenu();
void writeMenuTournament(const string & name);
void readCommandTournament();
void newTeam();
void newTournament();
void seeResults();
void writeAllTournament();
void writeAllTeams();

map <int, Team*> gTeams;            //  Map with key and pointer to all teams
vector <Tournament*> gTournaments;  //  Vector with pointer to all tournaments


/*
*   Main program
*/
int main() {
    getFromFile();
    char kommando;

    writeMenu();
    kommando = lesChar("\nCommand");

    while (kommando != 'Q') {
        switch(kommando) {
            case 'N':   newTournament();            break;
            case 'O':   readCommandTournament();    break;
            case 'R':   seeResults();               break;
            case 'A':   writeAllTournament();       break;
            case 'T':   newTeam();                  break;
            case 'W':   writeAllTeams();            break;
        }
        writeMenu();
        kommando = lesChar("\nCommand");
    }
    saveState();
    return 0;
}

//Functions, read, write, etc.

Tournament :: Tournament(ifstream & inn, string nme) {
    char brcktTyp;          //  Bracket type
    int fin,
        key,                //  Map key for teams
        matchesAmnt,        //  Amount of matches on file
        team1,              //  First team of a match's map index
        numbrTeamsNotPlayed;
    vector <Match*> cMatches;  //  Temporary matches
    string buffer;


    name = nme;
    getline(inn, description);
    inn >> numTeams >> currentStage; inn.ignore();
    inn >> brcktTyp >> fin >> numbrTeamsNotPlayed;

    switch (brcktTyp) {
        case 'S':   type = SingleElimination;   break;
        case 'R':   type = RoundRobin;          break;
        default:    type = undecided;
    }

    finished = fin;

    for (int i = 0; i < numbrTeamsNotPlayed; i++) {
        inn >> key;
        teamsNotPlayed.push_back(key);       //  Puts the team pointer associated with the team key
                                                                    //  in the correct stage
    }

    for (int i = 0; i < currentStage; i++) {        //  For each stage
        inn >> matchesAmnt;                         //  Read amount of matches

        for (int i = 0; i < matchesAmnt; i++) {     //  For each match
            cMatches.push_back(new Match(inn));     //  Reads match
        }

    matches.push_back(cMatches);                    //  Saves all matches in stage
    cMatches.clear();                               //  Clears for next stage
    }

    getline(inn, buffer);
}

/*
*   Initiates a tournament
*/
void Tournament :: initiateTournament() {       
    bool addMoreTeams;
    cout << "\nInfo about tournament:";
    readData();                                     //  Reads data for tournament

    cout << "\n\tAdding existing teams to tournament (enter to end):\n";
    do {                                            
        addMoreTeams = addTeam();                   //  Adds team to tournament
    } while (addMoreTeams);                         //  While addTeam says we should add more teams


    cout << "\n\tFinished creating tournament " << name << "\n";
}

/*
* Reads the name, desc, type, and num teams.
*/
void Tournament::readData() {
    char bracketType;       //  The type of bracket for the tournament

    cout << "\n\tTournament names already in use: ";
    for (const auto & tournament : gTournaments)        //  For each tournament
        cout << tournament->getName() << ", ";          //  Write tournamentname

                                                        //  Reads name of tournament:
    cout << "\n\tTournament name: ";      getline(cin, name);
    while (findTournament(name) || name.empty()) {      //  While invalid name
        cout << "\tInvalid tournament name, choose new tournament name: ";
        getline(cin, name);                             //  Reads new name
    }
                                                        //  Reads description:
    cout << "\tDescription: ";          getline(cin, description);

    type = SingleElimination;                           //  Hardcoded tournament type


    do {                            //  Reads amount of teams in tournament:
        numTeams = lesInt("\tAmount of teams (2,4,8,16)", 2, 16);
    } while (numTeams!=2 && numTeams!=4 && numTeams!=8 && numTeams!=16);
}

/*
* Goes to the next stage
*/
void Tournament::nextStage() {
    if (!finished) {            //  If tournament not finished:
                                //  If all matches from this stage played or first stage:
        if ((teamsNotPlayed.empty() && currentStage) || (!currentStage && teamsNotPlayed.size() == numTeams)) {

            if (currentStage >= 1)     //  If we are at 1 stage or later:
                                //  For all the matches in previous stage:
                for (const auto & match : matches[currentStage-1])
                                //  Add the winner of each match to next stage;
                    teamsNotPlayed.push_back((match->getWinner()));

                                //  If there is only one team left:
            if (teamsNotPlayed.size() == 1) {
                finished = true;    //  Finish the tournament and announce winner:
                cout << "\n\n\tThe tournament is finished after " << currentStage
                     << " stages, the winner is: " << (gTeams.find(teamsNotPlayed[0])->second)->getName() << "\n\n";
                                
            } else if (currentStage == 0) {     //  If there is more than one team remaning and the stage is 0 (tournament hasn't started): 
                cout << "\nThe tournament is ready to be played, "
                     << "play the next matches to continue the tournament\n\n";
                currentStage++;                 
                matches.push_back({});

            } else {                //  More teams left and not first stage (tournament has been started):
                cout << "\n\tAll teams have played each other, proceeding to the next stage.\n"
                     << "\tPlay the next matches to continue the tournament\n\n";
                currentStage++;
                matches.push_back({});
            }
        } else if (!currentStage && teamsNotPlayed.size() != numTeams) {    //  Have not added enough teams to start:
            cout << "\n\tNeed to add " << numTeams << " but have only added " << teamsNotPlayed.size() << " teams\n";

        } else              //  Tournament has already been started:
            cout << "\n\tTournament already started!\n";

    } else                  //  If tournament is finished:
        cout << "\n\tTournament is finished!\n\n";
}

/*
 * Plays the next match
 */
void Tournament :: playNextMatch() {
    int team1,
        team2;
    Match*  newMatch;

    if (!finished) {            //  If not finished:
        if (currentStage) {     //  Tournament has been started:
                                //  More matches to be played:
            if (!teamsNotPlayed.empty()) {

                                //  Takes two teams from the teams who will play
                                //    and removes them from the list.
                team1 = teamsNotPlayed[teamsNotPlayed.size()-1];
                teamsNotPlayed.pop_back();

                team2 = teamsNotPlayed[teamsNotPlayed.size()-1];
                teamsNotPlayed.pop_back();


                                //  Adds the two teams to a new match:
                newMatch = new Match(team1, team2);
                                //  Play the match between the teams:
                newMatch->playMatch();

                                //  Saves the match to the tournament:
                matches[currentStage-1].push_back(newMatch);

                                //  If there are no teams remaining
                if (teamsNotPlayed.empty()) {
                    nextStage();    //  Prepares the next stage. 
                }

            } else              //  All matches in current stage played:
                cout << "\n\tYou have played all matches, proceed to the next stage.\n";

        } else                  //  Tournament hasn't been started:
            cout << "\n\tCan't play match, you need to start the tournament.\n";

    } else                      //  Tournament is finished:
        cout << "\n\tTournament is finished!\n";
}

/**
* existingTeamtoTourney
*
* @return If want to add more teams
*/
bool Tournament::addTeam() {
    string navnTeam;
    bool availableTeam = false;
    int newTeam,
        teamsRemaining = numTeams - teamsNotPlayed.size();      //  Remaining team to be added.

                                    //  If there are space for more teams:
    if (teamsNotPlayed.size() < numTeams && !currentStage) {

        cout << "\n\tTeams available to add to tournament: ";
        for (const auto & team : gTeams)                    //  For each team.
            if (!teamInTournament(team.first)) {            //  if the team is not in the tournament.
                cout << (team.second)->getName() << ", ";   //  writes the name of the team
                availableTeam = true;                       //  Saves that there is at least one team available
            }
        if (!availableTeam)             //  No teams available:
            cout << " No teams available - Create more teams and add them to the tournament!";

        else {                          //  At least one team available:
                                        //  Reads name of team to add:
            cout << "\n\t\tName of team num " << teamsNotPlayed.size()+1 << " (of " << numTeams << ") to add: ";
            getline(cin, navnTeam);


            if ( !navnTeam.empty() ) {  //  If user wrote a name:

                                        //  Tries to find team with the same name:
                newTeam = findTeam(navnTeam);

                                        //  If we found the team:
                if (newTeam != -1 && !teamInTournament(newTeam)) {
                                        //  Saves the team to the tournament:
                        teamsNotPlayed.push_back(newTeam);
                        cout << "\t\tAdded " << navnTeam << " to the tournament\n";

                } else if (newTeam == -1)   //  Did not find team:
                    cout << "\t\tCould not find team '" << navnTeam << "'\n";

                else                        //  Team already in tournament:
                    cout << "\t\tTeam already added to tournament!\n";

                if (teamsNotPlayed.size() < numTeams)
                    return true;        //  Still more teams to be added.

            } else {                    //  User didn't write a name:
                cout << "\n\n\tOK - not adding more teams (still need to add "
                    << teamsRemaining << " more teams to start tournament)\n";
            }
        }
    } else {                        //  No more space for teams:
        cout << "\n\t\tAlready added all teams!\n\n";
    }

    return false;                   //  Done with adding teams.
}

/*
* writeTournament, Writes the results of matches
*/
 void Tournament::writeResults() {
    cout << "\n" << name << " - " << description << ":\n";

    if (matches.empty() || matches[0].empty()) {            //  If no matches has been played:
        cout << "No matches played!\n";

    } else {                                                //  Exist results:
        for (int i = 0; i < currentStage; i++) {            //  For all stages:
            cout << "\n\tResults stage " << i+1 << ":\n";   //  write which stage.
            for (int j = 0; j < matches[i].size(); j++)     //  for all matches in the stage:
                matches[i][j]->writeScore();                //  Write result for match.
        }

        if (finished)       //  If tournament finished, write winner:
            cout << "\n\tThe winner of the tournament is: "
                 << gTeams.find(teamsNotPlayed[0])->second->getName() << "!\n";

        cout << "\n";
    }

}


/*
* Writes information about the tournament'
*/
void Tournament::writeInfo() {
    cout << "\n" << name << " - " << description
         << ":\n\tNum teams: " << numTeams
         << "\n\tBracket type: "
         << ((type == SingleElimination) ? "Single Elimination" : "Round robin")
         << "\n\tCurrent Stage: " << currentStage
        << "\n\tFinished: " << ((finished) ? "Yes" : "No" ) << "\n";
}


/*
*   Writes tournament and its matches to given file
*/
void Tournament :: writeToFile(ofstream & ut) {
    ut << name << "\n" << description << "\n" << numTeams << " " << currentStage << " ";

    switch (type) {
        case SingleElimination:     ut << "S ";     break;
        case RoundRobin:            ut << "R ";     break;
    }

    ut << finished << "\n" << teamsNotPlayed.size() << " ";
    for (const auto & val : teamsNotPlayed)
        ut << val << " ";

    for (int i = 0; i < currentStage; i++) {
        ut << "\n";
        ut << matches[i].size() << " ";

        for (const auto & val : matches[i]) {
            val->writeToFile(ut);
        }
    }
}

bool Tournament::teamInTournament(const int i) const {
    for (const auto & teamI : teamsNotPlayed)
        if (teamI == i)
            return true;

    return false;
}

/*
*   Constructor from file
*/
Team :: Team(ifstream & inn) {
    string memberName;
    inn.ignore();

    getline(inn, name);

    for (int i = 0; i < 5; i++) {
        getline(inn, memberName);
        members.push_back(memberName);
    }
}

/*
*   Writes internal info to given file
*/
void Team :: writeToFile(ofstream & ut) {
    ut << name;
    for (int i = 0; i < members.size(); i++)
        ut << "\n" << members[i];
}

/*
* Reads the name and members of the team.
*/
void Team::readData() {
    string medlem, bName;

    cout << "\nNames already in use: ";
    for (const auto & team : gTeams)                //  for each team:
        cout << (team.second)->getName() << ", ";   //  write name.

                                //  Read name of new team:
    cout << "\nName of new team: ";     getline(cin, bName);
                                //  While invalid name:
    while (findTeam(bName) != -1 || bName.empty()) {
        cout << "Team name already exists, choose new team name: ";
        getline(cin, bName);    //  Read new name.
    }
    name = bName;               //  Save name. 

    for (int i = 0; i < 5; i++) {   //  for each member, read and save name:
        cout << "\tMember " << i+1 << ": ";     getline(cin, medlem);
        members.push_back(medlem);
    }

}

/**
 * Returns the name of the tournament
 * 
 * @return name of tournament
 */
string Tournament::getName() {
    return name;
}

/*
* writes the name and memebers of the team.
*/
void Team::writeInfo() {
    cout << '\t' << setw(5) << name << ": ";
    for (const auto member : members)
        cout << setw(10) << member << ", ";
    cout << '\n';
}

/*
*   Constructor from file
*/
Match :: Match(ifstream & inn) {
    int team1MapIndex,
        team2MapIndex,
        team1Points,
        team2Points,
        winner;

    inn >> team1MapIndex >> team1Points >> team2MapIndex >> team2Points >> winner;

    team1 = team1MapIndex;
    team2 = team2MapIndex;

    pointsTeam1 = team1Points;
    pointsTeam2 = team2Points;

    teamWon = winner;
}

/*
* Writes the score from the match
*/
void Match::writeScore() {
    cout << setfill(' ') << setw(5) << (gTeams.find(team1)->second)->getName() << " - "
         << setw(5) << (gTeams.find(team2)->second)->getName() << ": "
         << pointsTeam1 << " - " <<  pointsTeam2 << '\n';
}

/*
* Plays the match by reading the points and deciding the winner
*/
void Match::playMatch() {
    int teamWonGame,
        i = 1;
                                    //  Writes teams playing each other:
    cout << "\nPlaying match between " << (gTeams.find(team1)->second)->getName()
         << " (1) and " << (gTeams.find(team2)->second)->getName() << " (2):\n";

                                    //  As long as no one has won:
    while (pointsTeam1 < 3 && pointsTeam2 < 3) {
        do {                        //  Gets who won each game:
            cout << "\nCurrent score: " << pointsTeam1 << " - " << pointsTeam2;
            cout << "\nWho won game " << i << " (1 or 2): ";
            cin >> teamWonGame;
        } while (teamWonGame != 1 && teamWonGame != 2);

                                    //  Adds their win to their score:
        if (teamWonGame == 1)   pointsTeam1++;
        else                    pointsTeam2++;

        i++;                        //  Increases which game is begin played.
    }
                                    //  Saves which team won (got 3 points):
    teamWon = ( (pointsTeam1 == 3) ? team1 : team2 );

                                    //  Writes the final score:
    cout << "\nMatch finished, final score:\t";
    writeScore();
}

/*
*   Writes match to given file
*/
void Match :: writeToFile(ofstream & ut) {
    ut << team1 << " " << pointsTeam1 << " " << team2 << " " << pointsTeam2 << " " << teamWon << " ";
}

/*
* writeAllTorunaments
*/
void writeAllTournament() {
    if (!gTournaments.empty()) {
        cout << "\n\nInfo about all tournaments:\n";
        for (const auto & val : gTournaments)
            val->writeInfo();
    } else
        cout << "\nNo tournaments exists!\n";
}

/*
* writeTeams
*/
void writeAllTeams() {
    if (!gTeams.empty()) {                  //  Teams exist:
        cout << "\n\nAll teams:\n";         
        for (const auto & team : gTeams)    //  for all teams:
            (team.second)->writeInfo();     //  write info.
        cout << '\n';
    } else                                  //  No teams exist:
        cout << "\n\nNo teams exist!\n\n";
}

/*
* findexistingTeam
*/
int findTeam(const string navn) {           
    int foundTeam = -1;

    for (const auto & team : gTeams)        //  for each team:
        if ((team.second)->isTeam(navn))    //  If name equals name
            foundTeam = team.first;         //  Save key value for team

    return foundTeam;   //  return key value for team (-1 if not found)
}

/*
* seeResults
*/
void seeResults() {
    if (!gTournaments.empty()) {                //  Tournaments exist:
        for (const auto & val : gTournaments)   //  for each tournament:
            val->writeResults();                //  write results
    } else
        cout << "\n\nNo tournaments exist!\n";
}

/*
* deleteAll
*/

/*
* createTournament
*/
void newTournament() {
    Tournament* nyTournament = new Tournament;  //  Create new tournament object.
    nyTournament->initiateTournament();         //  Read data for tournament.
    gTournaments.push_back(nyTournament);       //  Save tournament.
}

void newTeam() {
    int key;
    Team* newTeam = new Team;                   //  create new team.
    cout << "\n\nIDs already in use: ";
    for (const auto & team : gTeams)            //  For each team:
        cout << team.first << ", ";             //  write their key value.
                                                //  Read key value for new team:
    key = lesInt("\nId (Unique identifier) for a new team", 1, 999);
    if (!gTeams.count(key)) {       //  If the key is unique:
        newTeam->readData();        //  Read data for new team
                                    //  Saves team:
        gTeams.insert(pair <int, Team*> (key, newTeam));

    } else                          //  Team with key value already exists:
        cout << "\tTeam with same ID already exists!\n\n";

}


/**
 * Read teams and tournaments from file.
 * 
 */
void getFromFile() {
    ifstream innFil;

    innFil.open("teams.dta");
    gatherTeams(innFil);
    innFil.close();

    innFil.open("tournaments.dta");
    gatherTournaments(innFil);
    innFil.close();
}


void gatherTeams(ifstream & inn) {
    int mapIndex;
    Team* newTeam;

    if (inn) {
        cout << "\nReading team(s) from file...";
        inn >> mapIndex;

        while (!inn.eof()) {
            newTeam = new Team(inn);
            gTeams[mapIndex] = newTeam;
            inn >> mapIndex;
        }

        cout << "\nTeams finished reading\n";
    } else {
        cout << "\n\nCould not find 'teams.dta'!!\n\n";
    }
}

void gatherTournaments(ifstream & inn) {
    string tName;
    Tournament* newTournament;

    if (inn) {
        cout << "\nReading tournament(s) from file...";
        getline(inn, tName);
        while (!inn.eof()) {
            newTournament = new Tournament(inn, tName);
            gTournaments.push_back(newTournament);
            getline(inn, tName);
        }
        cout << "\nTournaments finished reading\n\n";
    } else {
        cout << "\n\nCould not find file 'tournaments.dta'!!\n\n";
    }
}

/**
 * Saves teams and tournaments to .dta files
 * 
 */
void saveState() {
    int index = 0;      //  Counts whether its the first team/tournament has been stored, used for formatting
    ofstream utFil;

    utFil.open("teams.dta");
    index = 0;
    for (const auto & val : gTeams) {
        utFil << val.first << " ";
        val.second->writeToFile(utFil);
        if (!(index++ == gTeams.size()-1)) utFil << "\n";
    }
    utFil.close();

    utFil.open("tournaments.dta");
    index = 0;
    for (const auto & val : gTournaments) {
        val->writeToFile(utFil);
        if (!(index++ == gTournaments.size()-1)) utFil << "\n";
    }
    utFil.close();
}

/**
 * Read commands for a tournament
 * 
 */
void readCommandTournament() {
    string nameTournament;
    Tournament* choosenTournament;
    char command;


    if (!gTournaments.empty()) {    //  If there exists tournaments:
        cout << "\n\nTournaments: ";
        for (const auto & tournament : gTournaments)    //  for each tournament:
            cout << tournament->getName() << ", ";      //  Write name.

                                    //  Reads a tournament:
        cout << "\nName of a tournament: ";    getline(cin, nameTournament);

                                    //  Tries to find the tournament provided:
        choosenTournament = findTournament(nameTournament);

        if (choosenTournament) {    //  If tournament exists
            writeMenuTournament(nameTournament);

            command = lesChar("What to do with tournament");

            while (command != 'M') {
                switch(command) {
                    case 'D':   choosenTournament->writeInfo();             break;  // tournament details
                    case 'A':   choosenTournament->addTeam();               break;  // add team
                    case 'S':   choosenTournament->nextStage();             break;  // starts tournament (goes to next stage)
                    case 'P':   choosenTournament->playNextMatch();         break;  // playmatch
                    case 'R':   choosenTournament->writeResults();          break;  // show results
                    case 'M':   cout << "\nReturning to the menu...\n\n";   break;
                }
                writeMenuTournament(nameTournament);
                command = lesChar("\nWhat to do with tournament");
            }

            cout << "Done with chosen tourmanet, returning to main menu\n";

        } else                      //  Tournament doesn't exist:
            cout << "\nCouldn't find tournament!\n";

    } else                          //  No tournaments exist:
        cout << "\nNo tournaments exist!\n";

}

/**
 * Finds a tournament with a given name
 * 
 * @param name - Name of tournament we are trying to find
 * @return Pointer to found tournament (nullptr if none found)
 */
Tournament* findTournament(const string & name) {
    for (const auto & val : gTournaments)       //  For each tournament:
        if (val->getName() == name)             //  If tournament has matching name:
            return val;                         //  Return pointer to found tournament.
    return nullptr;                             //  No tournament found.
}

/**
 * Writes the main menu
 * 
 */
void writeMenu() {
    cout << "\nCommands:\n\tN - New tournament\n"
         << "\tO - Tournament overview (Add teams, play matches, show results)\n"
         << "\tR - See results for all tournaments\n"
         << "\tA - Write info for all tournaments\n"
         << "\tT - New team\n"
         << "\tW - Write info for all teams\n"
         << "\tQ - Quit the program\n";
}

/**
 * Writes the menu for the tournament
 * 
 * @param name - Name of the tournament we are using
 */
void writeMenuTournament(const string & name) {
    cout << "\nCommands for tournament '" << name << "':\n"
         << "\tP - Play a match\n"
         << "\tD - Tournament details\n"
         << "\tA - Add team\n"
         << "\tS - Start the tournament\n"
         << "\tR - Show results\n"
         << "\tM - Back to main menu\n";
}

