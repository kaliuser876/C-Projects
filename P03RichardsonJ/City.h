#ifndef _CITY_
#define _CITY_
#include <string>

using namespace std;

class City {

public:
	// Constructor
	City() {
		x = 0;
		y = 0;
		cityName = "";
	}
	City(string name, int x, int y) {
		cityName = name;
		this->x = x;
		this->y = y;
	}

	// Getter and setter functions
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	string getName() {
		return cityName;
	}

	void setX(int newX) {
		x = newX;
	}

	void setY(int newY) {
		y = newY;
	}

	void setName(string newName) {
		cityName = newName;
	}


private:
	int x;
	int y;
	string cityName;


};



#endif // _CITY_
