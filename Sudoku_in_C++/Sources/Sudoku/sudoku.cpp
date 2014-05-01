//Sudoku in C++ by Connor Gallagher

#include <iostream>
#include <ios>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

//function declarations
void readFile(char gameBoard[][9], char solutionBoard[][9], char gameBoard_copy[][9], string boardName, bool &saveLoaded);
void writeFile(char gameBoard[][9], string boardName);
void displayBoard(char gameBoard[][9]);
void userInteract(char gameBoard[][9], char solutionBoard[][9], string &boardName, bool &saveLoaded);
bool inputSquare(char gameBoard[][9], char gameBoard_copy[][9], char solutionBoard[][9], string boardName);
bool displayOption(char gameBoard[][9]);
bool checkInput(char gameBoard[][9], char solutionBoard[][9], int colHeadInt, int rowHeadInt, char value, string boardName);
int endOfGame(char gameBoard[][9], char  solutionBoard[][9]);
int displayArray(char gameBoard[][9], char gameBoard_copy[][9], char solutionBoard[][9]);
bool loadSave (char gameBoard[][9], char solutionBoard[][9], string &boardName, bool &saveLoaded);

int main()
{
	cout<<"***Sudoku in C++! by Connor Gallagher***"<<endl;

	cin.ignore();

	char gameBoard[9][9];		//declare 2-d array
	char gameBoard_copy[9][9]; 	//used in the inputSquare function. Used to check which cells in the sudoku board are editable.
	char solutionBoard[9][9]; 	//holds the finished sudoku puzzle; used to check answers
	char value; 				//the variable that stores the value to be added to the puzzle
	int gameOver = 0;
	bool saveLoaded;

	string Select[]={"1","2","3"};

	//This is the number of the pre-made sudoku board to be used throughout this instance of the program's execution.
	int boardNumber = rand() % 3+1;

	string boardName = Select[boardNumber-1]; //Nuber randomly selected corresponds with index of the Select array.



	userInteract(gameBoard, solutionBoard, boardName, saveLoaded);
	readFile(gameBoard, solutionBoard, gameBoard_copy, boardName, saveLoaded);

	//displayArray(gameBoard, gameBoard_copy, solutionBoard);


	while (gameOver == 0) //will continue the game until the endOfGame condition is true.
		{
			//shows contents of gameBoard to the user.
			displayBoard(gameBoard);

			//asks user for coordinates and value to be placed on the board.
			inputSquare(gameBoard, gameBoard_copy, solutionBoard, boardName);

			//checks to see if the entire sudoku board has been completed properly, thereby ending the game
			gameOver = endOfGame(gameBoard, solutionBoard);
		}

	return 0;
}

//Opens one of the pre-made sudoku boards. Board number is chosen via rand() and constructed in main()
void readFile(char gameBoard[][9], char solutionBoard[][9], char gameBoard_copy[][9], string boardName, bool &saveLoaded)
{

	ifstream fin; //declare file input
	ifstream solution; //input for solution to current sudoku

	cout << string( 100, '\n' );
	cout << "\nOpening sudoku board..."<<endl;

	string infileName = "Boards/sudoku_" + boardName +".txt";
	string solution_path = "Solutions/solution_" + boardName +".txt";

	fin.open(infileName);
	solution.open(solution_path);

	while (!fin.is_open()) //will prompt the user for file location if opening the file fails.
	{
		cout << "\nI could not open your sudoku input file.\n\nPlease enter the path to a sudoku board."<<endl;
		cout << "Look in the folder 'Boards' in the game's directory."<<endl;

		getline(cin,infileName);
		while(infileName == "")
		{
			getline(cin, infileName);
		}
		fin.open(infileName);
	}
	while(!solution.is_open()) //will prompt the user for location of solution file if opening the file fails.
	{
		cout<<"\n\nI could not open the proper sudoku solution file. \n\nPlease enter the path to a sudoku solution."<<endl;
		cout << "Look in the folder 'Solutions' in the game's directory."<<endl;
		cout<<"NOTE: It is VERY IMPORTANT that you select the same number board and solution files.\n(For example: sudoku_1.txt needs solution_1.txt)"<<endl;

		getline(cin, solution_path);
		solution.open(solution_path);
	}
	//read file into array
	for(int row = 0; row < 9; row++) //fill for row
	{
		for (int column = 0; column < 9; column++) //fill for column
		{
			if (saveLoaded != true)
			{
				fin >> gameBoard[row][column];
				solution >> solutionBoard[row][column];
				gameBoard_copy[row][column] = gameBoard[row][column]; //used to check where values can be placed.
																  
				if(gameBoard[row][column] == '0') //if there is a 0
				{
					gameBoard[row][column] = ' '; //display as a gap, rather than a 0
				}
								  //These gaps are the places users can place values.
			}
				else
				{
				fin >> gameBoard_copy[row][column];
				solution >> solutionBoard[row][column];
				}
					
			}
		}
	

	cout<<"\nBoard successfully loaded!\n"<<endl;
	cin.ignore();

	//close the file
	fin.close(); 
	solution.close();
}

