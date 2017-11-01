/********************************************************************* 
 ** Program Filename: bowl.cpp
 ** Author: Chase Stubblefield
 ** Date: 8 March 2017
 ** Description: A bowling game simulator 
 ** Input: standalone program
 ** Output: standalone program
 *********************************************************************/

#include <iostream>
#include <ctime> // used for seeding the random number generator
#include <cstring> // for c style strings
#include <string>
#include <cstdlib>
#include <unistd.h> // used for sleep()

using namespace std;


/*--- FUNCTION DECLARATIONS ------------------------------------------------------*/
void delete_chararray(int, char***);
void delete_intarray(int, int***);
void display_10th(int***, int, int);
void display_scoresheet(int, int***, int, char***);
void generate_10th_frame(char***, int***, int, int);
void generate_bowls(char***, int***, int);
void get_player_names(int, char***);
void if_strike_on_first(int, int***, int, int, char***);
void setup_intarray(int, int***);
void show_math_row(int***, int, int);
void startup_display(int*);
void symbols(int***, int, int, int);
int do_math(int***, int, int);
int get_posint(string);
/*--------------------------------------------------------------------------------*/ 

int main(){
	int players; // number of players in the game
	char** playerNames; // where we will store the player names
	int** rawBowlValues; // where we will store each individual bowl
	srand(time(NULL)); // seeds the random number generator	
	bool playagain = true; // used for looping the game

while (playagain == true){
	startup_display(&players); // welcomes player and sets int player
	get_player_names(players, &playerNames); // fills and creates playerNames 2D array
	setup_intarray(players, &rawBowlValues); // fills and creates rawBowlValues 2D array
	generate_bowls(&playerNames, &rawBowlValues, players); // incrementally fills the board and calls the scoresheet after each call

//	for (int i = 0; i < players; i++){
//		for (int j = 0; j < 21; j++){
//			cout << rawBowlValues[i][j] << " ";
//			if (j % 2 == 1)
//				cout << "|";
//			if (j == 20)
//				cout << endl;
//		}
//	}

	delete_chararray(players, &playerNames); // deletes all the heap stuffs for playerNames
	delete_intarray(players, &rawBowlValues); // deletes all the heap stuffs for rawBowlValues
	cout << "Play again? (1) - Yes, (0) - No" << endl;
	cin >> playagain;
}
	return 0;
} // end of main function
  // line 62
/*--------------------------------------------------------------------------------*/ 
// Additions:
// 2D arrays! yay! 
// Cool formatting
// Alphabetical function declarations
// Extraneous comments
// Hope to add some cool ANSI escape sequence stuff after I submit this
/*--------------------------------------------------------------------------------*/ 


/********************************************************************* 
 ** Function: startup_display
 ** Description: welcomes player and takes an input for the number of players
 ** Parameters: the players variable to edit
 ** Pre-Conditions: function call
 ** Post-Conditions: the players variable is set
 ** Return: none
 *********************************************************************/
void startup_display(int *players){
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
	cout << "|                             Let's simulate a game of bowling!                         " << endl;
	cout << "| " << endl;			       
	cout << "|			      How many players? ";
	*players = get_posint("");
	cout << "| " << endl;
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
}
/********************************************************************* 
 ** Function: get_player_names 
 ** Description: creates the playerNames array for storing names
 ** Parameters: number of players and the pointer for the array
 ** Pre-Conditions: function call
 ** Post-Conditions: the array is created
 ** Return: none
 *********************************************************************/
void get_player_names(int players, char*** playerNames){

	*playerNames = new char* [players];

	for (int i = 0; i < players; i++){ // for however many players we have
		(*playerNames)[i] = new char[256]; // each of the rows points to a new char array on the heap 19 characters long
		cout << "Enter player " << i + 1 << "'s name: " << endl;
		cin.getline((*playerNames)[i],256);

	}
	cout << " " << endl; // a space for prettier formatting (:
}
/********************************************************************* 
 ** Function: setup_intarray
 ** Description: creates the integer array for storing bowls 
 ** Parameters: number of players and the pointer for the board
 ** Pre-Conditions: function call
 ** Post-Conditions: the array is created
 ** Return: none
 *********************************************************************/
