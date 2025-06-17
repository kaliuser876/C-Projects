#include "PuzzleMove.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

// Map to make printing direction easy
map<int, string> direction =
{
	{0, "down"},
	{1, "left"},
	{2, "right"},
	{3, "up"},
	{4, "nullMove"}
};

// Compare value to what direction should be returned
string valToDirection(int val) {
	return direction[val];
}


// Couldnt get the standard sort to work, so created a simple sort function. This is not cost efficent, but it does work
void sort(vector <tuple<int, PuzzleMove, int>>& frontier) {
	int currentPos;
	int lowValPos;
	tuple<int, PuzzleMove, int> temp;

	// Sorting the vec from low to high
	for (currentPos = 0; currentPos < frontier.size(); currentPos++) {
		lowValPos = currentPos;
		for (int i = currentPos; i < frontier.size(); i++) {
			if (get<0>(frontier.at(lowValPos)) > get<0>(frontier.at(i))) {
				// Find pos of lowest value
				lowValPos = i;
			}
		}
		// We have found the lowest value, we want to switch the values
		if (lowValPos != currentPos) {
			temp = frontier.at(currentPos);
			frontier.at(currentPos) = frontier.at(lowValPos);
			frontier.at(lowValPos) = temp;
		}
	}

}

// Need to find the manhatten distance for each state, then travel from there
int findManhattanDistance(vector<int> current, vector<int> goal, int size) {
	int distance = 0;
	for (int i = 0; i < current.size(); i++) {
		if (current.at(i) != goal.at(i)) {
			// If they are not the same, then we have to find where the value is
			for (int j = 0; j < goal.size(); j++) {
				if (current.at(i) == goal.at(j)) {
					// We found the correct value
					// Currently counting both occurences. 

					int valueOne = abs((i / size) - (j / size)); // row1 - row2
					int valueTwo = abs((i % size) - (j % size));	// col1 - col2

					distance += valueOne + valueTwo;
				}
			}
		}

	}

	return distance;
}

// Expand the frontier 
void visitNodes(vector< tuple<int, PuzzleMove, int>>& frontier, vector<PuzzleState>& visited, tuple<int, PuzzleMove, int>& fullNode, PuzzleState goal, int size) {
	PuzzleState temp;
	// Check all the moves, keep track of their manhattan distance's
	PuzzleMove node = get<1>(fullNode);

	tuple<int, PuzzleMove, int> pUp;
	tuple<int, PuzzleMove, int> pDown;
	tuple<int, PuzzleMove, int> pRight;
	tuple<int, PuzzleMove, int> pLeft;

	// After finding all the distances, we want to follow the smallest manhattan distance
	// Down
	if (node.getState().canMoveDown()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankDown();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, keep track of distance
			get<1>(pDown) = PuzzleMove(temp, node.getState(), MoveType::down);
			// This is g(n)
			get<2>(pDown) = get<2>(fullNode) + 1;
			// This is f(n) = g(n) + h(n) where h(n) is manhat dist
			get<0>(pDown) = findManhattanDistance(temp.getTiles(), goal.getTiles(), size) + get<2>(pDown);
			frontier.push_back(pDown);
		}
		// Dont have to do anything if temp is found in visited
	}
	// Up
	if (node.getState().canMoveUp()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankUp();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, keep track of distance
			get<1>(pUp) = PuzzleMove(temp, node.getState(), MoveType::up);
			get<2>(pUp) = get<2>(fullNode) + 1;
			get<0>(pUp) = findManhattanDistance(temp.getTiles(), goal.getTiles(), size) + get<2>(pUp);
			frontier.push_back(pUp);
		}
		// Dont have to do anything if temp is found in visited
	}
	// Left
	if (node.getState().canMoveLeft()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankLeft();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, keep track of distance
			get<1>(pLeft) = PuzzleMove(temp, node.getState(), MoveType::left);
			get<2>(pLeft) = get<2>(fullNode) + 1;
			get<0>(pLeft) = findManhattanDistance(temp.getTiles(), goal.getTiles(), size) + get<2>(pLeft);
			frontier.push_back(pLeft);
		}
		// Dont have to do anything if temp is found in visited
	}

	// Right
	if (node.getState().canMoveRight()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankRight();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, keep track of distance
			get<1>(pRight) = PuzzleMove(temp, node.getState(), MoveType::right);
			get<2>(pRight) = get<2>(fullNode) + 1;
			get<0>(pRight) = findManhattanDistance(temp.getTiles(), goal.getTiles(), size) + get<2>(pRight);
			frontier.push_back(pRight);
		}
		// Dont have to do anything if temp is found in visited
	}
	// All of the posible moves have been pushed onto the frontier, sort the frontier to make the best move
	sort(frontier);
}

