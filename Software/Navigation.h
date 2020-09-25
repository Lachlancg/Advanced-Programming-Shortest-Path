#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Node.h"
#include "Arc.h"
#include <algorithm>
class Navigation
{
	std::ofstream _outFile;

	// Add your code here

public:
	Navigation();
	~Navigation(void);
	Navigation(const Navigation& other)
	{
		places = other.places;
		searchID = other.searchID;
		_outFile = ofstream("Output.txt", ofstream::app);
	}
	Navigation& operator=(const Navigation& other) {
		places = other.places;
		searchID = other.searchID;		
		_outFile = ofstream("Output.txt", ofstream::app);
		return *this;
	}
	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(const std::string& commandString);
	//void writeToFile(const string& output, const string& command);
	void findRoute(Node* currNode, Node& destNode, const string& mode, vector<Node*>& queue, bool& fin);
	vector<Node*> Dijkstra(const Node* const currNode, Node& destNode, const string& mode);
	Node* minDistance();

private:
	int searchID = 0;
	vector<Node*> places;
	void clearPointers();
	inline double pythagoras(const double& easting, const double& northing) const
	{
		double dist = sqrt((pow(easting, 2)) + (pow(northing, 2)));
		
		dist /= 1000;
		return dist;
	}
	inline bool checkPath(const string& journeyMode, const string &arcMode)const
	{
		/*1 ship = ship
			2 bus = bus + ship
			3 car = car + bus + ship
			4 Rail = rail
			5 bike = rail, ship, bus, car, bike
			6 - foot = rail, ship, bus, car, bike, foot*/

		if (journeyMode == arcMode)
		{
			return true;
		}
		else if (arcMode == "Ship")
		{
			if (journeyMode == "Bus" || journeyMode == "Car" || journeyMode == "Bike" || journeyMode == "Foot" || journeyMode == "Ship")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (arcMode == "Rail")
		{
			if (journeyMode == "Foot" || journeyMode == "Bike" || journeyMode == "Rail" )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (arcMode == "Bus")
		{
			if (journeyMode == "Bus" || journeyMode == "Car" || journeyMode == "Bike" || journeyMode == "Foot")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (arcMode == "Car")
		{
			if (journeyMode == "Car" || journeyMode == "Bus" || journeyMode == "Foot" || journeyMode == "Bike")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (arcMode == "Bike")
		{
			if (journeyMode == "Bike" || journeyMode == "Foot")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (arcMode == "Foot")
		{
			if (journeyMode == "Foot")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	}
	inline Node returnNode(const int ref)const
	{
		const int refP = ref;
		const auto firstResult = find_if(places.begin(), places.end(), [&refP](Node* obj) {return obj->returnRef() == refP; });
		const auto index = distance(places.begin(), firstResult);
		return *places[index];
	}
	inline Node* returnNodePointer(const int ref)const
	{
		const int refP = ref;
		const auto firstResult = find_if(places.begin(), places.end(), [&refP](Node* obj) {return obj->returnRef() == refP; });
		const auto index = distance(places.begin(), firstResult);
		return places[index];
	}
	
	// Add your code here
};