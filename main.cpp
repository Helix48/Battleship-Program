//Program will execute a playable battleship with an option of 1 or 2 players
//Final summative project
//Program was created on December 20th, 2016
//(c) Seth Hammell

#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>

using namespace std;

//declare structure
struct aiChecks
{
    bool lastShotOutcome=false;
    bool huntModeActive=false;
    int previousRandChoice=0;
    int previousHit[2];
    int originalHit[2]= {-1};
};

//declare functions
string dummyproof (void); //function to dummyproof the coordinate
int *coordConverter (void); //function to get coordinate from the user
string charFilter (string input); //function to remove non-letter and non-numerical characters from the coordinate
void shipPlacement (string player, int board[10][10]); //function to place ships
bool *shipsPlaced (int board[10][10]); //function to check which ships are placed
int validShip (int *bow, int *stern, int board[10][10], bool *shipCheck); //function that checks if the coordinates entered form a valid ships
void placeShip (int *bow, int *stern, int shipSize, int board[10][10]); //function that actually places a ship
void shotPhase (int board1[10][10], int board2[10][10], string secondPlayer); //function that executes the shot phase
bool gameOverCheck (int board1[10][10], int board2[10][10], string secondPlayer); //function that checks if the game is over
aiChecks takeShot (string player, int board[10][10], int shotBoard[10][10], aiChecks checks); //function that takes in coordinates and takes a shot based off them
aiChecks shipSunkCheck (string player, int board[10][10], bool *boardSunkenShips, aiChecks check); //function that checks if a ship has been sunk
void displayBoard (int board[10][10]); //function that displays a player's board to them
void twoPlayer (void); //function that executes two player mode
void onePlayer (void); //function that executes one player mode
void titleScreen (void); //function that executes the titlescreen
void openingScreen (void); //function that displays the opening screen
void helpScreen (void); //function that displays the help screen
void playerSelectionScreen (void); //function that displays the player selection screen
int *randCoord (void); //function that generates a random coordinate
int *aiShot (int shotBoard[10][10], aiChecks check, int randChoice); //function that will be the main use of shooting for ai
bool newShotCheck (int shotBoard[10][10], aiChecks check, int choice);
int *probableCoord (int shotBoard[10][10]);

