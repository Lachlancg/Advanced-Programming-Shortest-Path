#pragma once
#include <string>
#include <vector>
#include "Arc.h"
using namespace std;
class Arc;
class Node
{
public:
	Node(void);
	Node(string& name, int referenceNum, double northing, double easting);
	~Node(void);
	void addPreviousNodes(vector<Node*>& N);

	
	inline int returnRef() const
	{
		const int ref = m_referenceNum;
		return ref;
	}

	inline double returnNorthing() const
	{
		return m_northing;
	}

	inline double returnEasting() const
	{
		return m_easting;
	}

	inline void addArc(Arc* const arcToAdd)
	{
		m_links.push_back(arcToAdd);
	}

	inline const vector<Arc*>& returnArcs() const
	{
		return m_links;
	}

	inline const string& returnName() const
	{
		return m_name;
	}

	inline int returnSearchID() const
	{
		return m_searchID;
	}

	inline void setSearchID(const int searchID) 
	{
		m_searchID = searchID;
	}

	inline double returnDistance() const
	{
		return m_distance;
	}
	inline void setDistance(const double distance)
	{
		m_distance = distance;
	}
	inline void addPreviousNode(Node* const N)
	{
		previousNodes.push_back(N);
	}
	
	inline const vector<Node*>& returnPreviousNodes() const
	{
		return previousNodes;
	}
	inline void clearPreviousNodes()
	{
		previousNodes.clear();
	}
private:
	string m_name;
	int m_referenceNum;
	double m_northing;
	double m_easting;
	vector<Arc*> m_links;
	int m_searchID;
	double m_distance;
	vector<Node*> previousNodes;
};