//works as a main menu sort of function
void userInteract(char gameBoard[][9], char solutionBoard[][9], string &boardName, bool &saveLoaded)
{
	string choose_str;
	int choose;
		cout << string( 100, '\n' );
		cout << "Options: \n";
		cout << "\t1) Start game\n";
		cout << "\t2) Load Save\n";
		cout << "\t3) Help\n";
		cout << "\t4) Exit\n";
		cout << "Please choose a number: ";

		getline(cin,choose_str);
		while (choose_str == "")
		{
			getline(cin,choose_str);
		}

		choose = atoi(choose_str.c_str());

		switch(choose)
		{
			case 1:
				break;

			case 2:
				loadSave (gameBoard, solutionBoard, boardName, saveLoaded);
				break;
			case 3:
 				cout << string(100, '\n' );
				cout<<"\nWhat is Sudoku?"<<endl;
				cin.ignore(1000, '\n');
				cout<<"\nSudoku is a number puzzle game.\nGiven a 9x9 cube, you place a number between 1 and 9 in each cell, one at a time."<<endl;
				cin.ignore();
				cout<<"The goal: for each row, column, and 3x3 cube, numbers 1-9 are only used once in each."<<endl;
				cin.ignore();
				cout<<"\nDon't worry; I'll tell you if you make a mistake! Good luck!\n";
				cin.ignore();
				userInteract(gameBoard, solutionBoard, boardName, saveLoaded);
				break;
			case 4:
				exit(0);
				break;
			default:
				cout<<"Please make a valid choice."<<endl;
				getline(cin,choose_str);
				while (choose_str == "")
				{
					getline(cin,choose_str);
				}
				choose = atoi(choose_str.c_str());
				break;
		}
		return;
	
}

//saves the current instance of the incomplete game to a .txt that can be opened and played later
void writeFile(char gameBoard[][9], string boardName)
{
	//declare file output
	ofstream fout;
	//saves the file in the folder "Saved_Boards"
	string outfileName = "Saved_Boards/savedBoard_" + boardName + ".txt";


	fout.open(outfileName);

	//write board to file
	for(int row = 0; row < 9; row++)
	{
		for(int column = 0; column < 9; column++)
		{
			if(gameBoard[row][column] == ' ') // if there is a gap
			{
				gameBoard[row][column] = '0'; //save as a 0
			}

			fout << gameBoard[row][column];
		}
	}
	cout << "\nYour game has been successfully saved in the Saved_Boards folder as savedBoard_"<< boardName << ".txt";
	cin.ignore();
	fout.close(); //close the file
}