void setup_intarray(int players, int*** rawBowlValues){

	*rawBowlValues = new int* [players];
	for (int i = 0; i < players; i++){ // for however many players we have
		(*rawBowlValues)[i] = new int[23]; // each of the rows points to a new int array on the heap 21 integers long for the bowls and 2 extra to have always be 0
	}
	for (int j = 0; j < players; j++){ 
		for (int k = 0; k < 21; k++){
			(*rawBowlValues)[j][k] = 0; // set all rows and columns to 0 in the rawBowlValues int array
		} 
	}
}
/********************************************************************* 
 ** Function: generate_bowls
 ** Description: incrementally generates random values for the 2D integer array
 ** Parameters: the names of the players, the board, and how many players there are
 ** Pre-Conditions: functions called correctly
 ** Post-Conditions: the integer array is filled and the int i is passed around but not edited anywhere else
 ** Return: none
 *********************************************************************/
void generate_bowls(char*** playerNames, int*** rawBowlValues, int players){

	int i;

	for (i = 0; i < 18; i+=2){ // goes through rounds (columns) after a full set of players is incremented through
		for (int j = 0; j < players; j++){
			display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
			cout << (*playerNames)[j] << ", it's your turn to bowl! Press enter!" << endl;
			cin.ignore();// stops the program until an enter is pressed
			(*rawBowlValues)[j][i] = rand()%11; // does the first bowl
			display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
			cout << "You knocked down " << (*rawBowlValues)[j][i] << " pins!" << endl;

			if( (*rawBowlValues)[j][i] == 10){ // if a strike, set next bowl to 0
				(*rawBowlValues)[j][i+1] = 0;
				cout << "*** You knocked down all 10 pins!! STRIKE!" << endl << "Press enter to let the next person bowl! ***" << endl;
				cin.ignore();
			}
			else{
				cout << "Press enter to bowl again!";
				cin.ignore();// stops the program until an enter is pressed
				(*rawBowlValues)[j][i+1] = rand()%(11- (*rawBowlValues)[j][i] ); // gets a random bowl from the remainder of the previous bowl
//trash				i++; // increments i for sake of the display scoresheet function 
				display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
//trash				i--; // decrement i now for the sake of the for loop
				cout << "You knocked down " << (*rawBowlValues)[j][i+1] << " pins! Press enter to let the next person bowl!" <<  endl; // is i because of the recent i increment
				cin.ignore(); 
			}
			cout << " " << endl; // a space for nice formatting (:
		}
	}
	i = 20;
	generate_10th_frame(playerNames, rawBowlValues, players, i); // after we have finished frames 1-9, call the special 10th frame function
}
/********************************************************************* 
 ** Function: display_scoresheet
 ** Description: displays the specific round and the current board with all names and scores
 ** Parameters: number of players, the board, where the generate bowls loop is at, and the names of the players
 ** Pre-Conditions: variables have been properly set
 ** Post-Conditions: no values are edited
 ** Return: none
 *********************************************************************/
void display_scoresheet(int players, int*** rawBowlValues, int i, char*** playerNames){
	system("clear");
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
	if (i == 20)
	cout << "|                             ROUND: " << 10 << endl;
	else	
	cout << "|                             ROUND: " << i/2 + 1 << endl;
	cout << " ----------------------------------------------------------------------------------------------------" << endl;
	cout << " " << endl;
	cout << "|**** Player ********|" << endl;
	cout << "|  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |  9   | 10   | *total* //" << endl;
	cout << "|---------------------------------------------------------------------|*********/" << endl;
	for (int p = 0; p < players; p++){  // increments through players
		cout << "| " << (*playerNames)[p] << endl; 
		cout << "|---------------------------------------------------------------------|" << endl;
		for (int r = 0; r < 18; r++){ // increments through rounds 1-9
			symbols(rawBowlValues, p, r, i); // displays the correct symbol using function call	
		}
		display_10th(rawBowlValues, p, i); // call function to display the tenth frame	
		cout << "|---------------------------------------------------------------------|" << endl;
		show_math_row(rawBowlValues, p, i);
	}	
}
/********************************************************************* 
 ** Function: show_math_row
 ** Description: displays the math row for the display board function
 ** Parameters: the board, whose turn it is, and where the value generation loop is at
 ** Pre-Conditions: board and player int have been set and loop is going correctly
 ** Post-Conditions: no variables altered
 ** Return: none
 *********************************************************************/
