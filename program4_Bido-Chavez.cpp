/*
Name       : Michael Bido-Chavez (euid: mb0501)
Due Date   : December 2, 2016
Class      : CSCE 3110
Instructor : Prof. Fahmida Hamid

Program 04
----------------
Description:
  This program implements the Dijkstra's Algorithm on a directed graph that
  has positive edge costs. Vertices of the graph are designated by integers
  1 to N, and integer weights. The graph itself is read in from a file that
  is in a predetermined format. This program also prints all the possible
  paths within that graph (no cycles).

  To run, see readme
----------------
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

const int INF = 2000000000; // infinity for an int

using namespace std;
// class vertex
class Vertex {
  private:
    int me;
    vector<int>* adj; // Adjacency list
    vector<int>* w;   // Edge weights
    int dist;         // Distance from source
    int path;         // Predecessor on path from source
  public:
    Vertex();
    vector<int>* getAdj() {return adj; }
    vector<int>* getWeight() {return w; }
    void setPath(int p) {path = p; }
    //~Vertex();
};

// function prototypes
void printMenu(int vertNum, string fileName);
void printAllPaths(int vertNum, Vertex* graph);
void allVertexDFS(int vertNum, Vertex* graph, int start, int end);
int oneVertexDFS(int vNum, Vertex* graph, int curr, int end, int stackPosition,
                bool visited[], int pathStack[]);
void greedyAlg(int vertNum, Vertex* graph, int start, int goal);

int main(int argc, char *argv[])
{
  // file variables
  string fileName = "graph2.dat";
  ifstream inFile(fileName.c_str());
  string fileLine;

  Vertex* graph;

  int vertNum = 0;
  int vertex1, vertex2, weight;
  // bool badFile = false;

  // populate the graph
  if (inFile.good())
  { // loop until eof
    while(getline(inFile,fileLine)) {
      // cout << fileLine << endl;
      // initialize total verticies number
      if (vertNum == 0) {
        istringstream iss(fileLine);
        // error check for incorrect file format
        if (!(iss >> vertNum)) {
          cout << "File syntax is incorrect for input – fix and try again" << endl;
          // exit(1);
          return 0;
          break;
        }
        graph = new Vertex[vertNum];
        // cout << "x: " << vertNum << endl;
      }
      else {
        // to populate the graph with edges
        istringstream iss(fileLine);
        // error check for file format
        if(!(iss >> vertex1 >> vertex2 >> weight)) {
          cout << "File syntax is incorrect for input – fix and try again" << endl;
          // exit(2);
          return 0;
          break;
        }
        // populate the graph w/ weights as well
        // for each graph vertex, add to adj list
        graph[(vertex1-1)].getAdj()->push_back(vertex2);
        graph[(vertex1-1)].getWeight()->push_back(weight);
        // uncomment the following two lines for an undirected graph
        // graph[(vertex2-1)].getAdj()->push_back(vertex1);
        // graph[(vertex2-1)].getWeight()->push_back(weight);

        // cout << "y: " << vertex1 << " " << vertex2 << " " << weight << endl;
      }
    }
  }

  inFile.close();

  // edit out the comment below to test adj list and weights for the graph
  /*
  for (int i = 0; i < vertNum; i++) {

    vector<int>* currentAdj = graph[i].getAdj();
    vector<int>* currentW = graph[i].getWeight();

    cout << "The adj list for " << (i + 1) << " is:";
    for (vector<int>::iterator it = currentAdj->begin(); it != currentAdj->end(); ++it) {
      cout << " " << *it;
    }
    cout << "\nThe weight list for " << (i + 1) << " is:";
    for (vector<int>::iterator it = currentW->begin(); it != currentW->end(); ++it) {
      cout << " " << *it;
    }
    cout << endl;
  }
  */
  // menu w/ switch
  // for interactive user to pick something
  int menuInput;
  int start;
  int objective;
  while(1) {
    printMenu(vertNum, fileName);
    cin >> menuInput;
    switch (menuInput) {
      case 1:
        // print all paths in a graph
        cout << "Printing all possible paths..." << endl;
        printAllPaths(vertNum, graph);
        break;
      case 2:
        // greedy Algorithm
        cout << "Enter the starting vertex (e.g. '2' for vertex 2): ";
        cin >> start;
        cout << "Enter the destination vertex (e.g. '1' for vertex 1): ";
        cin >> objective;
        cout << "Generating shortest path via Dijkstra's Algorithm..." << endl;
        greedyAlg(vertNum, graph, start, objective);
        break;
      case 3:
      // quit
        cout << "Thanks for using me. Goodbye!" << endl;
        return 0;
        // break;
      default:
        cout << "invalid input, try again" << endl;
    }
  }
  return 0;
}

