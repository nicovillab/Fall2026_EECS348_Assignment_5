//Name of program: CW_SudokuSolver.cpp
//Discription: A c++ program that takes in a .txt file with an unfinished sudoku board
//and uses a recursive function to brute-force every possible combination.
//
//Inputs: The inputted text file should be nine lines filled with 
// nine characters (1-9) or an underscore (_) which represents an empty square.
//
//Outputs: the program will print every possible combination 
// as well as the amount of possible solutions to the terminal.
//
// collaborators: none
//other sources: Google Gemini's code, chatgpt's code for pros and cons
// as well as some improvements
//Author: Christopher Warren
//Creation date: 10/15/2026
// Revision date: 10/15/2026
// Revisions:1. added comments  2.included validity checker. 3. implemented while loop to skip unnecessary recursions. 4. restructured solvesudoku function to find empty spaces before calling recursive function


#include <iostream> // input/output
#include <fstream> // reading/writing files
#include <vector> //vectors (for board)
#include <string> //strings

const int SIZE = 9; // out sudoku board will always be a 9x9, so we keep it constant here

// Function Print Board
//  prints the Sudoku board in the same format as it was input
// inputs: our coard matrix
// outputs: does not return anything but prints the board out
void printBoard(const std::vector<std::vector<char>>& board) {
    for (int r = 0; r < SIZE; ++r) { //goes through rows of matrix
        for (int c = 0; c < SIZE; ++c) { //goes through columns of matrix
            std::cout << board[r][c] << (c == SIZE - 1 ? "" : " "); //prints value, no extra space at last colummn
        } // end of column for loop
        std::cout << "\n"; //new line for new row
    } // end of row for loop
    std::cout << "\n"; // we want to separate the matrix from the next print here
} // end of printBoard Function

// Boolean Function isValid
// Function to check if placing a digit at board[row][col] is valid
//(valid meaning it:
//      1. does not equal a same value in the same row
//      2. does not equal a same value in the same column
//      3. does not equal a same value in the same 3x3 grid
// input: board, row and col value, and digit
// output: if it is valid, returns True. 
//         if it is invalid, returns False.
bool isValid(const std::vector<std::vector<char>>& board, int row, int col, char val) {
    // for the length and width of the board 
    for (int i = 0; i < SIZE; ++i) {
        if (board[row][i] == val) return false; //checks rows for a similar value
        if (board[i][col] == val) return false; // checks columns for a similar value
    } // end of for loop

    // Check the local 3x3 grid box constraint
    // our int row divided by three gives an int value 
    //that is always rounded down,
    // so multiplying by 3 
    //gives either 0, 3, or 6 - starting rows/columns
    int startRow = (row / 3) * 3; 
    int startCol = (col / 3) * 3;

    //our rows and columns for loops now only check the 3x3 grid 
    //starting where those values are
    for (int r = startRow; r < startRow + 3; ++r) {
        for (int c = startCol; c < startCol + 3; ++c) {
            if (board[r][c] == val) return false; //check 3x3 grid for similar value.
        }// end of if statement
    }// end of for statement

    return true; // if there are no similar values already put, our digit is valid.
} // end of isValid function

// void Function solveSudoku
// Recursive function to find and print all possible combinations
// recursively checks every index in the matrix, calling the function for the next
// if it is not empty already,
// and then trying all possible candidates for that box, checking if valid, 
//and calling the funtion for the next index
//
//inputs: the board, the current row and column index, the global solution count
//outputs: none, but when we fill all slots we call the printBoard function. 
// Additionally, when we find a solution, we increase the solution count variable, 
// so when it is over we will also know the amount of solutions.
void solveSudoku(std::vector<std::vector<char>>& board, int row, int col, int& solutionCount) {

    //try numbers '1' - '9' in this index
    // we use a for loop to check all values
    // if a number can be placed, input it it, call the function again,
    // and then remove it for the next number
    for (char val = '1'; val <= '9'; ++val) {
        if (isValid(board, row, col, val)) {
            board[row][col] = val; // Tentatively place value
            
            //update 3: we find the next empty value first before recursing
            //nextrow and nextcol are the values we will recurse to next
            int nextrow = row;
            int nextcol = col + 1;
            bool empty_exists = false; //this checks if the board is alreaxdy complete

            while (nextrow<SIZE){ // while we arent at the end of the board

                if (nextcol == SIZE){ // if we are at the end of the row first
                    nextrow = nextrow+1; //move to next row
                    nextcol = 0; //first item in row
                    continue; //skip the rest
                }//end of if statement

                if (board[nextrow][nextcol]=='_' and nextrow<SIZE){ //if we find an empty space
                    empty_exists = true; //keep track
                    break; //end while loop
                }//end of if statement
                nextcol = nextcol+1; //next index in row
            }//end of while loop
            
            // call function for next index if an empty exists
            if (empty_exists){
                solveSudoku(board, nextrow, nextcol, solutionCount);
            }else{ //else, the board is full, so we print it out
                solutionCount++; // increase the solution counter 
                std::cout << "--- Solution #" << solutionCount << " ---\n"; // prints current attempt (left in cause i think its helpful)
                printBoard(board); //function to print board
            } // end of if else statement
            board[row][col] = '_'; // Backtrack
        } // end of if statement
    }// end of for loop
} // end of solveSudoku function

