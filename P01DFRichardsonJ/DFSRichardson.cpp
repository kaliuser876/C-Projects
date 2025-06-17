#include <iostream>
#include "PuzzleMove.h"
#include "PuzzleState.h"
#include <stack>
#include <string>
#include <algorithm>
#include <map>

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


void visitNodes(stack<PuzzleMove>& stack, vector<PuzzleState>& visited, PuzzleMove& node) {
	//int returnValue = 0;
	PuzzleState newState;
	
	// Down
	if (node.getState().canMoveDown()) {
		// Im setting the value like this because of how I setup PuzzleState's move functions. Could have made it so it returns a temp variable but this returning *this also works. 
		newState = node.getState();
		newState = newState.moveBlankDown();
		// Check if the node has already been visited
		auto it = find(visited.begin(), visited.end(), newState);
		if (it == visited.end()) {
			// The node has not been visited yet, so we should add it to visited and push it onto the stack
			stack.push(PuzzleMove(newState, node.getState(), MoveType::down));
			visited.push_back(newState);
		}
	}

	// Left
	if (node.getState().canMoveLeft()) {
		// Check if the node has already been visited
		newState = node.getState();
		newState = newState.moveBlankLeft();
		auto it = find(visited.begin(), visited.end(), newState);
		if (it == visited.end()) {
			// The node has not been visited yet, so we should add it to visited and push it onto the stack
			stack.push(PuzzleMove(newState, node.getState(), MoveType::left));
			visited.push_back(newState);
		}
	}

	// Up
	if (node.getState().canMoveUp()) {
		newState = node.getState();
		newState = newState.moveBlankUp();
		// Check if the node has already been visited
		auto it = find(visited.begin(), visited.end(), newState);
		if (it == visited.end()) {
			// The node has not been visited yet, so we should add it to visited and push it onto the stack
			stack.push(PuzzleMove(newState, node.getState(), MoveType::up));
			visited.push_back(newState);
		}
	}

	// Right
	if (node.getState().canMoveRight()) {
		// Check if the node has already been visited
		newState = node.getState();
		newState = newState.moveBlankRight();
		auto it = find(visited.begin(), visited.end(), newState);
		if (it == visited.end()) {
			// The node has not been visited yet, so we should add it to visited and push it onto the stack
			stack.push(PuzzleMove(newState, node.getState(), MoveType::right));
			visited.push_back(newState);
		}
	}
	
	
	
}

int DFS(stack<PuzzleMove> &stack, PuzzleState goalState) {
	PuzzleMove node;
	vector<PuzzleState> visited;
	vector<PuzzleMove> path;
	
	// Check if we are already in goal state, if we are break out
	if (stack.top().getState() == goalState) {
		// This is we are given the inital state as the goalState.
		return 0;
	}
	// Otherwise we have to keep going

	// Add the state to visited
	visited.push_back(stack.top().getState());

	// Doing DFS in an interative way
	while (stack.empty() != true) {
		node = stack.top();
		// Check if path.at(i).parentState() aka node == path.at(i-1).currentState(), this means that we were able to move state to state
		if (path.size() > 1) {
			// The path can be checked
			if (path.at(path.size() - 1).getState() == node.getParent()) {
				// If the state at the back of the list is the same as the current nodes parent then they came from one of the 4 possible moves, and we can add it on
				path.push_back(node);
			}
			else {
				// The node is not a possible state from the last node, meaning we have to back track
				while (path.at(path.size() - 1).getState() != node.getParent() && !path.empty()) {
					// We are going to pop off the back untill the currentNodes parent matches the privious state
					path.pop_back();
				}
				// We will break out of it when the state is correct, or path is empty. 
				path.push_back(node);
			}
		}
		else {
			// There is nothing in path, which means we have to fill it with the inital node
			path.push_back(node);
		}

		stack.pop();

		
		if (node.getState() == goalState) {
			// We have found a solution
			cout << "\n*** Solution Found ***\n";
			cout << path.at(0).getState() << "\n\n";
			for (auto i = 1; i < path.size(); i++) {
				cout << valToDirection(path.at(i).getMoveName()) << "\n\n" << path.at(i).getState() << "\n\n";
			}
			// Does Rossi want all of the nodes we touched?
			cout << "Number of nodes expanded: " << visited.size() - 1; // This works because we filled it up with every node, excluding the first one

			cout << "\nPath length to Goal: " << path.size() - 1; // -1 because we are not including the starting state
			
			return 1;
		}
		// Check if we can move to any other positions, if we can then add it to visited and push onto the stack
		visitNodes(stack, visited, node);

	}
	return -1;

}

int main() {
	// Change DFS to BFS on other sheet
	cout << "***P01 Puzzle Uninformed Search - DFS ***" << endl;
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

		// Write BFS or DFS here
		// DFS uses Stack
		stack<PuzzleMove> DFSstack;
		PuzzleMove topMove = PuzzleMove(startState, startState, MoveType::nullMove);
		// Push the starting state onto the stack

		DFSstack.push(topMove);
		int state = DFS(DFSstack, goalState);

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