#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <random>

std::vector<std::pair<double, double> > readInput() {
    int i = 0, count = 0;
    std::vector<std::pair<double, double> > vertex;
    std::cin >> count;
    while (i++ < count) {
        double x, y;
        std::cin >> x >> y;
        vertex.push_back(std::pair<double, double>(x, y));
    }
    return vertex;
}

double distanceCalculate(std::pair<double, double> p1, std::pair<double, double> p2) {

    double x = p1.first - p2.first; //calculating number to square in next step
    double y = p1.second - p2.second;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);

    return dist;
}

class Tsp {       // The class
public:
    Tsp(std::vector<std::pair<double, double>> points) {// Access specifier
        m_points = points;
        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points.size(); j++) {
                m_matrix[i][j] = distanceCalculate(points[i], points[j]);
            }
        }
    }

    void nearestNeighbour() {
        bool used[1000] = { false };

        tour.push_back(0);
        used[0] = true;


        for (int i = 1; i < m_points.size(); i++) {
            int best = -1;
            double bestDistance = 100000000;
            for (int j = 0; j < m_points.size(); j++) {
                if (!used[j] && m_matrix[tour[i - 1]][j] < bestDistance) {
                    best = j;
                    bestDistance = m_matrix[tour[i - 1]][j];
                }
            }
            tour.push_back(best);
            used[best] = true;
        }
    }

    void reverse(int start, int end)
    {
        while (end - start > 0)
        {
            int temp = tour[start];
            tour[start] = tour[end];
            tour[end] = temp;
            start++;
            end--;
        }
    }

    /*
     - v   y -             - v - y -
         ×         ==>
     - w   x -             - w - x -
    */

    bool twoOpt(std::chrono::steady_clock::time_point startTime)
    {
        double minimumChange;
        int n = m_points.size();
        int nodeToBeChanged1, nodeToBeChanged2;

        do
        {
            minimumChange = 0.0;
            nodeToBeChanged1 = -1; nodeToBeChanged2 = -1;
            for (int i = 0; i < n - 2; ++i)
            {
                for (int j = i + 2; j < n; ++j)
                {
                    int v = tour[i];
                    int w = tour[j];
                    int x = tour[(i + 1)];
                    int y = tour[(j + 1) % n];
                    double change = m_matrix[v][w] + m_matrix[x][y] - m_matrix[v][x] - m_matrix[w][y];
                    if (minimumChange > change)
                    {
                        minimumChange = change;
                        nodeToBeChanged1 = i + 1; nodeToBeChanged2 = j;
                    }
                }
            }

            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
            if (ms > 1950)
                return false;

            if (nodeToBeChanged1 != -1 && nodeToBeChanged2 != -1)
            {
                reverse(nodeToBeChanged1, nodeToBeChanged2);
            }

        } while (minimumChange < 0);

        return true;
    }

    std::vector<int> tour;
private:
    double m_matrix[1000][1000];
    std::vector<std::pair<double, double>> m_points;
};


int main() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    auto input = readInput();
    Tsp* myTsp = new Tsp(input);

    myTsp->nearestNeighbour();

    while (myTsp->twoOpt(startTime))
    {
    }
    

    for (int i = 0; i < input.size(); i++) {
        std::cout << myTsp->tour[i] << std::endl;
    }

    return 0;
}
