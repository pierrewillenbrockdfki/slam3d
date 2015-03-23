#include "PoseGraph.hpp"
#include "Solver.hpp"

using namespace slam;

PoseGraph::PoseGraph()
{
	mNextVertexId = 0;
	mNextEdgeId = 0;
}

PoseGraph::~PoseGraph()
{
	
}

Vertex PoseGraph::addVertex(const VertexObject& object)
{
	Vertex n = boost::add_vertex(mGraph);
	mGraph[n] = object;
	mGraph[n].id = mNextVertexId;
	boost::put(boost::vertex_index_t(), mGraph, n, mNextVertexId);
	mVertexMap.insert(VertexMap::value_type(mNextVertexId, n));
	mNextVertexId++;
	return n;
}

Edge PoseGraph::addEdge(Vertex source, Vertex target, const EdgeObject& object)
{
	std::pair<Edge, bool> result = boost::add_edge(source, target, mGraph);
	Edge n = result.first;
	mGraph[n] = object;
	boost::put(boost::edge_index_t(), mGraph, n, mNextEdgeId);
	mNextEdgeId++;
	return n;
}

void PoseGraph::removeVertex(Vertex v)
{
	mVertexMap.erase(mGraph[v].id);
	boost::clear_vertex(v, mGraph);
	boost::remove_vertex(v, mGraph);
}

void PoseGraph::removeEdge(Edge e)
{
	boost::remove_edge(e, mGraph);
}

AdjacencyRange PoseGraph::getAdjacentVertices(Vertex v)
{
	return boost::adjacent_vertices(v, mGraph);
}

Measurement* PoseGraph::getMeasurement(Vertex v)
{
	return mGraph[v].measurement;
}

void PoseGraph::dumpGraphViz(std::ostream& out)
{
	boost::write_graphviz(out, mGraph);
}

VertexObject PoseGraph::getLastVertexObject()
{
	unsigned int s = boost::num_vertices(mGraph);
	Vertex desc = boost::vertex(s-1, mGraph);
	return mGraph[desc];
}

void PoseGraph::setCorrectedPose(unsigned int id, Transform tf)
{
	mGraph[mVertexMap.at(id)].corrected_pose = tf;
}

void PoseGraph::initializeSolver(Solver* solver)
{
	VertexIterator v_begin, v_end;
	boost::tie(v_begin, v_end) = boost::vertices(mGraph);
	for(VertexIterator it = v_begin; it != v_end; it++)
	{
		solver->addNode(mGraph[*it]);
	}
	
	EdgeIterator e_begin, e_end;
	boost::tie(e_begin,e_end) = boost::edges(mGraph);
	for(EdgeIterator it = e_begin; it != e_end; it++)
	{
		Vertex source = boost::source(*it, mGraph);
		Vertex target = boost::target(*it, mGraph);
		solver->addConstraint(mGraph[*it], mGraph[source].id, mGraph[target].id);
	}
}