//boolean function validboard
//checks if the initial board is valid to start.
// if the board isnt solveable form the beginning, we can save lots of time by checking beforehand.
//inputs: the board matrix
//outputs; if the board has no invalid numbers, return True
//         if the board has an invalid number, return False
bool validboard(std::vector<std::vector<char>>&board){
    char temp = '_';
    for (int row = 0; row < SIZE; ++row) { //go through rows
        for (int col = 0; col < SIZE; ++col){ // go through columns
            if (board[row][col] != '_'){ // if we find an index that isnt empty
                temp = board[row][col]; //move the value to a temporary variable
                board[row][col] = '_'; //change the index value to a blank
                if (!isValid(board, row, col, temp)){ // if the value is invalid there initially
                    return false; // the board doesnt have any solutions.
                } //end of if statement
                //otherwise return the value and continue
                board[row][col] = temp;
            }// end of if statement
        }//end of column for loop
    }//end of row for loop
    return true; // if all digits are valid return true
}//end of validboard function


//main function
//takes in .txt file and checks it's able to be read and the right size
// before moving it to the solve sudoku function
// no initial inputs but asks for .txt file in folder.
// will end program if issues were found with the file.
// will call sudoku solver function which will output all possible sudoku combinations
//of the given unfinished board
// as well as the number of possible solutions.

int main() { 
    std::string filename; // var for user file
    std::cout << "Enter the name of the Sudoku file (e.g., puzzle.txt): ";
    std::cin >> filename; //put in the user file given

    //check if file can be opened
    std::ifstream infile(filename);
    if (!infile) { // if it cant
        std::cerr << "Error: Could not open file " << filename << "\n"; //print to terminal
        return 1; // end program
    }

    //print our board variable
    std::vector<std::vector<char>> board(SIZE, std::vector<char>(SIZE)); 

    // Read the grid from the file
    for (int r = 0; r < SIZE; ++r) { // row for loop
        for (int c = 0; c < SIZE; ++c) { //column for loop
            if (!(infile >> board[r][c])) {//if there are less characters than the board expected:
                std::cerr << "Error: Invalid file format. Expected a 9x9 grid.\n"; //tell user
                return 1;// end program
            }
        }
    }
    infile.close(); // close file since we are done with it here

    int solutionCount = 0; //var that handles total amount of solutions found
    bool found_first = true;
    int nextrow = 0;
    int nextcol = 0;

    if (validboard(board)){ // if board is valid initially

        //this part of the code finds the first empty part in the list.
        
        for (int r = 0; r < SIZE; ++r) { // row for loop
            for (int c = 0; c < SIZE; ++c) { //column for loop
                if ((board[r][c])=='_') {//if the index is empty
                    nextrow = r; //keep track of row
                    nextcol = c;// keept track of column
                    found_first = true; //keeep track that an empty slot was found
                    break; //break from column loop
                    }//end of column loop
                }
                if(found_first){ //if empty slot was found, break from this one too
                break;
                }//end of row loop
            }//end of for loop
            if(found_first){ //if an empty slot was found, call the solve sudoku function
                solveSudoku(board, nextrow, nextcol, solutionCount); //find all solutions to given sudoku puzzle.
            }else{ //otherwise, the board is already full, so print the board with one solution
                printBoard(board);
                solutionCount = solutionCount + 1;
            }
        }else{ // if not tell user and end
            std::cout << "This board configuration is invalid.\n";
            return 1;//end program
        }
    

    // print if no valid solution is found
    if (solutionCount == 0) {
        std::cout << "No solution found.\n";
    } else { // else tell the user total amount of solutions.
        std::cout << "Total solutions found: " << solutionCount << "\n";
    }

    return 0; //end program
}// end of main