Vertex::Vertex()
{
  adj = new vector<int>();
  w = new vector<int>();
  // cout << "Vertex made" << endl;
}
// print out menu
void printMenu(int vertNum, string fileName)
{
  cout << "------------------------ DATA -------------------------" << endl;
  cout << "# of vertices: " << vertNum << ", file name: " << fileName << endl;
  cout << "------------------------ MENU -------------------------" << endl;
  cout << "1. All possible paths " << endl;
  cout << "2. Shortest path from one vertex to another " << endl;
  cout << "3. Quit " << endl;;
  cout << "-------------------------------------------------------" << endl;

  return;
}
// for every node
void printAllPaths(int vertNum, Vertex* graph)
{
  int startVert;
  int endVert;
  for (int i = 0; i < vertNum; i++) {
    startVert = i+1;
    for (int j = 0; j < vertNum; j++) {
      if (i != j) {
        endVert = j+1;
        allVertexDFS(vertNum, graph, startVert, endVert);
      }
    }
  }
  return;
}
// sorry about the lack of comments, poor time management on my part
// turning it in before it's late
// :(
void allVertexDFS(int vertNum, Vertex* graph, int start, int end)
{
  bool* visited;
  visited = new bool[vertNum];
  for (int i = 0; i < vertNum; i++) {
    visited[i] = false;
  }

  int* pathStack;
  pathStack = new int[vertNum];

  int temp = oneVertexDFS(vertNum, graph, start, end, 0, visited, pathStack);

  return;
}

int oneVertexDFS(int vNum, Vertex* graph, int curr, int end, int stackPosition,
                bool visited[], int pathStack[])
{
  int stackPos = stackPosition;
  pathStack[stackPos] = curr;
  stackPos++;

  visited[curr-1] = true;

  vector<int>* v = graph[curr-1].getAdj();

  if ( curr != end ) {
    for (vector<int>::iterator it = v->begin(); it != v->end(); ++it) {
      if(visited[*it - 1] == false) {
        stackPos = oneVertexDFS(vNum, graph, *it, end, stackPos, visited, pathStack);
      }
    }
  }
  else {
    for (int i = 0; i < stackPos; i++) {
      cout << " " << pathStack[i];
    }
    cout << endl;
  }

  visited[curr-1] = false;

  return (stackPos - 1);
}

void greedyAlg(int vertNum, Vertex* graph, int start, int goal)
{
  bool unvisited[vertNum];
  int unvisitedSize = vertNum;

  int dist[vertNum];
  //int prev[vertNum];

  for (int i = 0; i < vertNum; i++) {
    if ( i == start - 1 ) {
      unvisited[i] =  false;
    } else {
      unvisited[i] =  true;
    }
    dist[i] = INF;
    // prev[i] = -1;
  }

  dist[start - 1] = 0;
  int u = start - 1;
  int altDist;
  int smallest;
  int uNew;
  //prev[0] = start;

  vector<int>* vAdj;
  vector<int>* vW;
  vector<int>::iterator it;
  vector<int>::iterator jt;
  // while loop of Dijkstra Algorithm
  while (unvisitedSize > 0) {
    unvisitedSize--;
    // altDist = INF;
    vAdj = graph[u].getAdj();
    vW = graph[u].getWeight();
    jt = vW->begin();

    for (it = vAdj->begin(); it != vAdj->end(); ++it) {
      if (unvisited[*it - 1]) {
        altDist = *jt + dist[u];
        if (altDist < dist[goal-1]) {
          dist[*it - 1] = altDist;
          graph[*it - 1].setPath(u + 1);
        }
      }
      ++jt;
    }
    smallest = dist[goal - 1];
    for (int i = 0; i < vertNum; i++) {
      if (unvisited[i]) {
        if (dist[i] < smallest) {
          dist[i] = smallest;
          uNew = i;
        }
      }
    }

    unvisited[uNew] = false;
    if (u + 1 == goal) {
      //cout << "\nhoorah!\n" << endl;
      return;
    }
    u = uNew;
    //cout << "looped";
  }

  return;
}

/*
----------- helpful sources -----------
DFS:
  https://en.wikipedia.org/wiki/Depth-first_search#Pseudocode
  http://www.cs.toronto.edu/~heap/270F02/node36.html
  https://www.cs.usfca.edu/~galles/visualization/DFS.html

Dijkstra:
  https://en.wikipedia.org/wiki/Dijkstra's_algorithm#Pseudocode
  http://math.mit.edu/~rothvoss/18.304.3PM/Presentations/1-Melissa.pdf
  https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html
---------------------------------------
*/
