#ifndef _TOUR_
#define _TOUR_

#include <vector>
#include <iostream>
#include <chrono>
#include "City.h"
#include <algorithm>
#include <random>

using namespace std;

class Tour{

public:
	// Default constructor
	Tour() {
	}
	// Copy constructor
	Tour(const vector<City> rhsTour) {
		tour = rhsTour;
	}

	// GetTour Function
	vector<City> getTour() {
		return tour;
	}
	// GetCity Function
	City getCity(int index) {
		return tour.at(index);
	}
	// SetCity
	void setCity(City newCity, int index) {
		tour.at(index) = newCity;
		distance = 0;
	}
	// TourSize
	int tourSize() {
		return tour.size();
	}
	
	// getTotalDistance()
	int getTotalDistance() {
		if (distance == 0) {
			int tourDistance = 0;
			// Loop through our tour's cities
			for (int cityIndex = 0; cityIndex < tour.size(); cityIndex++) {
				// Get the city we are traveling from
				City fromCity = getCity(cityIndex);
				// Get the city we are traveling to
				City destinationCity;
				if (cityIndex + 1 < tour.size()) {
					destinationCity = getCity(cityIndex + 1);
				}
				else {
					destinationCity = getCity(0);
				}
				// Calculate teh eculideanDistance
				tourDistance += euclideanDistance(fromCity, destinationCity);
			}
			// Set the distance to the total distance from begining to the end
			distance = tourDistance;
		}
		// Return distance
		return distance; 
	}
	// generateInitalTour
	void generateInitalTour(vector<City>& destinationCities) {
		tour.resize(destinationCities.size());
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine e(seed);
		
		for (int i = 0; i < destinationCities.size(); i++) {
			// Fill the tour with cities
			setCity(destinationCities.at(i), i);
		}
		// Shuffle the tour
		shuffle(tour.begin(), tour.end(), e);
	}
	
	// PrintTour
	void printTour(ostream& out) {
		// Print the name of all the cities inside of tour
		for (int i = 0; i < tour.size() - 1; i++) {
			out << tour.at(i).getName() << " -> ";
		}
		out << tour.at(tour.size() - 1).getName();
	}

private:
	vector<City> tour;
	int distance = 0;

	// Private function euclideanDistance
	static int euclideanDistance(City city1, City city2) {
		int xDistance = abs(city1.getX() - city2.getX());
		int yDistance = abs(city1.getY() - city2.getY());
		double distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		return distance;
	}

};

ostream& operator<<(ostream& out, Tour rhs) {
	rhs.printTour(out);
	return out;
}

#endif // !_TOUR_