void show_math_row(int*** rawBowlValues, int p, int i){
	int total = 0;

	for (int bowl = 0; bowl < 21; bowl+=2){ // goes from first bowl of frame 1 to first bowl of 10th frame, always on the first bowl of the frame
		if (bowl > i+1) // if the display function is ahead of where we have filled values, just do a "|      "   
			cout << "|     ";
		else {
			if (do_math(rawBowlValues, p, bowl) > 99){     // if our total is 3 digits
				cout << "|   ";
			}
			else if (do_math(rawBowlValues, p, bowl) > 9){ // if our total is 2 digits
				cout << "|    ";
			}
			else {					      // if our total is 1 digit
				cout << "|     ";
			}
			cout << do_math(rawBowlValues, p, bowl); // cout the value
		}
	}

	cout << "|" << endl;
	cout << "|---------------------------------------------------------------------|" << endl;
}
/********************************************************************* 
 ** Function: do_math 
 ** Description: takes the backround 2D integer array and performs math at a point summing backward
 ** Parameters: the scoresheet, whose turn it is, and where the display board function is at
 ** Pre-Conditions: function called correctly
 ** Post-Conditions: the proper total is displayed
 ** Return: returns the total that was calculated
 *********************************************************************/
int do_math(int*** rawBowlValues, int p, int bowl){
	int total = 0;
	for (int r = bowl; r >= 0; r-=2){ // start at a certain frame and go backwards in sets of two, so we are always on first bowl of a frame 
		if( (*rawBowlValues)[p][r] == 10){ // if a strike
			total = total + 10 + (*rawBowlValues)[p][r+2]; // add 10 and the next bowl
			if( (*rawBowlValues)[p][r] == 10 && (*rawBowlValues)[p][r+1] != 0) // if a strike on round 18
				total = total + (*rawBowlValues)[p][r+1]; // add the proper next bowl
			else if ( (*rawBowlValues)[p][r+2] == 10) // if the first number added is also a strike
				total = total + (*rawBowlValues)[p][r+4]; // add the proper next bowl
			else 
				total = total + (*rawBowlValues)[p][r+3]; // add the proper next bowl	
		}
		else if( (*rawBowlValues)[p][r] + (*rawBowlValues)[p][r+1] == 10) // if a spare (only occurs if we don't have a strike)
			total = total + 10 + (*rawBowlValues)[p][r+2]; 
			// starting at the first bowl of a frame, and adding the next bowl to equal 10, add the proper next bowl, which 
			// is ALWAYS the first bowl of the next frame
		else // if not a strike or spare
			total = total + (*rawBowlValues)[p][r] + (*rawBowlValues)[p][r+1];
	}
	return total;
}
/********************************************************************* 
 ** Function: display_10th
 ** Description: a special function for the displaying the 10th frame, due to strange spare/strike conditionals 
 ** Parameters: the bowl values, where we are at in the display board function, and where we are at in the bowl value generation
 ** Pre-Conditions: variables have been set correctly
 ** Post-Conditions: no values are altered
 ** Return: none
 *********************************************************************/
void display_10th(int*** rawBowlValues, int p, int i){
	symbols(rawBowlValues, p, 18, i); // displays the correct symbol using function call	
	symbols(rawBowlValues, p, 19, i); 
	cout << " ";
	if ( (*rawBowlValues)[p][20] == 10 && (*rawBowlValues)[p][19] != 0 ) 
		cout << "X"; // if 10 on third bowl of last frame and second is not 0, then X
	else if ( (*rawBowlValues)[p][20] == 0 && i >= 18)
		cout << "-"; // if 0 on third bowl of last frame, then -
	else if ( (*rawBowlValues)[p][20] == 0 && i < 18)
		cout << " ";
	else if ( (*rawBowlValues)[p][20] + (*rawBowlValues)[p][19]  == 10 && (*rawBowlValues)[p][18] + (*rawBowlValues)[p][19] != 10 ) 
		cout << "/"; // if 2nd and 3rd add to 10, then /.
	else 
		cout << (*rawBowlValues)[p][20];
	cout << "|" << endl;
}
/********************************************************************* 
 ** Function: symbols
 ** Description: interprets the board to display the proper symbols
 ** Parameters: the board, where we are at in the board display generation, and where the value generation process is at
 ** Pre-Conditions: function is properly called
 ** Post-Conditions: no values are changed
 ** Return: none
 *********************************************************************/
