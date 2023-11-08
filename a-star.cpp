/*     Matias Moseley     C++     CS 450     12/4/2022     Tactical Pathing     */

#include<bits/stdc++.h>
using namespace std;

struct Skip{
    bool diagonal : 1;
} skip;

struct Info {
    pair<uint8_t, uint8_t> start;
    pair<uint8_t, uint8_t> end;
} info;

inline uint8_t determinCost(char ch, uint16_t i, uint16_t j){
    switch (ch) {
        case '.': return 1;
        case ',': return 2;
        case 'o': return 3;
        case '=': return 50;
        case '0': info.start.first = i; info.start.second = j; return 0;
        case '2': info.end.first = i; info.end.second = j; return 255;
        default: return 0;
    }
}

inline float calcHuer(pair<uint8_t, uint8_t> neighbor){
    uint8_t x = abs(neighbor.first - info.end.first);
    uint8_t y = abs(neighbor.second - info.end.second);
    return (x+y)+(-0.58579)*min(x, y);
}

inline float travelCost(uint8_t cost){
    return skip.diagonal ? cost * 1.5 : cost;
}

int main(){
    clock_t start = clock();

    map<pair<uint8_t, uint8_t>, pair<uint8_t, uint8_t>> path;
    map<pair<uint8_t, uint8_t>, float> totalCost;

    string line, ch;
    uint16_t i = 0, j;
    skip = {1};

    char charMap[256][256]; // Char and Cost map
    uint8_t costMap[256][256];

    while(getline(cin, line)){ // Grab each line
        stringstream ss(line);
        j = 0;
        while(getline(ss, ch, ' ')){ // Grab each char
            charMap[i][j] = ch[0];
            costMap[i][j] = determinCost(ch[0], i, j);
            j++;
        }
        i++;
    }

    priority_queue<pair<float, pair<uint8_t, uint8_t>>,
        vector<pair<float, pair<uint8_t, uint8_t>>>,
        greater<pair<float, pair<uint8_t, uint8_t>>>> openList; // priority queue / openList

    openList.emplace(0, info.start); // Initialize openList and path/cost tables
    path[info.start] = info.start;
    totalCost[info.start] = 0;

    pair<uint8_t, uint8_t> present, next;

    while(!openList.empty()){
        present = openList.top().second;
        openList.pop();

        if(present.first-1 > -1 && present.second-1 > -1 && costMap[present.first-1][present.second-1] != 0){ // Top Left
            next.first = present.first-1;
            next.second = present.second-1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 1; // Diagonal move

            float thisCost = totalCost[present] + travelCost(costMap[present.first-1][present.second-1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.first-1 > -1 && costMap[present.first-1][present.second] != 0){ // Top
            next.first = present.first-1;
            next.second = present.second;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 0; // Adjacent move

            float thisCost = totalCost[present] + travelCost(costMap[present.first-1][present.second]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.first-1 > -1 && present.second+1 <= j && costMap[present.first-1][present.second+1] != 0){ // Top Right
            next.first = present.first-1;
            next.second = present.second+1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 1; // Diagonal move

            float thisCost = totalCost[present] + travelCost(costMap[present.first-1][present.second+1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.second+1 <= j && costMap[present.first][present.second+1] != 0){ // Right
            next.first = present.first;
            next.second = present.second+1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 0; // Adjacent move

            float thisCost = totalCost[present] + travelCost(costMap[present.first][present.second+1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.second-1 > -1 && costMap[present.first][present.second-1] != 0){ // Left
            next.first = present.first;
            next.second = present.second-1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 0; // Adjacent move
           
            float thisCost = totalCost[present] + travelCost(costMap[present.first][present.second-1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.first+1 <= i && present.second+1 <= j && costMap[present.first+1][present.second+1] != 0){ // Bottom Right
            next.first = present.first+1;
            next.second = present.second+1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 1; // Diagonal move
           
            float thisCost = totalCost[present] + travelCost(costMap[present.first+1][present.second+1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.first+1 <= i && costMap[present.first+1][present.second] != 0){ // Bottom
            next.first = present.first+1;
            next.second = present.second;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 0; // Adjacent move
           
            float thisCost = totalCost[present] + travelCost(costMap[present.first+1][present.second]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
        if(present.first+1 <= i && present.second-1 > -1 && costMap[present.first+1][present.second-1] != 0){ // Bottom Left
            next.first = present.first+1;
            next.second = present.second-1;
            if(next == info.end){ // If next is end
                path[info.end] = present;
                totalCost[info.end] = totalCost[present];
                break;
            }
            skip.diagonal = 1; // Diagonal move
           
            float thisCost = totalCost[present] + travelCost(costMap[present.first+1][present.second-1]);
            if (totalCost.find(next) == totalCost.end() || thisCost < totalCost[next]) { // Cost not present or new cost is better
                totalCost[next] = thisCost;
                float priority = thisCost + calcHuer(next);
                openList.emplace(priority, next);
                path[next] = present;
            }
        }
    }

    present = path[info.end]; // Edit charMap
    while(present != info.start){
        charMap[present.first][present.second] = '*';
        present = path[present];
    }

    for(uint16_t row = 0; row < i; row++){ // Display charMap
        for(uint16_t col = 0; col < j; col++){
            cout << charMap[row][col] << " ";
        }
        cout << endl;
    }

    cout << "Total cost: " << totalCost[info.end] << endl; // Display total cost

    double time_taken_in_seconds = (double)( clock() - start ) / CLOCKS_PER_SEC; // Time in microseconds
    double time_taken_in_microseconds = time_taken_in_seconds * 1000000.0;
    cout << "Time taken in microseconds: " << time_taken_in_microseconds << endl;
}