int aStarSearch(PuzzleState inital, PuzzleState goal, int size) {
	// Check if goal state is the same as the inital state
	if (inital == goal) {
		return 0;
	}
	// If we are still running, then it was not the first state and we have to check more.
	PuzzleMove node; // Simple way to access the move
	tuple<int, PuzzleMove, int> fullNode; // Keep track of the current node we are looking at
	vector<tuple<int, PuzzleMove, int>> frontier; // Keep track of all the paths we can take
	vector<PuzzleMove> path; // Keep track of the nodes we have traveled down
	vector<PuzzleMove> builtPath; // When we are building the actual path taken
	vector<PuzzleState> visited; // Keep track of nodes we have entered into
	frontier.push_back(tuple<int, PuzzleMove, int>(-1, PuzzleMove(inital, inital, MoveType::nullMove), 0)); // Initalizing the frontier

	while (!frontier.empty()) {
		// Grab the first node, pop it off the vector, add it to visited and path
		fullNode = frontier.at(0);
		node = get<1>(fullNode);
		visited.push_back(node.getState());
		path.push_back(node);
		frontier.erase(frontier.begin());

		if (node.getState() == goal) {
			// Print out the path
			cout << "\nSolution:\n\n";
			// Look at the bottom of the list
			int currentLook = path.size() - 1;
			bool found = false;
			// When we have a solution path, we want to follow it all the way up, then read it back down
			while (path.at(currentLook).getState() != path.at(0).getState()) {
				// Look for the parent of the node
				found = false;
				builtPath.push_back(path.at(currentLook));
				// Simple loop for finding the parent of the current node
				for (int i = 0; i < path.size() && !found; i++) {
					if (path.at(currentLook).getParent() == path.at(i).getState()) {
						currentLook = i;
						found = true;
					}
				}
			}
			// Do it one more time to get the root node
			builtPath.push_back(path.at(currentLook));
			// Flip it so we can read it left to right
			reverse(builtPath.begin(), builtPath.end());

			// Print out the states and the moves taken
			cout << builtPath.at(0).getState() << endl << endl;
			for (int i = 1; i < builtPath.size(); i++) {
				cout << valToDirection(builtPath.at(i).getMoveName()) << endl << endl << builtPath.at(i).getState() << endl << endl;
			}

			// Nodes for the path
			cout << "\nSteps to goal : " << builtPath.size() - 1;

			return 1;

		}
		// We are not at the goal state yet, need to expand the frontier
		visitNodes(frontier, visited, fullNode, goal, size);
	}

	return -1;
}


int main() {
	int runNum = 0;
	cout << "***P02 Puzzle Informed Search - A* ***\n" << endl;
	bool running = true;
	while (running) {
		runNum++;
		cout << "****** RUN #" << runNum << " ********************************\n\n";

		int size = 0;
		cout << "Enter number of rows and columns: ";
		cin >> size;
		PuzzleState startState = PuzzleState(size, size);
		PuzzleState goalState = PuzzleState(size, size);


		// Prompt user to enter row size, then the start and desired state
		cout << endl;
		cout << "Enter start state row by row: ";
		cin >> startState;
		cout << "\nEnter goal state row by row: ";
		cin >> goalState;

		int state;

		state = aStarSearch(startState, goalState, size);


		if (state == -1) {
			// Did not find a solution, which means the stack was emptied before we found a solution
			cout << "\nCould not find a solution.";
		}
		else if (state == 0) {
			// the inital state was the goal
			cout << "\n*** Solution found immediately: start state is the goal state. ***";
		}
		else {
			// Dont have to do anything because we are printing it out inside the DFS function
		}
		cout << endl << endl;
	}


	return 0;
}