void symbols(int*** rawBowlValues, int p, int r, int i){
	if (r%2 == 0){ // if we are on an even numbered r, a first bowl of a frame
		if (r != 18)
			cout << "|  ";
		else
			cout << "| ";
		if ( (*rawBowlValues)[p][r] == 10)
			cout << "X"; // if 10 on first bowl, then strike
		else if ( (*rawBowlValues)[p][r] == 0 && r < i )
			cout << "-"; // if 0 on first bowl if i has already passed this point
		else if ( (*rawBowlValues)[p][r] == 0 )
			cout << " "; // print spaces for unaltered parts of the board
		else
			cout << (*rawBowlValues)[p][r]; // print what was bowled
	}
	else { // if we are on an odd numbered r, a second bowl of a frame
		if (r != 19)
			cout << "  ";
		else
			cout << " ";
		if ( (*rawBowlValues)[p][r] == 10 ) 
			cout << "/"; // if a 0 and a 10, then spare
		else if ( (*rawBowlValues)[p][r-1] != 10 && (*rawBowlValues)[p][r-1] + (*rawBowlValues)[p][r] == 10)
			cout << "/"; // if not a 10 and then sum is 10, then spare
		else if ( (*rawBowlValues)[p][r] == -5 )
			cout << " "; // if we had a strike before, this r will be a -5 and we need to display just a space
		else if ( (*rawBowlValues)[p][r] == 0 && r < i )
			cout << "-"; // if 0 on second bowl and i has already passed this point
		else if ( (*rawBowlValues)[p][r] == 0)
			cout << " "; // other occurences of a 0 should be printed as a space
		else
			cout << (*rawBowlValues)[p][r]; // print what was bowled
	}
}
/********************************************************************* 
 ** Function: generate_10th_frame
 ** Description: generates special values for the tenth frame incrementally
 ** Parameters: the players' names, the board, how many players, and where the board generation process is at
 ** Pre-Conditions: values are properly set
 ** Post-Conditions: values are added to the integer array
 ** Return: none
 *********************************************************************/
void generate_10th_frame(char*** playerNames, int*** rawBowlValues, int players, int i){

	//cout << " ----------------------------------------------------------------------------------------------------" << endl;
	//cout << "|                             ROUND: 10" << endl;
	//cout << " ----------------------------------------------------------------------------------------------------" << endl;

	// only one for loop is necessary since we only need to cycle through the players one time

	for (int j = 0; j < players; j++){ // go through each player just one time.
		cout << (*playerNames)[j] << ", it's your turn to bowl! Press enter!" << endl;
		cin.ignore();// stops the program until an enter is pressed
		(*rawBowlValues)[j][18] = rand()%11; // does the first bowl
		display_scoresheet(players, rawBowlValues, 18, playerNames); // calls the scoresheet
		cout << "You knocked down " << (*rawBowlValues)[j][18] << " pins!" << endl;

		if_strike_on_first(j, rawBowlValues, i, players, playerNames); // call a silly extra function to reduce lines in this function full of disgusting conditionals

		if ( ((*rawBowlValues)[j][18] + (*rawBowlValues)[j][19] == 10) && (*rawBowlValues)[j][18] != 10 || (*rawBowlValues)[j][19] == 10){ 
			// if spare or strike on second frame, bowl from full third
			cout << "*****  Congratulations! You've earned a third bowl! Press enter!  *****" << endl;
			cin.ignore();// stops the program until an enter is pressed
			(*rawBowlValues)[j][20] = rand()%11; // player gets a third bowl from a full set of pins
			display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
			cout << "You knocked down " << (*rawBowlValues)[j][20] << " pins for your extra bowl!" << endl;
		}
		else{
			(*rawBowlValues)[j][20] = 0; // if no spare or strike, extra frame not given and game over.
		}
	}
}

/********************************************************************* 
 ** Function: if_strike_on_first
 ** Description: special conditionals for the first bowl of the tenth frame
 ** Parameters: where the generation is at for the board, the board, number of players, and the players' names
 ** Pre-Conditions: function called correctly
 ** Post-Conditions: values are set for the first bowl of the tenth
 ** Return: none
 *********************************************************************/
