#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <fstream>
#include <limits>

#include "graph.h"
#include "minqueue.h"

using namespace std;

//
// buildGraph:
//
// Inputs the graph vertices and edges from the given file, building
// the graph g.  File format:
//   vertex vertex vertex ... #
//   src dest weight ... #
//
void buildGraph(string filename, graph &g)
{
    ifstream file(filename);

    if (!file.good())
    {
        cout << endl;
        cout << "**Error: unable to open input file '" << filename << "'." << endl;
        cout << endl;
        return;
    }

    //
    // Input vertices as single uppercase letters:  A B C ... #
    //
    //
    // TODO: use >> operator, since we are not inputting
    // strings that might contain blanks:
    //
    char v;
    file >> v;
    while ('#' != v)
    {
        // cout << v << endl;
        g.addvertex(v);
        file >> v;
    }

    //
    // Now input edges:  Src Dest Weight ... #
    //

    //
    // TODO: use >> operator, since we are not inputting
    // strings that might contain blanks:
    //
    char from, to;
    int weight;
    file >> from;
    file >> to;
    file >> weight;
    while ('#' != from)
    {
        g.addedge(from, to, weight);
        file >> from;
        file >> to;
        file >> weight;
    }
}

//
// outputGraph:
//
// Outputs graph g to the console.
//
void outputGraph(graph &g)
{
    vector<char> vertices = g.vertices();

    cout << "**Vertices: ";

    for (char c : vertices)
    {
        cout << c << " ";
    }

    cout << endl;

    cout << "**Edges: ";

    for (char v : vertices)
    {
        vector<char> neighbors = g.neighbors(v);

        for (char n : neighbors)
        {
            int weight = g.getweight(v, n);
            cout << "(" << v << "," << n << "," << weight << ") ";
        }
    }

    cout << endl;
}

//
// BFS:
//
// Performs a breadth-first search from the given starting vertex.
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> BFS(graph &g, char startV)
{
    vector<char> visited;
    queue<char> frontierQueue;
    set<char> discoveredSet;
    char currentV;

    frontierQueue.push(startV);
    discoveredSet.insert(startV);
    while (!frontierQueue.empty())
    {
        currentV = frontierQueue.front();
        frontierQueue.pop();
        visited.push_back(currentV);
        for (char adjV : g.neighbors(currentV))
        {
            if (discoveredSet.find(adjV) == discoveredSet.end())
            {
                frontierQueue.push(adjV);
                discoveredSet.insert(adjV);
            }
        }
    }

    return visited;
}

//
// DFS:
//
// Performs a depth-first search from the given starting vertex.
// Returns the vertices in the order they were visited; startV will
// be first.
//
vector<char> DFS(graph &g, char startV)
{
    vector<char> visited;
    stack<char> frontierStack;
    set<char> visitedSet;
    char currentV;
    frontierStack.push(startV);
    visitedSet.insert(startV);
    while (!frontierStack.empty())
    {
        currentV = frontierStack.top();
        frontierStack.pop();
        visited.push_back(currentV);
        for (char adjV : g.neighbors(currentV))
        {
            if (visitedSet.find(adjV) == visitedSet.end())
            {
                frontierStack.push(adjV);
                visitedSet.insert(adjV);
            }
        }
    }

    return visited;
}

void printPriorityQueue(priority_queue <pair<char, int>> pq){

    while (!pq.empty()) 
    { 
        cout << "(" << pq.top().first << ", " << pq.top().second << ")"; 
        pq.pop(); 
    } 
    cout << '\n';
}

//
// Dijkstra:
//
// Performs a Dijkstra algorithm search from the given starting vertex.
// Returns the vertices they were visited in ascending order of distance
vector<char> Dijkstra(graph &g, char startV, char predV[26], int distance[26]){
    const int Infinity = numeric_limits<int>::max();
    minqueue<char, int> unvisitedQueue; //minqueue.h
    // minqueue<char, int> unvisitedQueue(100);  //minqueue_test.h
    vector<char> visited;
    
    char currentV;
    int edgeWeight;
    int alternativePathDistance;

    for (char e : g.vertices())
    {
        predV[e - 'A'] = -1;
        distance[e - 'A'] = Infinity;
        unvisitedQueue.pushinorder(e, Infinity);
    }

    distance[startV - 'A'] = 0;
    predV[startV - 'A'] = startV;
    unvisitedQueue.pushinorder(startV, 0);

    while(!unvisitedQueue.empty()){
        currentV = unvisitedQueue.minfront();
        unvisitedQueue.minpop();
        if(distance[currentV - 'A'] == Infinity){
            break;
        }
        visited.push_back(currentV);

        for(char adjV : g.neighbors(currentV)){
            edgeWeight = g.getweight(currentV, adjV);
            alternativePathDistance = distance[currentV - 'A'] + edgeWeight;

            if(alternativePathDistance < distance[adjV - 'A']){
                distance[adjV - 'A'] = alternativePathDistance;
                predV[adjV - 'A'] = currentV;
                unvisitedQueue.pushinorder(adjV, alternativePathDistance);
            }
        }
    }
    return visited;
}

void printVertices(string prefix, vector<char> &v){
    cout << prefix;
    for(char e : v){
        cout << e << " ";
    }
    cout << endl;
}

void rec_printPath(char startV, char predV[26]){
    if(startV == predV[startV - 'A']){
        cout << startV << " ";
        return;
    }
    rec_printPath(predV[startV - 'A'], predV);
    cout << startV << " ";
}

void printPath(vector<char> &visited, char predV[26], int distance[26]){
    for(char e : visited){
        cout << " " << e << ": " << distance[e - 'A'] << " via ";
        rec_printPath(e, predV);
        cout << endl;
    }

}

int main()
{
    graph g;
    string filename;

    cout << "Enter filename containing graph data> ";
    // cin >> filename;
    filename = "graph1.txt";

    //
    // Let's input the graph, and then output to see what we have:
    //
    buildGraph(filename, g);
    outputGraph(g);

    char startV;
    cout << endl;
    cout << "Enter a starting vertex or #> ";
    // cin >> startV;
    startV = 'A';

    while (startV != '#')
    {
        vector<char> visited;

        
        if (g.isvertex(startV))
        {
            // BFS:
            visited = BFS(g, startV);
            printVertices("BFS: ", visited);

            // DFS:
            visited = DFS(g, startV);
            printVertices("DFS: ", visited);

            // Dijkstra:
            char predV[26];
            int distance[26];
            visited = Dijkstra(g, startV, predV, distance);
            printVertices("Dijkstra: ", visited);
            //cout << "(" << predV[0] << ", " << distance[0] << ") ";
            printPath(visited, predV, distance);
        }
        else
        {
            cout << "Not a vertex in graph, ignored..." << endl;
        }

        cout << endl;
        cout << "Enter a starting vertex or #> ";
        cin >> startV;
    }

    //
    // done:
    //
    return 0;
}