int main()
{
    for (int counter=0;counter<100;counter++)
    srand (time(NULL)); //makes the time the program starts at different every time
    titleScreen(); //calls the titlescreen and begins the program
    return 0;
}
int *probableCoord (int shotBoard[10][10])
{
    vector <int> probableCoordinates;
    int randChoice;
    int *finalCoordinate=new int[2];
    int biggestProbablity=0;
    int bow[2],stern[2];
    int probableBoard[10][10]= {{0}};
    int shipSizes[5]= {2,3,3,4,5};
    int check;
    bool shipCheck[5];
    for (int counter3=1; counter3<6; counter3++)
    {
        if (counter3==3)
        {
            shipCheck[1]=true;
        }
        for (int counter4=0; counter4<2; counter4++)
        {
            for (int counter=0; counter<10; counter++)
            {
                for (int counter2=0; counter2<10; counter2++)
                {
                    stern[0]=counter;
                    stern[1]=counter2;
                    bow[counter4]=stern[counter4];
                    bow[(counter4+1)%2]=stern[(counter4+1)%2]+shipSizes[counter3-1]-1;
                    if (!(stern[0]>9 or stern[0]<0 or stern[1]>9 or stern[1]<0 or bow[0]>9 or bow[0]<0 or bow[1]>9 or bow[0]<0))
                    {
                        check=validShip(bow,stern,shotBoard,shipCheck);
                        if (check==counter3)
                        {
                            placeShip(bow,stern,1,probableBoard);
                        }
                    }
                }
            }
        }
    }
    for (int counter=0; counter<10; counter++)
    {
        for (int counter2=0; counter2<10; counter2++)
        {
            if (probableBoard[counter][counter2]>biggestProbablity)
            {
                biggestProbablity=probableBoard[counter][counter2];
            }
        }
    }
    for (int counter=0; counter<10; counter++)
    {
        for (int counter2=0; counter2<10; counter2++)
        {
            if (probableBoard[counter][counter2]==biggestProbablity)
            {
                probableCoordinates.push_back(counter*10+counter2);
            }
        }
    }
    randChoice=rand()%probableCoordinates.size();
    finalCoordinate[1]=probableCoordinates[randChoice]%10;
    finalCoordinate[0]=(probableCoordinates[randChoice]-finalCoordinate[1])/10;
    return finalCoordinate;
}
bool newShotCheck (int shotBoard[10][10], aiChecks check, int choice)
{
    bool validShot=false;
    if (choice==1)
    {
        if (check.previousHit[0]>=1 and shotBoard[check.previousHit[0]-1][check.previousHit[1]]==0)
        {
            validShot=true;
        }
    }
    if (choice==2)
    {
        if (check.previousHit[1]>=1 and shotBoard[check.previousHit[0]][check.previousHit[1]-1]==0)
        {
            validShot=true;
        }
    }
    if (choice==3)
    {
        if (check.previousHit[0]<=10 and shotBoard[check.previousHit[0]+1][check.previousHit[1]]==0)
        {
            validShot=true;
        }
    }
    if (choice==4)
    {
        if (check.previousHit[1]<=10 and shotBoard[check.previousHit[0]][check.previousHit[1]+1]==0)
        {
            validShot=true;
        }
    }
    return validShot;
}
int *aiShot (int shotBoard[10][10], aiChecks check, int randChoice)
{
    int *aiCoord=new int[2]; //declares integer
    if (check.huntModeActive)
    {
        if (randChoice==1)
        {
            aiCoord[0]=check.previousHit[0]-1;
            aiCoord[1]=check.previousHit[1];
        }
        if (randChoice==2)
        {
            aiCoord[0]=check.previousHit[0];
            aiCoord[1]=check.previousHit[1]-1;
        }
        if (randChoice==3)
        {
            aiCoord[0]=check.previousHit[0]+1;
            aiCoord[1]=check.previousHit[1];
        }
        if (randChoice==4)
        {
            aiCoord[0]=check.previousHit[0];
            aiCoord[1]=check.previousHit[1]+1;
        }
    }
    else
    {
        aiCoord=probableCoord(shotBoard);
    }
    return aiCoord;
}
int *randCoord (void)
{
    int *randomCoord=new int[2]; //declares integer
    randomCoord[0]=rand()%10; //generates the first part of the coordinate which is a number from 0-9
    randomCoord[1]=rand()%10; //generates second part of the coordinate
    return randomCoord;
}
void twoPlayer (void)
{
    int board1[10][10]= {{0}}; //declares the boards as 2-deminsional arrays
    int board2[10][10]= {{0}};
    shipPlacement("Player 1",board1); //places player 1's ships
    shipPlacement("Player 2",board2); //places player 2's ships
    shotPhase(board1,board2,"Player 2"); //begins the shot phase of the game
}
void onePlayer (void)
{
    int board1[10][10]= {{0}};
    int board2[10][10]= {{0}};
    //shipPlacement("Player 1",board1);
    shipPlacement("AI",board1); //places the AI's ships
    shipPlacement("AI",board2); //places the AI's ships
    shotPhase(board1,board2,"AI");
}
void titleScreen (void)
{
    int check=0;
    string input,choice; //declares strings
    do //do statement to go repeat until they choose play
    {
        system ("cls");
        openingScreen();
        cin>>choice;
        if (choice!="1" and choice!="2" and choice !="3") //if statement to check if a valid choice has been entered
        {
            cout<<"Invalid choice"<<endl;
            cout<<endl;
        }
        if (choice=="2") //if 2 is entered then go to the helpscreen
        {
            helpScreen();
            system ("cls");
        }
        if (choice=="3") //if 3 is entered end the program
        {
            check=1;
            break;
        }
    }
    while (choice!="1"); //if 1 is entered exit the loop
    system ("cls");
    if (check==0)
    {
        playerSelectionScreen();
        do
        {
            cin>>input; //user enters amount of players
            if (input!="1" and input!="2") //checks user choose a valid choice
            {
                //cout<<"Invalid choice"<<endl;
                //cout<<endl;
            }
        }
        while (input!="1" and input!="2"); //exits if they did
        if (input=="1") //if they entered one begin one player battleship
        {
            system ("cls");
            onePlayer();
        }
        else if (input=="2") //if they entered two begin two player battleship
        {
            system ("cls");
            twoPlayer();
        }
    }
}
void playerSelectionScreen (void)
{
    ifstream file; //declares variables
    string input;
    file.open("playerSelectionScreen.txt"); //opens the file
    if(!file.is_open()) //checks if file is open
    {
        //cout<<"Couldn't open file"; //if not then display error message
    }
    do
    {
        getline (file,input); //takes file text
        //cout<<input<<endl; //and outputs it
    }
    while (!file.eof()); //stops at end of file
    file.close();
}
void helpScreen (void)
{
    system ("cls");
    ifstream file; //declares variables
    string input;
    file.open("helpScreen.txt"); //opens the file
    if(!file.is_open()) //checks if file is open
    {
        //cout<<"Couldn't open file"; //if not then display error message
    }
    do
    {
        getline (file,input);
        //cout<<input<<endl;
    }
    while (!file.eof());
    file.close();
    //cout<<endl;
    system ("pause");
    //cout<<endl;
}
void openingScreen (void)
{
    ifstream file;
    string input;
    file.open("loadoutScreen.txt");
    if(!file.is_open()) //checks if file is open
    {
        //cout<<"Couldn't open file"; //if not then display error message
    }
    do
    {
        getline (file,input);
        //cout<<input<<endl;
    }
    while (!file.eof());
    file.close();
}
void shotPhase (int board1[10][10], int board2[10][10], string secondPlayer)
{
    aiChecks check;
    int shotBoard1[10][10]= {{0}};
    int shotBoard2[10][10]= {{0}};
    //cout<<"Are you ready to begin the shot phase?"<<endl;
    //cout<<endl;
    system ("pause");
    system ("cls");
    bool board1SunkenShips[5]= {false}; //creates a boolean array for each board where each index corresponds to a ship and becomes true if that ship is sunk
    bool board2SunkenShips[5]= {false};
    //cout<<"Now that both players have placed their ships the shooting phase will begin"<<endl;
    for (int gameCheck=true; gameCheck==true; gameCheck=gameOverCheck(board1,board2,secondPlayer)) //for loop to loop until the game is over
    {
        check=takeShot("Player 1",board2,shotBoard2,check); //lets player 1 take a shot
        check=shipSunkCheck(secondPlayer,board2,board2SunkenShips,check); //checks if player 1 sunk any of the other player's ships
        if (!(gameOverCheck(board1,board2,secondPlayer))) //if the game is over exit the for loop
        {
            break;
        }
        check=takeShot(secondPlayer,board1,shotBoard1,check); //lets other player take a shot
        check=shipSunkCheck("Player 1",board1,board1SunkenShips,check); //checks if the other player has sunk any of player 1's ships
        if (check.huntModeActive==false)
        {
            check.originalHit[0]=-1;
        }
    }
}
aiChecks shipSunkCheck (string player, int board[10][10], bool *boardSunkenShips, aiChecks check)
{
    string ships[5]= {"Destroyer","Cruiser","Submarine","Battleship","Aircraft Carrier"}; //creates a string array containing the ship names
    bool *boardShips; //creates a bool array
    boardShips=shipsPlaced(board); //gets boolean values of which ships are still on the board
    for (int counter=0; counter<5; counter++) //for loop to check all ships
    {
        if (boardSunkenShips[counter]==false and boardShips[counter]==false) //checks if the ship was not sunk last turn and if it is now
        {
            if (check.huntModeActive and player!="AI")
            {
                check.huntModeActive=false;
                check.lastShotOutcome=false;
            }
            //cout<<endl;
            //cout<<player<<"'s "<<ships[counter]<<" has been sunk"<<endl; //displays a message indicating the ship sunk and who it belongs to
            boardSunkenShips[counter]=true; //sets the value to true to keep which ships are sunk in memory
        }
    }
    return check;
}
aiChecks takeShot (string player, int board[10][10], int shotBoard[10][10], aiChecks check)
{
    ofstream gameResults;
    gameResults.open("gameResults.txt", fstream::app);
    if (!gameResults.is_open())
    {
        //cout<<"Couldn't open file";
    }
    int choiceCheck=1; //declares variables
    bool numberCheck;
    int randChoice;
    int shotCheck;
    int *shot;
    //cout<<endl;
    //cout<<player<<"'s turn"<<endl; //displays who's turn it is to take a shot
    //cout<<player<<" guesses: ";
    gameResults<<endl;
    gameResults<<player<<"'s turn"<<endl;
    gameResults<<player<<" guesses: ";
    do
    {
        shotCheck=1; //sets shotCheck to one for dummyproofing purposes
        if (player!="AI")
        {
            shot=randCoord();
        }
        else if (player=="AI")
        {
            if (check.lastShotOutcome and check.previousRandChoice!=0)
            {
                randChoice=check.previousRandChoice;
                bool numberCheck=newShotCheck (shotBoard,check,randChoice);
                if (numberCheck==false)
                {
                    check.previousHit[0]=check.originalHit[0];
                    check.previousHit[1]=check.originalHit[1];
                    check.previousRandChoice=0;
                    shotCheck=0;
                }
            }
            else if (choiceCheck==0 and check.originalHit[0]!=-1)
            {
                randChoice=(check.previousRandChoice+2)%4;
                if (randChoice==0)
                {
                    randChoice+=4;
                }
                choiceCheck+=1;
            }
            else
            {
                randChoice=rand()%4+1;
            }
            shot=aiShot(shotBoard,check,randChoice);
            if (shot[0]>9 or shot[0]<0 or shot[1]>9 or shot[0]<0)
            {
                    check.previousHit[0]=check.originalHit[0];
                    check.previousHit[1]=check.originalHit[1];
                    check.previousRandChoice=0;
                shotCheck=0;
            }
        }
        if (board[shot[0]][shot[1]]==-1 or board[shot[0]][shot[1]]>10) //checks if the spot shot is -1 or greater than 10, indicating that it has already been shot, making it invalid
        {
            shotCheck=0;
            if (player!="AI")
            {
                //cout<<char(shot[0]+'A')<<shot[1]+1<<" has already been shot"<<endl; //error message
                gameResults<<char(shot[0]+'A')<<shot[1]+1<<" has already been shot"<<endl;
            }
        }
    }
    while (shotCheck==0);
    if (player=="AI")
    {
        //cout<<char(shot[0]+'A')<<shot[1]+1<<endl;
        gameResults<<char(shot[0]+'A')<<shot[1]+1<<endl;
    }
    if (board[shot[0]][shot[1]]==0) //if statement to check if the spot shot has a value of 0, indicating that it is a miss
    {
        if (player=="AI")
        {
            check.lastShotOutcome=false;
        }
        if (player=="AI" and check.huntModeActive)
        {
            check.previousHit[0]=check.originalHit[0];
            check.previousHit[1]=check.originalHit[1];
        }
        else if (player=="AI")
        {
            check.previousRandChoice=0;
        }
        //cout<<char(shot[0]+'A')<<shot[1]+1<<" is a miss"<<endl; //miss message
        gameResults<<char(shot[0]+'A')<<shot[1]+1<<" is a miss"<<endl;
        board[shot[0]][shot[1]]=-1; //sets the value to -1
        shotBoard[shot[0]][shot[1]]=1;
    }
    if (board[shot[0]][shot[1]]>=1 and board[shot[0]][shot[1]]<=5) //if statement to check if the spot shot contains a value from 1 to 5 indaicating that it is a hit
    {
        if (player=="AI")
        {
            check.huntModeActive=true;
            check.previousRandChoice=randChoice;
            check.lastShotOutcome=true;
            check.previousHit[0]=shot[0];
            check.previousHit[1]=shot[1];
            if (check.originalHit[0]==-1)
            {
                check.originalHit[0]=shot[0];
                check.originalHit[1]=shot[1];
            }
        }
        //cout<<char(shot[0]+'A')<<shot[1]+1<<" is a hit"<<endl; //hit message
        gameResults<<char(shot[0]+'A')<<shot[1]+1<<" is a hit"<<endl;
        board[shot[0]][shot[1]]+=10; //adds 10 to the valid
        shotBoard[shot[0]][shot[1]]=2;
    }
    displayBoard(shotBoard);
    gameResults.close();
    return check;
}
bool gameOverCheck (int board1[10][10], int board2[10][10], string secondPlayer)
{
    bool continueGame=false; //declares booleans
    bool board1Check=false,board2Check=false;
    for (int counter=0; counter<10; counter++) //two counters to run the board variables
    {
        for (int counter2=0; counter2<10; counter2++)
        {
            if (board1[counter][counter2]>=1 and board1[counter][counter2]<=5) //checks if any values in the board1 are from 1 to 5 meaning there are still ships on that board
            {
                board1Check=true;
            }
            if (board2[counter][counter2]>=1 and board2[counter][counter2]<=5) //same thing as if statement above except for board2
            {
                board2Check=true;
            }
        }
        if (board1Check==true and board2Check==true) //if both boards still have a ship in play continueGame is set to true and the game will continue
        {
            continueGame=true;
        }
    }
    if (board1Check==false)
    {
        //cout<<endl;
        //cout<<secondPlayer<<" is victorious"<<endl;
    }
    if (board2Check==false)
    {
        //cout<<endl;
        //cout<<"Player 1 is victorious"<<endl;
    }
    return continueGame;
}
void shipPlacement (string player, int board[10][10])
{
    string ships[5]= {"Destroyer","Cruiser","Submarine","Battleship","Aircraft Carrier"}; //declare variables
    int temp;
    int *bow,*stern;
    bool *shipCheck=shipsPlaced(board); //gets values of which ships are placed
    int check;
    if (player=="Player 2")
    {
        system ("pause");
        system ("cls");
    }
    if (player!="AI") //checks if this is not the ai, because if it is there is no need to displays a message, this is the case for all //cout statements
    {
        //cout<<player<<" input your ships"<<endl;
    }
    for (;;) //endless loop to continue getting ships until all are placed
    {
        shipCheck=shipsPlaced(board); //updates values of which ships are placed
        if (shipCheck[5]==false) //shipCheck[5] represents a true or false value that is only false if all ships are placed, so once it's false the program exits the loop
        {
            break;
        }
        if (player!="AI")
        {
            //cout<<"First coordinate:"<<endl;
            bow=coordConverter(); //gets the coordinates from the user
            //cout<<"Second coordinate:"<<endl;
            stern=coordConverter(); //gets other coordinate
        }
        else if (player=="AI")
        {
            bow=randCoord();
            stern=randCoord();
        }
        if (stern[0]>bow[0]) //if the first part of second coordinate is bigger than the first part of the first coordinate this switches them
        {
            temp=stern[0];
            stern[0]=bow[0];
            bow[0]=temp;
        }
        else if (stern[1]>bow[1]) //if the second part of the second coordinate is bigger than the second part of the first coordinate this switches
        {
            temp=stern[1];
            stern[1]=bow[1];
            bow[1]=temp;
        }
        check=validShip(bow,stern,board,shipCheck); //gets the value of which ship the user has entered (1 being the smallest and 5 being the biggest)
        if (check==0 and player!="AI")
        {
            //cout<<"Invalid ship placement"<<endl;
        }
        else if (check==1)
        {
            placeShip(bow,stern,1,board); //places the ship that cooresponds to the one the user entered
        }
        else if (check==2)
        {
            placeShip(bow,stern,2,board);
        }
        else if (check==3)
        {
            placeShip(bow,stern,3,board);
        }
        else if (check==4)
        {
            placeShip(bow,stern,4,board);
        }
        else if (check==5)
        {
            placeShip(bow,stern,5,board);
        }
        if (check!=0 and player!="AI")
        {
            system ("cls");
            //cout<<"Your "<<ships[check-1]<<" has been placed from "<<char(stern[0]+'A')<<stern[1]+1<<" to "<<char(bow[0]+'A')<<bow[1]+1<<endl; //tells the user that the placement is successful and which ship has been placed
            displayBoard (board); //displays the board of the ships they've placed
        }
    }
}
void displayBoard (int board[10][10])
{
    int moveCount=0;
    for (int counter=9; counter>=0; counter--) //two for loops to run through the board
    {
        for (int counter2=0; counter2<10; counter2++)
        {
            cout<<board[counter2][counter]<<" "; //and display each value of it
            if(board[counter2][counter]!=0)
                moveCount++;
        }
        cout<<endl;
    }
     cout<<moveCount<<endl;
}
void placeShip (int *bow, int *stern, int shipType, int board[10][10])
{
    int v1; //variable called value one, this allows the program to account for different parts of the coordinates being equal and just being able to swap them out by changing the value of this variable to 1 or 0
    if (bow[0]==stern[0]) //checks if the first two parts of the coordinates are equal
    {
        v1=1; //if so the value is set to one
    }
    else if (bow[1]==stern[1]) //checks if the second two parts of the coordinates are equal
    {
        v1=0; //if so the value is set to 0
    }
    for (int counter=stern[v1]; counter<=bow[v1]; counter++) //for loop to run through the board from the smaller coordinate to the bigger one entered
    {
        if (v1==1) //if the value is one
        {
            board[bow[0]][counter]+=shipType; //set the values on the board by increasing the second coordinate
        }
        else if (v1==0) //if the value is two
        {
            board[counter][bow[1]]+=shipType; //set the values on the board by increasing the first coordinate
        }
    }
}
int validShip (int *bow, int *stern, int board[10][10], bool *shipCheck)
{
    int valid=0; //declares variables
    int v1; //variable called value one, this allows the program to account for different parts of the coordinates being equal and just being able to swap them out by changing the value of this variable to 1 or 0
    if (bow[0]==stern[0]) //same code has been commented before
    {
        v1=1;
    }
    else if (bow[1]==stern[1])
    {
        v1=0; //up to here
    }
    else
    {
        return valid;
    }
    for (int counter=stern[v1]; counter<=bow[v1]; counter++) //for loop to run through the board from the smaller coordinate to the larger one similar to the code above
    {
        if (v1==1) //same code as the placeShip function except instead of modifying the values it's checking if they're not equal to 0
        {
            if (board[bow[0]][counter]!=0)
            {
                break; //if they're not equal to zero exits the loop
            }
        }
        else if (v1==0)
        {
            if (board[counter][bow[1]]!=0)
            {
                break; //same code stops at here
            }
        }
        if (counter==bow[v1]) //checks if the for loop is at the last loop
        {
            if (abs(bow[v1]-stern[v1])==1 and shipCheck[0]==false) //five if statements to check if the which ship should be placed (numbers 1 to 5 correspond to the five ships)
            {
                valid=1; //returning valid as one means that the ship "Destroyer" will be placed
            }
            else if (abs(bow[v1]-stern[v1])==2 and shipCheck[1]==false)
            {
                valid=2; //Cruiser
            }
            else if (abs(bow[v1]-stern[v1])==2 and shipCheck[2]==false)
            {
                valid=3; //Submarine
            }
            else if (abs(bow[v1]-stern[v1])==3 and shipCheck[3]==false)
            {
                valid=4; //Battleship
            }
            else if (abs(bow[v1]-stern[v1])==4 and shipCheck[4]==false)
            {
                valid=5; //Aircraft Carrier
            }
        }
    }
    return valid; //valid's value starts at 0 so if it does not fall into any of the if statements above it will be returned as zero which indicates that the coordinates entered were not valid for a ship placement
}
bool *shipsPlaced (int board[10][10])
{
    bool *shipCheck=new bool[6]; //bool array of six bools
    shipCheck[0]=shipCheck[1]=shipCheck[2]=shipCheck[3]=shipCheck[4]=shipCheck[5]=false; //sets all values to false
    for (int x=0; x<10; x++) //two counter loops to run through the board
    {
        for (int y=0; y<10; y++)
        {
            if (board[x][y]==1) //checks for the value of one on the board meaning that the destroyer is placed
            {
                shipCheck[0]=true; //if it is sets the corresponding boolean to true
            }
            else if (board[x][y]==2) //same check as above except for cruiser
            {
                shipCheck[1]=true;
            }
            else if (board[x][y]==3) //submarine
            {
                shipCheck[2]=true;
            }
            else if (board[x][y]==4) //battleship
            {
                shipCheck[3]=true;
            }
            else if (board[x][y]==5) //aircraft carrier
            {
                shipCheck[4]=true;
            }
        }
    }
    if (shipCheck[0]==false or shipCheck[1]==false or shipCheck[2]==false or shipCheck[3]==false or shipCheck[4]==false) //if all ships are placed return true for shipCheck[5] meaning that the ship placement portion is complete
    {
        shipCheck[5]=true;
    }
    return shipCheck;
}
int *coordConverter(void)
{
    int *newcoord=new int[2]; //declares integer with two values
    string input=dummyproof(); //gets dummyproofed coordinate from dummyproof
    newcoord[0]=((int)input[0])-64-1; //takes the string value and subtracts 64 to convert it back to an integer and subtracts one to account for arrays starting at 0
    if (input.length()==2)
    {
        newcoord[1]=((int)input[1])-48-1; //subtracts 48 to convert it back to an intger and then subtracts one to account for arrays starting at 0
    }
    else
    {
        newcoord[1]=10-1; //if input length is 3 only possible integer can be 10 and then subtract one to account for arrays starting at 0
    }
    return newcoord;
}
string dummyproof(void) //dummproofing program function
{
    int check; //declares integer
    string unfilteredInput; //declares strings
    string input;
    do
    {
        cin>>unfilteredInput; //gets intput from user
        input=charFilter(unfilteredInput);
        check=1; //sets check to one to prevent errors
        if (input.length()<2 or input.length()>3) //checks if the length is from 2-3
        {
            check=0; //setting check to 0 means that the dummyproof failed
        }
        else if (input[0]<'A' or input[0]>'J') //checks if the first part of the coordinate is from 'A' to 'J'
        {
            check=0;
        }
        if (input.length()==2) //if the length is two
        {
            if (!isdigit(input[1]) or input[1]=='0') //checks if the second part of the coordinate is a digit and not zero
            {
                check=0;
            }
        }
        if (input.length()==3) //if the length is three
        {
            if (input[1]!='1' or input[2]!='0')//checks if the second part of the coordinate is 10
            {
                check=0;
            }
        }
        if (check==0)
        {
            input=""; //resets variables
            check=0;
            //cout<<"Invalid coordinate, try again."<<endl; //displays error message
        }
    }
    while (check==0); //repeats if conditions are met
    return input; //keeps value of input for use later
}
string charFilter (string unfilteredInput) //function to remove all characters that aren't letters or semi colons
{
    string newString; //declares string
    for (int counter=0; counter<unfilteredInput.length(); counter++) //for loop to fun through the word
    {
        if (toupper(unfilteredInput[counter])>='A' and toupper(unfilteredInput[counter])<='Z')
        {
            newString+=toupper(unfilteredInput[counter]); //if it is then adds it to newstring
        }
        else if (isdigit(unfilteredInput[counter]))
        {
            newString+=toupper(unfilteredInput[counter]); //if it is then adds it to newstring
        }
    }
    return newString;
}
