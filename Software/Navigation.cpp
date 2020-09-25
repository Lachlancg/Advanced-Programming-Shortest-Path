#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cfloat>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;


// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
	try
	{
		clearPointers();
	}
	catch (const std::exception&)
	{

	}
}

bool Navigation::ProcessCommand(const string& commandString)
{

	istringstream inString(commandString);
	string command;
	inString >> command;

	if (command == "MaxDist")
	{
		double easting;
		double northing;
		double dist;
		double maxDist = 0;
		string output;
		string maxDistPlaces;
		for (int i = 0; i < places.size(); i++)
		{
			for (int u = 0; u < places.size(); u++)
			{
				if (places[i]->returnEasting() > places[u]->returnEasting())
				{
					easting = places[i]->returnEasting() - places[u]->returnEasting();
				}
				else
				{
					easting = places[u]->returnEasting() - places[i]->returnEasting();
				}
				if (places[i]->returnNorthing() > places[u]->returnNorthing())
				{
					northing = places[i]->returnNorthing() - places[u]->returnNorthing();
				}
				else
				{
					northing = places[u]->returnNorthing() - places[i]->returnNorthing();
				}
				dist = pythagoras(easting, northing);

				if (maxDist < dist)
				{
					maxDistPlaces = places[i]->returnName() + ", " + places[u]->returnName();
					maxDist = dist;
				}
			}
		}
		ostringstream strs;
		strs << fixed;
		strs << setprecision(3);
		strs << maxDist;
		output = "\n" + maxDistPlaces + ", " + strs.str();
		_outFile << command + output + "\n\n";
		return true;
	}
	else if (command == "MaxLink")
	{
		double maxLink = 0;
		string output;
		string maxLinkRef;
		for (int i = 0; i < places.size(); i++)
		{
			for (int u = 0; u < places[i]->returnArcs().size(); u++)
			{
				if (maxLink < places[i]->returnArcs()[u]->returnSize())
				{
					maxLink = places[i]->returnArcs()[u]->returnSize();
					maxLinkRef = to_string(places[i]->returnRef()) + ", " + to_string(places[u]->returnRef());
				}
			}
		}
		ostringstream strs;
		strs << fixed;
		strs << setprecision(3);
		strs << maxLink;
		output = "\n" + maxLinkRef + ", " + strs.str();
		_outFile << commandString + output + "\n\n";
		return true;
	}
	else if (command == "FindDist")
	{
		Node firstNode;
		Node secondNode;
		istringstream iss(commandString);
		string buffer;
		vector<string> words;
		double easting;
		double northing;

		while (getline(iss, buffer, ' '))
		{
			words.push_back(buffer);
		}

		firstNode = returnNode(stoi(words[1]));
		secondNode = returnNode(stoi(words[2]));

		if (firstNode.returnEasting() > secondNode.returnEasting())
		{
			easting = firstNode.returnEasting() - secondNode.returnEasting();
		}
		else
		{
			easting = secondNode.returnEasting() - firstNode.returnEasting();
		}
		if (firstNode.returnNorthing() > secondNode.returnNorthing())
		{
			northing = firstNode.returnNorthing() - secondNode.returnNorthing();
		}
		else
		{
			northing = secondNode.returnNorthing() - firstNode.returnNorthing();
		}


		ostringstream strs;
		strs << fixed;
		strs << setprecision(3);
		strs << pythagoras(easting, northing);
		const string output = "\n" + firstNode.returnName() + ", " + secondNode.returnName() + ", " + strs.str();
		_outFile << commandString + output + "\n\n";
		return true;
	}
	else if (command == "FindNeighbour")
	{
		Node nodeToSearch;
		istringstream iss(commandString);
		string buffer;
		vector<string> words;
		string neighbours;
		Node* bufferNode;

		while (getline(iss, buffer, ' '))
		{
			words.push_back(buffer);
		}

		nodeToSearch = returnNode(stoi(words[1]));

		for (int u = 0; u < nodeToSearch.returnArcs().size(); u++)
		{
			bufferNode = nodeToSearch.returnArcs()[u]->returnNode();
			neighbours = neighbours + "\n" + to_string(bufferNode->returnRef());
		}

		_outFile << commandString + neighbours + "\n\n";
		return true;
	}
	else if (command == "Check")
	{
		istringstream iss(commandString);
		string buffer;
		vector<string> words;
		vector<Node> route;
		vector<Arc*> bufferArcs;
		string mode;
		Node bufferNode;
		Node* firstCompareNode;
		Node compareNode;
		bool flag = true;
		string outputString = "";
		int count;
		while (getline(iss, buffer, ' '))
		{
			words.push_back(buffer);
		}
		mode = words[1];
		words.erase(words.begin());
		words.erase(words.begin());


		for (int i = 0; i < words.size(); i++)
		{
			bufferNode = returnNode(stoi(words[i]));
			route.push_back(bufferNode);
		}
		for (int j = 0; j < route.size() - 1; j++)
		{

			count = 0;
			bufferArcs = route[j].returnArcs();
			for (int n = 0; n < bufferArcs.size(); n++)
			{
				compareNode = route[j + 1];
				firstCompareNode = bufferArcs[n]->returnNode();
				if (flag == false)
				{
					break;
				}
				if (compareNode.returnRef() == firstCompareNode->returnRef())
				{
					if (checkPath(mode, bufferArcs[n]->returnMode()))
					{
						flag = true;
						outputString = outputString + "\n" + to_string(route[j].returnRef()) + ", " + to_string(compareNode.returnRef()) + ", PASS";
					}
					else
					{
						flag = false;
						outputString = outputString + "\n" + to_string(route[j].returnRef()) + ", " + to_string(compareNode.returnRef()) + ", FAIL";
						break;
					}
				}
				else
				{
					count++;
				}
				if (count == bufferArcs.size())
				{
					flag = false;
					outputString = outputString + "\n" + to_string(route[j].returnRef()) + ", " + to_string(compareNode.returnRef()) + ", FAIL";
					break;
				}
			}
		}
		_outFile << commandString + outputString + "\n\n";
		return true;
	}
	else if (command == "FindRoute")
	{
		istringstream iss(commandString);
		string buffer;
		vector<string> words;
		string outputString = "";

		Node* firstNode = nullptr;
		Node secondNode;
		string mode;
		vector<Node*> queue;
		bool fin = false;

		searchID++;
		while (getline(iss, buffer, ' '))
		{
			words.push_back(buffer);
		}
		mode = words[1];

		firstNode = returnNodePointer(stoi(words[2]));
		secondNode = returnNode(stoi(words[3]));
		findRoute(firstNode, secondNode, mode, queue, fin);
		if (queue.empty())
		{
			outputString = "\nFAIL";
		}
		else
		{
			for (int i = 0; i < queue.size(); i++)
			{
				outputString = outputString + "\n" + to_string(queue[i]->returnRef());
			}

		}
		_outFile << commandString + outputString + "\n\n";
		return true;
	}
	else if (command == "FindShortestRoute")
	{
		istringstream iss(commandString);
		string buffer;
		vector<string> words;
		string outputString = "";

		Node* firstNode = nullptr;
		Node secondNode;
		string mode;
		vector<Node*> queue;
		Node* bufferNode = nullptr;
		vector<Node*> bufferNodes;
		searchID++;
		double minDist = INT_MAX;
		while (getline(iss, buffer, ' '))
		{
			words.push_back(buffer);
		}

		mode = words[1];

		firstNode = returnNodePointer(stoi(words[2]));
		secondNode = returnNode(stoi(words[3]));

		queue = Dijkstra(firstNode, secondNode, mode);
		if (queue.size() == 1)
		{
			outputString = "\nFAIL";
		}
		else
		{
			
			while (queue[0]->returnRef() != firstNode->returnRef())
			{
				for (int i = 0; i < queue[0]->returnPreviousNodes().size(); i++)
				{
					bufferNodes = queue[0]->returnPreviousNodes();
					if (bufferNodes[i]->returnDistance() < minDist)
					{
						bufferNode = bufferNodes[i];
						minDist = bufferNodes[i]->returnDistance();
					}
				}
				queue.insert(queue.begin(), bufferNode);
			}
			for (int i = 0; i < queue.size(); i++)
			{
				outputString = outputString + "\n" + to_string(queue[i]->returnRef());
			}

		}
		_outFile << commandString + outputString + "\n\n";

		return true;
	}
	return false;
}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Compiler Flag to check memory leak

	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);
	if (finPlaces.fail() || finLinks.fail()) return false;

	char line[255];
	string placeName;
	int ref;
	double lat;
	double longi;
	double northing;
	double easting;
	int dest;
	string typeLink;
	Node* destNode = nullptr;
	Node* currNode = nullptr;
	double eastingSum;
	double northingSum;
	double dist;
	while (finPlaces.good())
	{
		finPlaces.getline(line, 255, ',');
		placeName = string(line);

		finPlaces.getline(line, 255, ',');
		istringstream sin(line);
		sin >> ref;

		finPlaces.getline(line, 255, ',');
		istringstream latS(line);
		latS >> lat;

		finPlaces.getline(line, 255, '\n');
		istringstream longS(line);
		longS >> longi;

		LLtoUTM(lat, longi, northing, easting);

		Node* const N = new Node(placeName, ref, northing, easting);
		places.push_back(N);

	}
	while (finLinks.good())
	{
		finLinks.getline(line, 255, ',');
		istringstream refS(line);
		refS >> ref;

		finLinks.getline(line, 255, ',');
		istringstream destS(line);
		destS >> dest;

		finLinks.getline(line, 255, '\n');
		istringstream typeS(line);
		typeS >> typeLink;


		destNode = returnNodePointer(dest);
		currNode = returnNodePointer(ref);
		if (destNode->returnEasting() > currNode->returnEasting())
		{
			eastingSum = destNode->returnEasting() - currNode->returnEasting();
		}
		else
		{
			eastingSum = currNode->returnEasting() - destNode->returnEasting();
		}
		if (destNode->returnNorthing() > currNode->returnNorthing())
		{
			northingSum = destNode->returnNorthing() - currNode->returnNorthing();
		}
		else
		{
			northingSum = currNode->returnNorthing() - destNode->returnNorthing();
		}

		dist = pythagoras(eastingSum, northingSum);

		for (int i = 0; i < places.size(); i++)
		{
			if (places[i]->returnRef() == ref)
			{
				Arc* const A = new Arc(destNode, typeLink, dist);
				places[i]->addArc(A);

			}
			if (places[i]->returnRef() == dest)
			{
				Arc* const B = new Arc(currNode, typeLink, dist);
				places[i]->addArc(B);

			}
		}
	}

	return true;
}

