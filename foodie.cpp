//
//  main.cpp
//  Final_Project_ECE479
//
//  Created by Shambhavi Singh on 5/6/23.
//

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stack>
#include <set>
#include <cfloat>

#define ROW 36
#define COL 32


using namespace std;
class Node {
    //One square in the map
public:
    int row;
    int col;
    int heuristic;
    bool street;
    string type;
    // Default constructor
    Node()
    {
        row = 0;
        col = 0;
        heuristic = 0;
        street = true;
        type = "";
    }
    // Parameterised Constructor
    Node(int row, int col){
        this->row = row;
        this->col = col;
        this->heuristic = 0;
        this->street = true;
        this->type = "";
    }
    // Parameterised Constructor
    Node(int heuristic, bool street, string type)
    {
        this->heuristic = heuristic;
        this->street = street;
        this->type = type;
    }
};

class nNode {
    //Fpr stack to trace path
public:
    Node* node;
    double f;
    nNode()
    {
        node = new Node();
        f = 0.0;
    }
    nNode(double f, Node* node){
        this->f = f;
        this->node = node;
    }
};

struct cell {
    //resemblance to a tree
    int parent_i, parent_j;
    double f, g, h;
};

int calculateHeuristic(int row, int col, Node* goal){
    //Calculates heuristic wiith manhattan distance
    return (abs((goal -> row) - row) + abs((goal -> col) - col));
}

