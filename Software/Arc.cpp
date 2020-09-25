#include "Arc.h"
#include "Node.h"
#include "Navigation.h"

Arc::Arc(): m_destinationNode(nullptr), m_mode(), m_size(0)
{
}
Arc::Arc(Node* const destinationNode, const string& mode, const double& size): m_destinationNode(destinationNode), m_mode(mode), m_size(size)
{
}

Arc::~Arc(void)
{
}

