#include <iostream>
#include "PuzzleMove.h"
#include "PuzzleState.h"
#include <queue>
#include <string>
#include <algorithm>
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

// Function that will look at possible moves that can be taken, and add them to the queue
void breadthSearch(PuzzleMove node, queue<PuzzleMove>& que, vector<PuzzleState>& visited) {
	PuzzleState temp;

	// Check the posible states we can move to from the current position, node. 
	// Down
	if (node.getState().canMoveDown()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankDown();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, put inside of visited and the que
			visited.push_back(temp);
			que.push(PuzzleMove(temp, node.getState(), MoveType::down));

		}
		// Dont have to do anything if temp is found in visited
	}

	// Left
	if (node.getState().canMoveLeft()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankLeft();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, put inside of visited and the que
			visited.push_back(temp);
			que.push(PuzzleMove(temp, node.getState(), MoveType::left));

		}
		// Dont have to do anything if temp is found in visited
	}

	// Up
	if (node.getState().canMoveUp()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankUp();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, put inside of visited and the que
			visited.push_back(temp);
			que.push(PuzzleMove(temp, node.getState(), MoveType::up));

		}
		// Dont have to do anything if temp is found in visited
	}

	// Right
	if (node.getState().canMoveRight()) {
		temp = node.getState(); // Populate the next state
		temp = temp.moveBlankRight();
		if (find(visited.begin(), visited.end(), temp) == visited.end()) {
			// Not found in visted, put inside of visited and the que
			visited.push_back(temp);
			que.push(PuzzleMove(temp, node.getState(), MoveType::right));

		}
		// Dont have to do anything if temp is found in visited
	}
	// We have pushed all the posible moves for the current node onto the que.

}

// Function for checking if we have gotten to the final state, or need to take more steps. 
int BFS(queue<PuzzleMove>& que, PuzzleState goalState) {
	vector<PuzzleState> visited;
	vector<PuzzleMove> path;
	vector<PuzzleMove> builtPath;
	PuzzleMove node;
	node = que.front();
	// Check if the inital state is the goal state
	if (node.getState() == goalState) {
		return 0;
	}
	// If we get passed, the inital state is not goal state
	visited.push_back(node.getState());

	while (!que.empty()) {
		// Steps to take while que is not empty
		node = que.front();
		que.pop();

		// Need to create a path to follow
		path.push_back(node);

		// Check if we have reached the goal state
		if (node.getState() == goalState) {
			// We have found a solution
			cout << "\n*** Solution Found ***\n";
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

			//Nodes expanded
			// I have a lot more nodes expanded than Rossi, ask if this is okay?
			cout << "Number of nodes expanded : " << visited.size() - 1;
			// Nodes for the path
			cout << "\nPath length to Goal : " << builtPath.size() - 1;
			
			return 1;
		}
		// We are not at goal state, which means we have to continue searching
		breadthSearch(node, que, visited);
	}
	return -1;
}

int main() {
	cout << "***P01 Puzzle Uninformed Search - BFS ***" << endl;
	bool running = true;
	while (running) {
		cout << "<======================================================>" << endl;

		int size = 0;
		cout << "Enter number of rows and columns: ";
		cin >> size;
		PuzzleState startState = PuzzleState(size, size);
		PuzzleState goalState = PuzzleState(size, size);
		PuzzleMove currentMove;
		PuzzleState currentState;
		PuzzleState temp;
		PuzzleState test;

		// Prompt user to enter row size, then the start and desired state
		cout << endl;
		cout << "Enter start state row by row:" << endl;
		cin >> startState;
		cout << "\nEnter goal state row by row:" << endl;
		cin >> goalState;

		
		// BFS uses queue
		queue<PuzzleMove> BFSQue;
		BFSQue.push(PuzzleMove(startState, startState, MoveType::nullMove));

		int state = BFS(BFSQue, goalState);

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