bool isValid(int row, int col)
{
    // Returns true if the cell is in the grid else false
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isUnBlocked(vector<vector<Node*>> map, int row, int col)
{
    // Returns true if the cell is not blocked else false
    if (map[row][col] -> street == true && (map[row][col] -> type != "blocked"))
        return true;
    else
        return false;
}

bool isDestination(int row, int col, Node* goal)
{
    // Returns true if the cell is destination else false
    if (row == goal -> row && col == goal -> col)
        return true;
    else
        return false;
}

void printMap(vector<vector<Node*>> map, cell cellDetails[][COL], Node* goal){
    //Prints map on finding a solution
    int row = goal -> row;
    int col = goal -> col;
    vector<Node*> Path;
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push_back(new Node(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    Path.push_back(new Node(row, col));
    for(int i = 0; i < map.size(); ++i){
        for(int j = 0; j < map[i].size(); ++j){
            bool path = false;
            for(Node* n: Path){
                if(n->row == i && n->col == j && !(i == 17 && j == 17) && !(goal->row == i && goal->col == j)){
                    cout << "P ";
                    path = true;
                    break;
                }
            }
            if(path)
                continue;
            else if(i == 17 && j == 17){
                cout << "F ";
            }
            else if(goal->row == i && goal->col == j){
                cout << "G ";
            }
            else if(!(map[i][j] -> street)){
                cout << "  ";
            }
            else if(map[i][j] -> street){
                cout << "1 ";
            }
            
        }
        cout << "\n";
    }
    
}
void tracePath(cell cellDetails[][COL], Node* goal, bool print)
{
    if(print)
        cout<< "\nThe Path is ";
    int row = goal -> row;
    int col = goal -> col;
 
    stack<Node*> Path;
    
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(new Node(row, col));
        //cout << "\nrow: "<< row << " col: " << col << " COST: " << cellDetails[row][col].f;
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    Path.push(new Node(row, col));
    while (!Path.empty()) {
        Node* n = Path.top();
        Path.pop();
        if(print)
            printf("-> (%d,%d) ", n -> row, n -> col);
    }
    return;
}

 double aStarSearch(vector<vector<Node*>> map, Node* src, Node* goal, bool print)
{
    // If the source is out of range
    if (isValid(src->row, src->col) == false) {
        printf("Source is invalid\n");
        return 0.0;
    }
    if (isValid(goal->row, goal->col) == false) {
        printf("Destination is invalid\n");
        return 0.0;
    }
    if (isUnBlocked(map,src->row, src->col) == false || isUnBlocked(map, goal->row, goal->col)== false) {
        printf("Source or the destination is blocked\n");
        return 0.0;
    }
    if (isDestination(src->row, src->col, goal) == true) {
        printf("We are already at the destination\n");
        return 0.0;
    }
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
    cell cellDetails[ROW][COL];
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
    i = src->row;
    j = src->col;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
    set<nNode*> openList;
    openList.insert(new nNode(0.0, new Node(i, j)));
    bool foundDest = false;
    while (!openList.empty()) {
        nNode* n = *openList.begin();
        openList.erase(openList.begin());
        i = n -> node -> row;
        j = n -> node -> col;
        closedList[i][j] = true;
        
        double gNew, hNew, fNew;
        
        //North
        if (isValid(i - 1, j) == true) {
            if (isDestination(i - 1, j, goal) == true) {
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                tracePath(cellDetails, goal, print);
                if(print){
                    cout << "\nThe destination cell is found. Reached " << goal -> type << ".\n";
                    printMap(map, cellDetails, goal);
                }
                foundDest = true;
                return cellDetails[i][j].f;
            }
            else if (closedList[i - 1][j] == false && isUnBlocked(map, i - 1, j) == true){
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHeuristic(i - 1, j, goal);
                fNew = gNew + hNew;
                if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
                    openList.insert(new nNode(fNew, new Node(i - 1, j)));
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }
        //South
        if (isValid(i + 1, j) == true) {
            if (isDestination(i + 1, j, goal) == true) {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                tracePath(cellDetails, goal, print);
                if(print){
                    cout << "\nThe destination cell is found. Reached " << goal -> type << ".\n";
                    printMap(map, cellDetails, goal);
                }
                foundDest = true;
                return cellDetails[i][j].f;
            }
            else if (closedList[i + 1][j] == false && isUnBlocked(map, i + 1, j) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHeuristic(i + 1, j, goal);
                fNew = gNew + hNew;
                if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew) {
                    openList.insert(new nNode(fNew, new Node(i + 1, j)));
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }
        //East
        if (isValid(i, j + 1) == true) {
            if (isDestination(i, j + 1, goal) == true) {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                tracePath(cellDetails, goal, print);
                if(print){
                    cout << "\nThe destination cell is found. Reached " << goal -> type << ".\n";
                    printMap(map, cellDetails, goal);
                }
                foundDest = true;
                return cellDetails[i][j].f;
            }
            else if (closedList[i][j + 1] == false && isUnBlocked(map, i, j + 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHeuristic(i, j + 1, goal);
                fNew = gNew + hNew;
                if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew) {
                    openList.insert(new nNode(fNew, new Node(i, j + 1)));
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }
        //West
        if (isValid(i, j - 1) == true) {
            if (isDestination(i, j - 1, goal) == true) {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                tracePath(cellDetails, goal, print);
                if(print){
                    cout << "\nThe destination cell is found. Reached " << goal -> type << ".\n";
                    printMap(map, cellDetails, goal);
                }
                foundDest = true;
                return cellDetails[i][j].f;
            }
            else if (closedList[i][j - 1] == false && isUnBlocked(map, i, j - 1) == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHeuristic(i, j - 1, goal);
                fNew = gNew + hNew;
                if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew) {
                    openList.insert(new nNode(fNew, new Node(i, j - 1)));
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
    }
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");
    return 0.0;
}
    


int main(int argc, const char * argv[]) {
    // initializing the map
    vector<vector<Node*>> map
    {
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, true, "Goal Honors"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, true, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "bumpy"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, true, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, true, "bumpy"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "bumpy"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, true, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, true, "bumpy"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "bumpy"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "Goal Babcock"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, true, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "Goal ManziMo"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "Goal Pima"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "Goal Coconino"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "Goal Gila"), new Node(8, true, "NA"), new Node(9, true, "Goal Maricopa"), new Node(10, true, "NA"), new Node(11, true, "Goal Yuma"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, true, "FW"), new Node(18, false, "NA"), new Node(19, false, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "bumpy"), new Node(1, true, "bumpy"), new Node(2, true, "bumpy"), new Node(3, true, "bumpy"), new Node(4, true, "bumpy"), new Node(5, true, "bumpy"), new Node(6, true, "bumpy"), new Node(7, true, "bumpy"), new Node(8, true, "bumpy"), new Node(9, true, "bumpy"), new Node(10, true, "bumpy"), new Node(11, true, "bumpy"), new Node(12, true, "bumpy"), new Node(13, true, "bumpy"), new Node(14, true, "bumpy"), new Node(15, true, "bumpy"), new Node(16, true, "bumpy"), new Node(17, true, "bumpy"), new Node(18, true, "bumpy"), new Node(19, true, "bumpy"), new Node(20, true, "bumpy"), new Node(21, true, "bumpy"), new Node(22, true, "bumpy"), new Node(23, true, "bumpy"), new Node(24, true, "bumpy"), new Node(25, true, "bumpy"), new Node(26, true, "bumpy"), new Node(27, true, "bumpy"), new Node(28, true, "bumpy"), new Node(29, true, "bumpy"), new Node(30, true, "bumpy"), new Node(31, true, "bumpy")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "Goal Cochise"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "Goal Yavapai"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "NA"), new Node(23, true, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "Goal Kaibab"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "Goal Graham"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "Goal Aldea"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Pueblo"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Colonia"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, true, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Posada"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, true, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "Goal Coronado"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, false, "NA"), new Node(17, true, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Ville"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Santa"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, false, "NA"), new Node(5, true, "Goal Sonora"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "NA"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, false, "NA"), new Node(2, false, "NA"), new Node(3, true, "Goal Arbol"), new Node(4, false, "NA"), new Node(5, true, "NA"), new Node(6, false, "NA"), new Node(7, false, "NA"), new Node(8, false, "NA"), new Node(9, false, "NA"), new Node(10, false, "NA"), new Node(11, false, "NA"), new Node(12, false, "NA"), new Node(13, false, "NA"), new Node(14, false, "NA"), new Node(15, false, "NA"), new Node(16, true, "NA"), new Node(17, false, "NA"), new Node(18, false, "NA"), new Node(19, true, "Goal Likins"), new Node(20, false, "NA"), new Node(21, false, "NA"), new Node(22, false, "NA"), new Node(23, false, "NA"), new Node(24, false, "NA"), new Node(25, false, "NA"), new Node(26, false, "NA"), new Node(27, false, "NA"), new Node(28, false, "NA"), new Node(29, false, "NA"), new Node(30, false, "NA"), new Node(31, true, "NA")},
        {new Node(0, true, "NA"), new Node(1, true, "NA"), new Node(2, true, "NA"), new Node(3, true, "NA"), new Node(4, true, "NA"), new Node(5, true, "NA"), new Node(6, true, "NA"), new Node(7, true, "NA"), new Node(8, true, "NA"), new Node(9, true, "NA"), new Node(10, true, "NA"), new Node(11, true, "NA"), new Node(12, true, "NA"), new Node(13, true, "NA"), new Node(14, true, "NA"), new Node(15, true, "NA"), new Node(16, true, "NA"), new Node(17, true, "NA"), new Node(18, true, "NA"), new Node(19, true, "NA"), new Node(20, true, "NA"), new Node(21, true, "NA"), new Node(22, true, "Goal Pinal"), new Node(23, true, "Goal Navajo"), new Node(24, true, "NA"), new Node(25, true, "NA"), new Node(26, true, "NA"), new Node(27, true, "NA"), new Node(28, true, "NA"), new Node(29, true, "NA"), new Node(30, true, "NA"), new Node(31, true, "NA")}
    };
    
    for(int i = 0; i < map.size(); ++i){
        for(int j = 0; j < map[i].size(); ++j){
            map[i][j] -> row = i;
            map[i][j] -> col = j;
        }
    }
    
    cout<<"***************************** MAP *****************************\n";
    for(int i = 0; i < map.size(); ++i){
        for(int j = 0; j < map[i].size(); ++j){
            if(map[i][j] -> type == "FW"){
                cout << "F ";
            }
            else if(!(map[i][j] -> street)){
                cout << "  ";
            }
            else cout << map[i][j] -> street << " ";
        }
        cout << "\n";
    }
    //aStarSearch(map, map[17][17], map[1][9], true);
    int numRobot, mobRobot, numDelivery, num, totMobalized;
    totMobalized = 0;
    vector<Node*> numDorm;
    char ch = 'y';
    cout << "\n**************************** FOODIE ****************************\n";
    cout << "Enter the number of food robots(less than or equal to 15): ";
    cin >> numRobot;
    while(numRobot > 15){
        cout << "\nEnter valid number of robots (out of 15): ";
        cin >> numRobot;
    }
    while(ch == 'y' || ch == 'Y'){
        cout << "\nHow many food robots do you want to mobolize: ";
        cin >> mobRobot;
        while(mobRobot + totMobalized > numRobot){
            cout << "\nEnter valid number of robots you want to mobolize (out of " << numRobot - totMobalized << "): ";
            cin >> mobRobot;
        }
        totMobalized += mobRobot;
        cout << "\nDormitory:\n1. Honors Village\n2. Babcock\n3. Manzanita Mohave\n4. Coconino\n5. Gila\n6. Maricopa\n7. Yuma\n8. Pima\n9. Cochise\n10. Yavapai\n11. Kaibab Huachuca\n12. Coronado\n13. Arizona Sonora\n14. Arbol de la Vida\n15. Peublo de la Cienega\n16. Posada San Pedro\n17. Villa del Puente\n18. Graham Greenlee\n19. Colonia de la Paz\n20. Apache Santa Cruz\n21. Likins\n22. Navajo\n23. Pinal\n";
        
        for(int i = 0; i < mobRobot; ++i){
            cout << "ROBOT " << (i+1) << ":\n";
            cout << "Enter number of deliveries (always less than or equal to 2): ";
            cin >> numDelivery;
            while(numDelivery > 2){
                cout << "\nEnter valid number of deliveries (0 to 2): ";
                cin >> numDelivery;
            }
            for(int j = 0; j < numDelivery; ++j){
                cout << "Enter the dorm number for delivery: ";
                cin >> num;
                if(num >= 1 && num <= 23){
                    if(num == 1){
                        numDorm.push_back(map[1][9]);
                    }
                    else if(num == 2){
                        numDorm.push_back(map[6][28]);
                    }
                    else if(num == 3){
                        numDorm.push_back(map[12][5]);
                    }
                    else if(num == 4){
                        numDorm.push_back(map[14][8]);
                    }
                    else if(num == 5){
                        numDorm.push_back(map[16][7]);
                    }
                    else if(num == 6){
                        numDorm.push_back(map[16][9]);
                    }
                    else if(num == 7){
                        numDorm.push_back(map[16][11]);
                    }
                    else if(num == 8){
                        numDorm.push_back(map[12][18]);
                    }
                    else if(num == 9){
                        numDorm.push_back(map[22][7]);
                    }
                    else if(num == 10){
                        numDorm.push_back(map[22][13]);
                    }
                    else if(num == 11){
                        numDorm.push_back(map[26][5]);
                    }
                    else if(num == 12){
                        numDorm.push_back(map[31][3]);
                    }
                    else if(num == 13){
                        numDorm.push_back(map[33][5]);
                    }
                    else if(num == 14){
                        numDorm.push_back(map[34][3]);
                    }
                    else if(num == 15){
                        numDorm.push_back(map[27][19]);
                    }
                    else if(num == 16){
                        numDorm.push_back(map[29][19]);
                    }
                    else if(num == 17){
                        numDorm.push_back(map[31][19]);
                    }
                    else if(num == 18){
                        numDorm.push_back(map[26][19]);
                    }
                    else if(num == 19){
                        numDorm.push_back(map[28][19]);
                    }
                    else if(num == 20){
                        numDorm.push_back(map[32][19]);
                    }
                    else if(num == 21){
                        numDorm.push_back(map[34][19]);
                    }
                    else if(num == 22){
                        numDorm.push_back(map[35][23]);
                    }
                    else if(num == 23){
                        numDorm.push_back(map[35][22]);
                    }
                }
                
            }
            if(numDorm.size() == 1){
                aStarSearch(map, map[17][17], numDorm[0], true);
                numDorm.pop_back();
            }
            else if(numDorm.size() == 2){
                if(aStarSearch(map, map[17][17], numDorm[0], false) + aStarSearch(map, numDorm[0], numDorm[1], false) + aStarSearch(map, numDorm[1], map[17][17], false) <= aStarSearch(map,map[17][17], numDorm[1], false) + aStarSearch(map, numDorm[1], numDorm[0], false) + aStarSearch(map, numDorm[0], map[17][17], false)){
                    aStarSearch(map, map[17][17], numDorm[0], true);
                    aStarSearch(map, numDorm[0], numDorm[1], true);
                    aStarSearch(map, numDorm[1], map[17][17], true);
                }
                else{
                    aStarSearch(map, map[17][17], numDorm[1], true);
                    aStarSearch(map, numDorm[1], numDorm[0], true);
                    aStarSearch(map, numDorm[0], map[17][17], true);
                }
                numDorm.pop_back();
                numDorm.pop_back();
            }
            else if(numDorm.size() == 0){
                cout<<"\nThank you for using FOODIE";
                ch = 'n';
            }
        }
        cout <<"\nDo you want to continue sending deliveries(enter y or n): ";
        cin >> ch;
        if(totMobalized >= numRobot && ch == 'y'){
            cout<<"\nAll robots are in the process of delivering right now. Please wait.";
            cout<<"\nThank you for using FOODIE\n";
            ch = 'n';
            break;
        }
        if(ch != 'y'){
            cout<<"\nThank you for using FOODIE\n";
        }
    }
    
    return 0;
}