void Navigation::clearPointers()
{
	if (!places.empty())
	{
		for (int j = 0; j < places.size(); ++j)
		{
			for (auto u = places[j]->returnArcs().begin(); u < places[j]->returnArcs().end(); ++u)
			{
				delete *u;
			}
		}
		for (auto i = places.begin(); i != places.end(); ++i)
		{
			delete *i;
		}
	}
}
void Navigation::findRoute(Node* const currNode, Node& destNode, const string& mode, vector<Node*>& queue, bool& fin)
{

	vector<Arc*> neighbourArcs;

	if (currNode->returnRef() == destNode.returnRef())
	{
		queue.push_back(currNode);
		fin = true;
	}

	neighbourArcs = currNode->returnArcs();
	for (int i = 0; i < neighbourArcs.size(); i++)
	{
		if (checkPath(mode, neighbourArcs[i]->returnMode()) && fin != true)
		{
			if (neighbourArcs[i]->returnNode()->returnSearchID() != searchID)
			{
				if (currNode->returnSearchID() != searchID)
				{
					currNode->setSearchID(searchID);
					queue.push_back(currNode);
				}

				findRoute(neighbourArcs[i]->returnNode(), destNode, mode, queue, fin);
			}
		}
	}
}

vector<Node*> Navigation::Dijkstra(const Node* const currNode, Node& destNode, const string& mode)
{
	vector<Arc*> neighbourArcs;
	vector<Node> queue;
	Node* U;
	vector<Node*> destNodes;
	const vector<Node*> previousNodes;
	int ref;
	bool fin = false;
	for (int i = 0; i < places.size(); i++)
	{
		if (places[i]->returnRef() == currNode->returnRef())
		{
			places[i]->setDistance(0);
		}
		else
		{
			places[i]->setDistance(INT_MAX);
		}
		places[i]->clearPreviousNodes();
		queue.push_back(*places[i]);
	}
	while (!queue.empty() && fin != true)
	{
		destNodes.clear();
		destNodes = returnNode(destNode.returnRef()).returnPreviousNodes();
		for (int i = 0; i < destNodes.size(); i++)
		{
			if (destNodes[i]->returnRef() == currNode->returnRef())
			{
				fin = true;
			}
		}

		U = minDistance();
		if (U == nullptr)
		{
			fin = true;
			break;
		}
		ref = U->returnRef();
		const auto firstResult = find_if(queue.begin(), queue.end(), [&ref](Node obj) {return obj.returnRef() == ref; });
		const auto index = distance(queue.begin(), firstResult);
		queue.erase(queue.begin() + index);
		U->setSearchID(searchID);
		neighbourArcs = U->returnArcs();
		for (int i = 0; i < neighbourArcs.size(); i++)
		{
			if (neighbourArcs[i]->returnNode()->returnSearchID() != searchID && U->returnDistance() + neighbourArcs[i]->returnSize() < neighbourArcs[i]->returnNode()->returnDistance() && checkPath(mode, neighbourArcs[i]->returnMode()))
			{
				neighbourArcs[i]->returnNode()->setDistance(U->returnDistance() + neighbourArcs[i]->returnSize());
				neighbourArcs[i]->returnNode()->addPreviousNode(U);
			}
		}
	}
	destNodes.push_back(&destNode);
	return destNodes;
}

Node* Navigation::minDistance()
{
	Node *N = nullptr;
	double minDist = INT_MAX;
	for (int i = 0; i < places.size(); i++)
	{
		if (places[i]->returnSearchID() != searchID && places[i]->returnDistance() < minDist)
		{
			N = places[i];
			minDist = N->returnDistance();
		}
	}
	if (N == nullptr)
	{
		return N;
	}
	else
	{
		return N;
	}
}