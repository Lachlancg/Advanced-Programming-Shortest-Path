#pragma once
#include <string>
#include "Node.h"
using namespace std;

class Node;
class Arc
{
public:
	Arc(void);
	Arc(Node* destinationNode, const string &mode, const double& size);
	~Arc(void);
	Arc(const Arc& other)
	{
		m_mode = other.m_mode;
		m_size = other.m_size;
		m_destinationNode = other.m_destinationNode;
	}
	Arc& operator=(const Arc& other) {
		m_mode = other.m_mode;
		m_size = other.m_size;
		m_destinationNode = other.m_destinationNode;
		return *this;
	}
	inline double returnSize() const
	{
		return m_size;
	}
	inline Node* returnNode() const
	{
		return m_destinationNode;
	}
	inline const string& returnMode() const
	{
		return m_mode;
	}
	

private:
	Node* m_destinationNode;
	string m_mode;
	double m_size;

	
};

