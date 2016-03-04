#include <assert.h>
#include <set>
#include <limits.h>
#include "../graph/Graph.h"
#include "../graph/GraphBuilder.h"

using namespace std;

// Lab 2
list<Vertex*> recoverShortestPath(vector<Vertex*> shortestPaths, Vertex* last) {
    list<Vertex*> result;

    result.push_back(last);
    for (Vertex* i = shortestPaths[last->id]; i != nullptr ; i = shortestPaths[i->id])
        result.push_back(i);

    result.reverse();
    return result;
}
int getMinDistanceIndex(vector<int> &distances, vector<bool> &marked) {
    int min = INT_MAX;
    int minIndex = INT_MAX;

    for (int i = 0; i < distances.size(); i++)
        if (!marked[i] && distances[i] <= min) {
            min = distances[i];
            minIndex = i;
        }

    assert(minIndex != INT_MAX);
    return minIndex;
}
list<Vertex*> Dijkstra(Graph* graph, int startId, int destinationId) {
    vector<Vertex*> vertexes = graph->getAllVertexes();

    // Preparations
    vector<int> distances(graph->getVertexAmount(), INT_MAX);
    vector<bool> marked(graph->getVertexAmount(), false);
    vector<Vertex*> shortestPreviouses(graph->getVertexAmount(), nullptr);
    distances[startId] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < vertexes.size()-1; count++) {
        int minIndex = getMinDistanceIndex(distances, marked);
        marked[minIndex] = true;

        if (minIndex == destinationId)
            break;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (list<Edge*>::iterator edgeIt = vertexes[minIndex]->neighborhood.begin(); edgeIt != vertexes[minIndex]->neighborhood.end(); edgeIt++) {
            if (!marked[(*edgeIt)->destination->id] && distances[minIndex] + (*edgeIt)->weight < distances[(*edgeIt)->destination->id]) {
                distances[(*edgeIt)->destination->id] = distances[minIndex] + (*edgeIt)->weight;
                shortestPreviouses[(*edgeIt)->destination->id] = (*edgeIt)->source;
            }
        }
    }

    return recoverShortestPath(shortestPreviouses, vertexes[destinationId]);
}

list<Vertex*> BellmanFord(Graph* graph, int startId, int destinationId) {
    vector<Vertex*> vertexes = graph->getAllVertexes();

    // Preparation
    vector<int> distances(graph->getVertexAmount(), INT_MAX);
    vector<Vertex*> shortestPreviouses(graph->getVertexAmount(), nullptr);
    distances[startId] = 0;

    for(int i = 0; i < vertexes.size()-1; i++) {
        for(vector<Vertex*>::iterator vertexIt = vertexes.begin(); vertexIt != vertexes.end(); vertexIt++) {
            if (distances[(*vertexIt)->id] == INT_MAX) { // int overflow may screw comparison up
                continue;
            } else {
                for (list<Edge *>::iterator neighborhoodIt = (*vertexIt)->neighborhood.begin();
                     neighborhoodIt != (*vertexIt)->neighborhood.end(); neighborhoodIt++) {
                    if (distances[(*neighborhoodIt)->destination->id] > distances[(*neighborhoodIt)->source->id] + (*neighborhoodIt)->weight) {
                        distances[(*neighborhoodIt)->destination->id] = distances[(*neighborhoodIt)->source->id] + (*neighborhoodIt)->weight;
                        shortestPreviouses[(*neighborhoodIt)->destination->id] = (*neighborhoodIt)->source;
                    }
                }
            }
        }
    }

    return recoverShortestPath(shortestPreviouses, vertexes[destinationId]);
}

// Lab 3
bool contains(vector<Vertex*> vertexes, int id) {
    for (vector<Vertex*>::iterator it = vertexes.begin(); it != vertexes.end(); it++)
        if ((*it)->id == id)
            return true;

    return false;
}
Graph* Prim(Graph* graph) {
    GraphBuilder builder(graph->getVertexAmount());
    vector<Vertex*> vertexes = graph->getAllVertexes();
    std::set<Edge> allAvailableEdges;

    // preparations
    int startId = 0; //rand() % vertexes.size();
    builder.addVertex(startId);
    for(list<Edge*>::iterator startEdgesIt = vertexes[startId]->neighborhood.begin(); startEdgesIt != vertexes[startId]->neighborhood.end(); startEdgesIt++ )
        allAvailableEdges.insert(**startEdgesIt);

    while(builder.getCurrentGraphVertexesAmount() != graph->getVertexAmount()) {
        vector<Vertex*> currentVertexes = builder.getResult()->getAllVertexes();
        set<Edge>::iterator lightweight = allAvailableEdges.begin();

        if (!contains(currentVertexes, lightweight->destination->id)) {
            builder.addVertex(lightweight->destination->id);
            for(list<Edge*>::iterator neighboursIt = lightweight->destination->neighborhood.begin(); neighboursIt != lightweight->destination->neighborhood.end(); neighboursIt++ ) {
                if (!(*neighboursIt)->isOpposite(*lightweight))
                    allAvailableEdges.insert(**neighboursIt);
            }

            builder.addUndirectedEdge(lightweight->source->id, lightweight->destination->id, lightweight->weight);
        }

        allAvailableEdges.erase(lightweight);
    }

    return builder.getResult();
}

void removeLoops(set<Edge>& edges) {
    for(set<Edge>::iterator it = edges.begin(); it != edges.end();)
        if (it->source != it->destination)
            edges.erase(it++);
        else
            ++it;
}
void removeDoubles(set<Edge>& edges) {
    // TODO: add logic
    // ohh, there will be bug, i promise!
    // ffs, i hate iterator deletion
    for(set<Edge>::iterator it = edges.begin(); it != edges.end();) {
        int currentEdgeCount = 0;

        for (set<Edge>::iterator it2 = edges.begin(); it2 != edges.end(); it2++) {
            if ((it)->equals(*it2) && ++currentEdgeCount > 2) { // 2 because of unordered implementation
                edges.erase(it2++);
                it++;
            } else {
                ++it;
                ++it2;
            }
        }
    }
}
Graph* Kruskal(Graph* graph) {
    GraphBuilder builder(graph->getVertexAmount());
    vector<Vertex*> vertexes = graph->getAllVertexes();
    set<Edge> allEdges;

    // sort it by weight
    for(vector<Vertex*>::iterator vIt = vertexes.begin(); vIt != vertexes.end(); vIt++)
        for(list<Edge*>::iterator eIt = (*vIt)->neighborhood.begin(); eIt != (*vIt)->neighborhood.end(); eIt++)
            allEdges.insert(**eIt);

    removeLoops(allEdges);
    removeDoubles(allEdges);

    // Now add vertisies and edge between them from left to right of edge list if they add new vertices
    while(builder.getCurrentGraphVertexesAmount() != graph->getVertexAmount()) {
        Graph* constructedGraph = builder.getResult();
        set<Edge>::iterator min = allEdges.begin();

        if (!constructedGraph->haveCycle(*min)) {
            Graph* currentGraph = builder.getResult();

            if (currentGraph->getVertex(min->source->id) == nullptr)
                builder.addVertex(min->source->id);
            if (currentGraph->getVertex(min->destination->id) == nullptr)
                builder.addVertex(min->destination->id);

            builder.addUndirectedEdge(min->source->id, min->destination->id, min->weight);
        }

        // TODO: add proper logic
        allEdges.erase(min);
    }

    return builder.getResult();
}