void if_strike_on_first(int j, int*** rawBowlValues, int i, int players, char*** playerNames){
	if ( (*rawBowlValues)[j][18] == 10) { // if a strike on the first bowl of the 10th frame
		cout << "*****  Congratulations! You've earned a third bowl! Press enter!  *****" << endl;
		cin.ignore(); // same
		(*rawBowlValues)[j][19] = rand()%11; // gets another bowl from a full set
		display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
		cout << "You knocked down " << (*rawBowlValues)[j][19] << " pins!" << endl; // displays what was bowled
		cout << "Press enter to do your extra bowl!" << endl;
		cin.ignore(); // stops the program until an enter is pressed
		(*rawBowlValues)[j][20] = rand()%(11- (*rawBowlValues)[j][19] ); // gets a random bowl from the remainder of the previous bowl
		display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
		cout << "You knocked down " << (*rawBowlValues)[j][20] << " pins for your extra bowl!" << endl; // displays what was bowled
	}
	else{ // if not a strike
		cout << "Press enter to bowl again!" << endl;
		cin.ignore(); // stops the program until an enter is pressed
		(*rawBowlValues)[j][19] = rand()%(11- (*rawBowlValues)[j][18] ); // gets a random bowl from the remainder of the previous bowl
		display_scoresheet(players, rawBowlValues, i, playerNames); // calls the scoresheet
		cout << "You knocked down " << (*rawBowlValues)[j][19] << " pins!" << endl; // displays what was bowled
	}
}
/********************************************************************* 
 ** Function: get_posint 
 ** Description: checks input for 1-99
 ** Parameters: a string prompt
 ** Pre-Conditions: an input is requested
 ** Post-Conditions: a good input is given
 ** Return: the good input
 *********************************************************************/
int get_posint(string promptInput){
	bool good = false; // good stays false until we get the right input
	string input; // the string the user will enter
	int goodInput; // the good return value

	while(good == false){
		cout << promptInput; // the custom user prompt for input given when the function is called
		getline(cin,input); // where we will take in the user input

		if (input.length() == 1){
			if (input.at(0) >= '1' && input.at(0) <= '9'){
				good = true; // if we have an input of length 1 that is between 1 and 9, good = true and loop ends
			}
			else{
				cout << "Please enter an integer between 1 and 99." << endl; // any strange thing other than good input repeats loop
			}
		}
		else if (input.length() == 2){
			if (input.at(0) >= '1' && input.at(0) <= '9' && input.at(1) >= '0' && input.at(1) <= '9'){
				good = true; // if we have an input of length 2 that is between 1 and 99, good = true and loop ends
			}
			else{
				cout << "Please enter an integer between 1 and 99." << endl; // any strange thing other than good input repeats loop
			}
		}
		else{
			cout << "Please enter an integer between 1 and 99." << endl; // any strange thing other than good input repeats loop
		}
	}
	goodInput = atoi(input.c_str()); // converts 1-99 (stored as a string/ASCII) to an int 
	return goodInput; // the choice variable gets goodInput
}
/********************************************************************* 
 ** Function: delete_chararray
 ** Description: properly deletes the character array to prevent a memory leak
 ** Parameters: the number of players and the playernames array
 ** Pre-Conditions: function call
 ** Post-Conditions: the array has been deleted
 ** Return: none
 *********************************************************************/
void delete_chararray(int players, char*** playerNames){
	for (int i = 0; i < players; i++){
		delete[](*playerNames)[i]; // deletes each row of playerNames
	}
	delete[](*playerNames); // deletes the column of pointers that are left 
	*playerNames = NULL;
}
/********************************************************************* 
 ** Function: delete_intarray
 ** Description: properly deletes the character array to prevent a memory leak
 ** Parameters: the number of players and the bowlvalues array
 ** Pre-Conditions: function call
 ** Post-Conditions: the array has been deleted
 ** Return: none
 *********************************************************************/
void delete_intarray(int players, int*** rawBowlValues){
	for (int i = 0; i < players; i++){
		delete[](*rawBowlValues)[i]; // deletes each row of rawBowlValues
	}
	delete[](*rawBowlValues); // deletes the column of pointers that are left
	*rawBowlValues = NULL;
}
