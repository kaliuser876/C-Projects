/*
	* This class describes what a PuzzleState is.
	* The blank_position is for decreasing the time
	* to check what tile operator is applicable
*/
#ifndef _PUZZLESTATE_
#define _PUZZLESTATE_
#include <iostream>
#include <vector>

using namespace std;

class PuzzleState
{
public:
	// default constructor
	PuzzleState() { }

	//constructor
	PuzzleState(int n, int m) {
		tiles.resize(n * m);
		rows = n;
		cols = m;
	}
	// destructor
	~PuzzleState() { }

	// copy constructor
	PuzzleState(const PuzzleState& rhs)
		: rows(rhs.rows), cols(rhs.cols),
		tiles(rhs.tiles),
		blank_position_row(rhs.blank_position_row),
		blank_position_col(rhs.blank_position_col)
	{ }

	// Start filling in code
	bool operator==(const PuzzleState& rhs) {
		for (int i = 0; i < tiles.size(); i++) {
			if (tiles.at(i) != rhs.tiles.at(i)) {
				return false;
			}
		}
		return true;
	} 
	bool operator!=(const PuzzleState& rhs) { return !(*this == rhs);}
	const PuzzleState& operator=(const PuzzleState& rhs) { 
		rows = rhs.rows;
		cols = rhs.cols;
		tiles = rhs.tiles;
		blank_position_row = rhs.blank_position_row;
		blank_position_col = rhs.blank_position_col;
		return *this;} //assignment op
	bool isNullState() { return rows == 0; }

	// Movement check
	// When we are at the top we cant move up, when at bottom cant move down. Same thing for left and right

	// Needs test, make sure the test are correct
	bool canMoveUp() {
		if (blank_position_col == 0) return false;
		else return true;
	}
	bool canMoveDown() {
		if (blank_position_col == cols - 1) return false;
		else { return true; }
	}
	bool canMoveLeft() {
		if (blank_position_row == 0) return false;
		else { return true; }
	}
	bool canMoveRight() {
		if (blank_position_row == rows - 1) return false;
		else { return true; }
	}
	// Movement action

	// Needs test, make sure things are moving the correct way
	PuzzleState moveBlankUp() {
		// Ask if Rossi wants us to check if we can move in here or before we call it. 
		// Assuming we check before the function is called
		// (rows * blank_position_col) + blank_position_row) gets the blank position
		tiles.at((rows * blank_position_col) + blank_position_row) = tiles.at((rows * (blank_position_col - 1)) + blank_position_row);
		tiles.at((rows * (blank_position_col - 1)) + blank_position_row) = 0;
		blank_position_col -= 1; // Subtract 1 which moves the blank up one
		return *this;
	}
	PuzzleState moveBlankDown() {
		tiles.at((rows * blank_position_col) + blank_position_row) = tiles.at((rows * (blank_position_col + 1)) + blank_position_row);
		tiles.at((rows * (blank_position_col + 1)) + blank_position_row) = 0;
		blank_position_col += 1; // Add 1 which moves the blank down one
		return *this;
	}
	PuzzleState moveBlankLeft() {
		tiles.at((rows * blank_position_col) + blank_position_row) = tiles.at((rows * blank_position_col) + (blank_position_row - 1));
		tiles.at((rows * blank_position_col) + (blank_position_row - 1)) = 0;
		blank_position_row -= 1; // Subtract 1 which moves the blank left one
		return *this;
	}
	PuzzleState moveBlankRight() {
		tiles.at((rows * blank_position_col) + blank_position_row) = tiles.at((rows * blank_position_col) + (blank_position_row + 1));
		tiles.at((rows * blank_position_col) + (blank_position_row + 1)) = 0;
		blank_position_row += 1; // Add 1 which moves the blank right one
		return *this;
	}

	void read(istream& in) {
		int val = -1;
		bool isUnique;
		// Simple read statement, will only take positive numbers up to N-1. 
		for (int i = 0; i < rows * cols; i++) {
			in >> val;
			// Check if the value is between 0 - N-1 
			if (val >= 0 && val < (rows * cols)) {
				isUnique = true;
				// Check the vector to the point we have filled in.  
				for (int j = 0; j < i && isUnique; j++) {
					// Check if the new val is already inside of the vector
					if (tiles.at(j) == val) {
						i -= 1;
						isUnique = false;
					}
				}
				// Add the value if its unique
				if (isUnique) {
					if (val == 0) {
						blank_position_col = i / cols;
						blank_position_row = i % rows;
					}
					tiles.at(i) = val;
				}
			}
			// If the value is less than 0 or greater than rows * cols
			else {
				i -= 1;
			}	
		}		
	}
	// Print out all the values of the vector
	void print(ostream& out) {
		for (int i = 0; i < tiles.size(); i++) {
			if (i % rows == 0 && i != 0) {
				out << endl;
			}
			out << tiles.at(i) << " ";
		}
	}
	static PuzzleState  NullState;
private:
	int rows;
	int cols;
	vector<int> tiles;
	int blank_position_row;
	int blank_position_col;
};
// Call the read and print functions
istream& operator>>(istream& in, PuzzleState& rhs) {
	rhs.read(in);
	return in;
}
ostream& operator<<(ostream& out, PuzzleState& rhs) {
	rhs.print(out);
	return out;
}
#endif