void displayBoard(char gameBoard[][9])
{
	char rowHeadArray[] = {'A','B','C','D','E','F','G','H','I'};

	//display column header
	cout << "\n   R  S  T   U  V  W   X  Y  Z\n" << endl;

	for (int i = 0; i < 9; i++)
	{
		if (i == 3 || i == 6) cout << "  ---------+---------+--------\n"; //3x3 divider
		cout << rowHeadArray[i] <<":";

		for (int j = 0; j < 9; j++)
		{
			cout << " "<< gameBoard[i][j]<<" ";
			if (j == 2 || j == 5) cout << "|";
			if (j == 8) cout << "\n";
		}
	}

/*
	//display across row 1 (row header, each square, and line break)
	cout << "1:" << gameBoard[0][0] << "  " << gameBoard[0][1]
	     << " " << gameBoard[0][2] << "| " << gameBoard[0][3]
	     << " " << gameBoard[0][4] << "  " << gameBoard[0][5]
	     << "|" << gameBoard[0][6] << "  " << gameBoard[0][7]
	     << " " << gameBoard[0][8] << endl;

	//display across row 2
	cout << "2: " << gameBoard[1][0] << " " << gameBoard[1][1]
	     << " " << gameBoard[1][2] << "|" << gameBoard[1][3]
	     << " " << gameBoard[1][4] << " " << gameBoard[1][5]
	     << "|" << gameBoard[1][6] << " " << gameBoard[1][7]
	     << " " << gameBoard[1][8] << endl;

	//display across row 3
	cout << "3: " << gameBoard[2][0] << " " << gameBoard[2][1]
	     << " " << gameBoard[2][2] << "|" << gameBoard[2][3]
	     << " " << gameBoard[2][4] << " " << gameBoard[2][5]
	     << "|" << gameBoard[2][6] << " " << gameBoard[2][7]
	     << " " << gameBoard[2][8] << endl;

	//3 X 3 line separator
	cout << "   -----+-----+-----" << endl;

	//display across row 4
	cout << "4: " << gameBoard[3][0] << " " << gameBoard[3][1]
	     << " " << gameBoard[3][2] << "|" << gameBoard[3][3]
	     << " " << gameBoard[3][4] << " " << gameBoard[3][5]
	     << "|" << gameBoard[3][6] << " " << gameBoard[3][7]
	     << " " << gameBoard[3][8] << endl;

	//display across row 5
	cout << "5: " << gameBoard[4][0] << " " << gameBoard[4][1]
	     << " " << gameBoard[4][2] << "|" << gameBoard[4][3]
	     << " " << gameBoard[4][4] << " " << gameBoard[4][5]
	     << "|" << gameBoard[4][6] << " " << gameBoard[4][7]
	     << " " << gameBoard[4][8] << endl;

	//display across row 6
	cout << "6: " << gameBoard[5][0] << " " << gameBoard[5][1]
	     << " " << gameBoard[5][2] << "|" << gameBoard[5][3]
	     << " " << gameBoard[5][4] << " " << gameBoard[5][5]
	     << "|" << gameBoard[5][6] << " " << gameBoard[5][7]
	     << " " << gameBoard[5][8] << endl;

	//3 X 3 line separator
	cout << "   -----+-----+-----" << endl;

	//display across row 7
	cout << "7: " << gameBoard[6][0] << " " << gameBoard[6][1]
	     << " " << gameBoard[6][2] << "|" << gameBoard[6][3]
	     << " " << gameBoard[6][4] << " " << gameBoard[6][5]
	     << "|" << gameBoard[6][6] << " " << gameBoard[6][7]
	     << " " << gameBoard[6][8] << endl;

	//display across row 8
	cout << "8: " << gameBoard[7][0] << " " << gameBoard[7][1]
	     << " " << gameBoard[7][2] << "|" << gameBoard[7][3]
	     << " " << gameBoard[7][4] << " " << gameBoard[7][5]
	     << "|" << gameBoard[7][6] << " " << gameBoard[7][7]
	     << " " << gameBoard[7][8] << endl;

	//display across row 9
	cout << "9: " << gameBoard[8][0] << " " << gameBoard[8][1]
	     << " " << gameBoard[8][2] << "|" << gameBoard[8][3]
	     << " " << gameBoard[8][4] << " " << gameBoard[8][5]
	     << "|" << gameBoard[8][6] << " " << gameBoard[8][7]
	     << " " << gameBoard[8][8] << endl;
	     */
}

/*
 So, this part of the code will edit one square of the table. It will
 do this by asking the user for the column header and the row header.
 */

bool inputSquare(char gameBoard[][9], char gameBoard_copy[][9], char solutionBoard[][9], string boardName)
{
	//declare variables
	char columnHeader;
	char rowHeader;
	int columnHeaderInt;
	int rowHeaderInt;
	char value = '0';
	char values[] = "123456789";

	char rowCoordinatesArray[] = {'a','b','c','d','e','f','g','h','i'};
	char columnCoordinatesArray[] = {'r','s','t','u','v','w','x','y','z'};


	cout << "\nWhat cell would you like to edit ([row][column])? "<<endl;
	cout << "Type 'qq' to save and quit."<<endl;
	cin >> rowHeader;
	cin >> columnHeader;

	rowHeader = tolower(rowHeader);
	columnHeader = tolower(columnHeader);

	if(rowHeader == 'q' || columnHeader == 'q')
	{
		writeFile(gameBoard, boardName);
		cin.ignore();
		exit(0);
	}

	for (int i = 0; i < 9; i++)
	{
		if (rowHeader == rowCoordinatesArray[i]) rowHeaderInt = i;
		if (columnHeader == columnCoordinatesArray[i]) columnHeaderInt = i;
	}

	//if location entered by user is not a location on the board, prompts the user to re-enter
	while(rowHeaderInt >= 9 || columnHeaderInt >= 9)
	{
		cout<<"\n"<<rowHeader<<columnHeader<<" is not a valid location on the board.\nPlease re-enter the coordinates of the cell you would like to edit."<<endl;
		cin >> rowHeader >> columnHeader;
		for (int i = 0; i < 9; i++)
		{
			if (rowHeader == rowCoordinatesArray[i]) rowHeaderInt = i;
			if (columnHeader == columnCoordinatesArray[i]) rowHeaderInt = i;
		}
	}

	//if square is full in its initialized form, display "you cannot change that value" message
	/*if(gameBoard_copy[rowHeaderInt][columnHeaderInt] != 0)
	{
		cout << "\nYou cannot change the value in " << rowHeader << columnHeader << ".\n"<< endl;
		inputSquare(gameBoard, gameBoard_copy, solutionBoard, boardName);
	}
	*/
	//else
	//{
		//ask for the value and take in the user input
		cout << "\nWhich number would you like to place at " << rowHeader << "," << columnHeader << "?" << endl;
		cin >> value;



		//make sure the value is within correct range
		while(value != '1' && value != '2' && value != '3' && value != '4' && value != '5' && value != '6' && value != '7' && value != '8' && value !='9')
		{
			cout << value << "is not a valid value in " << rowHeader << columnHeader <<". Please enter a different value."<<endl;
			cin >> value;
		}
		gameBoard[rowHeaderInt][columnHeaderInt] = value;
		//checks the input value against the sudoku's solution array
		checkInput(gameBoard, solutionBoard, columnHeaderInt, rowHeaderInt, value, boardName);


	//}

return true;
}

