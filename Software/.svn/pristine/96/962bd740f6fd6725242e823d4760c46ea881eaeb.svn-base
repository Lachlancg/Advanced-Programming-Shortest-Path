#include "Node.h"



Node::Node(void) : m_name(""), m_referenceNum(0), m_northing(0), m_easting(0), m_searchID(0), m_distance(0)
{
}
Node::Node(string& name, const int referenceNum, const double northing, const double easting) : m_name(name), m_referenceNum(referenceNum), m_northing(northing), m_easting(easting), m_searchID(0), m_distance(0)
{

}

Node::~Node(void)
{
	
}
void Node::addPreviousNodes(vector<Node*>& N)
{
	for (int i = 0; i < N.size(); i++)
	{
		previousNodes.push_back(N[i]);
	}
}







