#include<iostream>
#include<string>
#include<fstream>
#include "Tour.h"

using namespace std;

// Calculates the acceptance probabilty
static double acceptanceProbability(int currentDistance, int newDistance, double temperature) {
	if (newDistance < currentDistance) {
		return 1.0;
	}
	return exp((currentDistance - newDistance) / temperature);
}

int main() {
	cout << "*** TSP using Simulated Annealing ***\n\n\n";
	int temp;
	float coolingRate;
	string fileName;
	ifstream cityFile;
	string outputString;
	string num1;
	string num2;
	char firstLetter = NULL;
	City tempCity;
	vector<City> destinationCities;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);
	Tour best;
	int solutionsTested = 0;
	int solutionsAccepted = 0;

	// Ask user to enter temperature 
	cout << "Starting Temerature: ";
	cin >> temp;

	// Ask user to enter cooling rate
	cout << "\n\nCooling Rate: ";
	cin >> coolingRate;

	// Ask user to enter name of file
	cout << "\n\nPlease enter a file name (quit to end): ";
	cin >> fileName;
	if (fileName.compare("quit") == 0) {
		cout << "Bye!";
		return 0;
	}
	// Read in from a file the city and heats
	cityFile.open(fileName);
	if (cityFile.is_open()){
		// The user enter a valid file name and it successfuly opened. 
		cout << "\n\nCities: \n\n";
		while (cityFile.good()) {
			getline(cityFile, outputString);
			// Update the city name
			tempCity.setName(firstLetter + outputString);
			//cout << "STRING: " << firstLetter << outputString << endl;

			// Reading in the x and y positions, then sets the city values
			cityFile >> num1;
			cityFile >> num2;
			tempCity.setX(stoi(num1));
			tempCity.setY(stoi(num2));

			// Push the new city onto destinationCities
			destinationCities.push_back(tempCity);
			cout << "\t" << tempCity.getName() << " " << num1 << " " << num2 << "\n\n";
			//cout << "Num1: " << num1 << " and Num2: " << num2 << endl;

			// The pointer in the file has been weird, and this fixes it
			cityFile >> firstLetter;
		}
		

		// Make sure to close the file when we are done with it.
		cityFile.close();
	}
	else {
		// There was an error and we didnt open the file
	}
	Tour currentSolution;
	currentSolution.generateInitalTour(destinationCities);
	
	cout << "\nInitaial solution distance: " << currentSolution.getTotalDistance() << "\n";
	cout << "Tour: \n" << currentSolution; 

	best = Tour(currentSolution.getTour());

	while (temp > 1) {
		// Create new neighbor tour
		Tour newSolution = Tour(currentSolution.getTour());
		solutionsTested++;
		
		// Get a random positions in the tour
		int tourPos1 = rand() % newSolution.tourSize();
		int tourPos2 = rand() % newSolution.tourSize();

		// Make sure they are not the same
		while(tourPos1 == tourPos2){tourPos2 = rand() % newSolution.tourSize();}

		// Get the cities at the selected pos
		City citySwap1 = newSolution.getCity(tourPos1);
		City citySwap2 = newSolution.getCity(tourPos2);
		
		// Swap em
		newSolution.setCity(citySwap1, tourPos2);
		newSolution.setCity(citySwap2, tourPos1);

		// Get the energy of the solutions
		int currentDistance = currentSolution.getTotalDistance();
		int neighborDistane = newSolution.getTotalDistance();

		// Decide if we should accept the neighbor
		double randNumb = (rand() % 1000) / 1000.0;
		if (acceptanceProbability(currentDistance, neighborDistane, temp) > randNumb) {
			currentSolution = Tour(newSolution.getTour());
			solutionsAccepted++;
		}

		// Keep track of the best solution
		if (currentSolution.getTotalDistance() < best.getTotalDistance()) {
			best = Tour(currentSolution.getTour());
			
		}
		

		// Cool system
		temp *= 1 - coolingRate;
	}
	int bestDistance = best.getTotalDistance();

	cout << "\n\nNumber of states tested: " << solutionsTested;
	cout << "\nNumber of states accepted: " << solutionsAccepted;
	cout << "\n\nFinal solution distance: " << bestDistance;
	cout << "\nTour: \n" << best;


	return 0;
}