//checks the value that the user enters in inputSquare function against the solution file.
//If there is an inconsistency, it lets the user know.
bool checkInput(char gameBoard[][9], char solutionBoard[][9], int colHeadInt, int rowHeadInt, char value, string boardName)
{
	/*
	ifstream fin;
	string solution_path = "/Solutions/solution_" + boardName +".txt";


	fin.open(solution_path);

	for (int i = 0; i<9; i++)
	{
		for (int j = 0; j<9; j++)
		{
			fin>>solutionBoard[i][j];
			cout<<solutionBoard[i][j]<<" ";
		}
	}
	fin.close();

	char char1 = gameBoard[rowHead][colHead];
	char char2 = solutionBoard[rowHead][colHead];

	cin.ignore();
	*/

	if(gameBoard[rowHeadInt][colHeadInt] != solutionBoard[rowHeadInt][colHeadInt])
	{
		cout << "\n\nHang on. The value you entered here isn't quite right."<<endl;
		cout << "Try entering a different value.\n"<<endl;
		cin.ignore();
		return false;
	}
	else
	{
		cout<<"\n\nEverything looks good. Keep going!"<<endl;
		cin.ignore();

	}

return true;
}

//checks the gameBoard array against the solutionBoard array. If they match up, then the game ends.
int endOfGame(char gameBoard[][9], char  solutionBoard[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (gameBoard[i][j] != solutionBoard[i][j])
			{
				return 0;
			}

		}
	}

cout << "\n\nYou did it; great job!"<<endl;

return 1;
}

//helper function. Displays the contents of the gameboard array and the solution array.
int displayArray(char gameBoard[][9], char gameBoard_copy[][9], char solutionBoard[][9])
{
	for( int i = 0; i <9; i++)
	{
		for (int j = 0; j<9; j++)
		{
			cout<< gameBoard[i][j];
		}
	}

	cout << "\n";

	for( int i = 0; i <9; i++)
	{
		for (int j = 0; j<9; j++)
		{
			cout<< gameBoard_copy[i][j];
		}
	}

	cout << "\n";

	for( int i = 0; i <9; i++)
	{
		for (int j = 0; j<9; j++)
		{
			cout<< solutionBoard[i][j];
		}
	}

	cout<<"\n\n";

	return 1;
}

//prompts the user to enter the path to his/her save file.
//The tricky part is having the parsing the board number from the string so it knows which solution board to use.
bool loadSave (char gameBoard[][9], char solutionBoard[][9], string &boardName, bool &saveLoaded)
{
	string path = "";
	string new_path;
	int path_length;
	string solution = "Solutions/solution_";
	char numbers[] = {'1','2','3'};

	ifstream fin;
	ifstream test;

	cout << string( 100, '\n' );
	cout<<"Please enter the path to your saved game. \nCheck the Saved_Boards folder."<<endl;

	while (path == "") getline(cin, path);
	path_length = path.length();

	fin.open(path);


	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fin >> gameBoard[i][j];
			if ( gameBoard[i][j] == '0') gameBoard[i][j] = ' ';
		}
	}

	fin.close();
			
	//looks for occurences of board number in the path variable
	for (int i = 0; i < 3; i++) 
	{
		if (path.find(numbers[i], path_length - 6) != -1)
 			{
 			solution += path.substr((path.find(numbers[i], path_length - 6)),(path.find(numbers[i], path_length - 6)) +1 ); //creates a substring
  			}	
  	}

  	new_path = path.substr(0, path.find("Saved_Boards"));
  	new_path += solution;

  	cout << new_path <<endl;

  	test.open(new_path);

  	if (!test.is_open())
  	{
  		cout << "Opening unsuccessful."<<endl;
  		loadSave (gameBoard, solutionBoard, boardName, saveLoaded);
  	}

  	saveLoaded = true;

return true;
}



