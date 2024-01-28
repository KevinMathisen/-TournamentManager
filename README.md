# Tournament Manager

## Installation

### Requirements
To install and run the LOL Tournament manager you need:
- A computer running Windows
- Installed C++ library

### How to install
Follow these steps to download program:
- Download a .zip file from the repo using [this link](https://github.com/KevinMathisen/TournamentManager), clicking **Code** then **Download ZIP** 
- Unzip the zip file downloaded
- Run the TournamentManager.exe file to start the program

Alternatively, visit the [git repo](https://github.com/KevinMathisen/TournamentManager) and download the source code and all related documentation and run the source code with a c++ compiler.

## Usage

When running the program a CLI window should appear and you should be able to type commands.  
 
### How to navigate around the main menu
 
- Press **N** to create a new tournament
- Press **O** to see the overview of a tournament
- Press **R** to see all results
- Press **A** to see all existing tournaments
- Press **T** to create a new team
- Press **W** to see all existing teams
- Press **Q** to quit the program
 
Please note that if a user tries using the **R**,**O**, or **A** command before any tournaments have been added they will be automatically sent back to the main menu and provided with an error message.

If a user tries using the **W** command before any teams have been added, they will also automatically be sent back to the main menu. 
 
### Creating a tournament
 
In the main menu after pressing **N** you will be taken to the create tournament screen. The screen presents you with 4 fields of information that must be filled out in order for a tournament to be created. The fields to be filled in are as follows.
 
- Tournament name – Fill out the name of the tournament in this field. Can't be the same name as existing tournaments.
 
- Tournament description – Fill out the tournament description in this field.

- Amounts of teams participating – Choose the number of teams that will participate in the tournament in this field. Please note that the number of teams can only be 2, 4, 8, or 16.
 
- Teams to be added - Choose the teams from the existing list of teams to be added to the tournament. If you are done adding teams, you can type enter to finish. 
 

### Seeing an overview of a tournament
 
In the main menu after pressing **O** you will be taken to the choose tournament screen. In the choose tournament screen please present a valid tournament name. If the presented name is valid the user is taken to that tournament's overview screen, otherwise, the user is sent back to the main menu. In the overview screen the following commands will be available.
 
- Press **D** to see the tournament details
- Press **A** to add a team to the tournament
- Press **S** to start the tournament 
- Press **P** to play a match 
- Press **R** to see the results for the tournament
- Press **M** to go back to the main menu
 
### Seeing tournament details

In the tournament overview screen after pressing **D** you will be taken to the tournament preview screen. This screen provides the user with the number of teams, bracket type, and the current stage of the ongoing tournament.

### Playing a match

In the tournament overview screen after pressing **P** you will be taken to the play match screen, as long as you have started the tournament. You can start the tournament with command **S**. The names of the two participating teams are displayed. You will then be prompted for the match results, where the first team to get 3 points wins the match. This is because the matches are played as best-of-five. The result is then registered and the user gets sent back to the tournament overview screen.

### Starting the tournament

In the tournament overview screen after pressing **S** the tournament will be started. Please note that before starting the tournament you need to add all the required teams. You can do this with the command **A**.

### Seeing the results

Results can either be seen from the Main Menu by pressing **R** for an overview of the results across all tournaments or from the Tournament overview screen by pressing **R** to see the results for a specifically chosen tournament.


### Adding a team to a tournament

In the tournament overview screen after pressing **A** you will be taken to the add a team screen, as long as there is space for more teams. In the add team screen please present a valid team name. If the presented name is valid the specified team gets added to the tournament, otherwise, the user is sent back to the main menu.


## License

This project is licensed under the [MIT License](LICENSE). See the `LICENSE` file for more details.
