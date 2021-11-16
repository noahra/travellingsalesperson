#include <iostream>
#include <vector>
#include <math.h>

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

    int tour[1000];

    void nearestNeighbour() {


        double totalDistance = 0;
        bool used[1000] = {false};

        tour[0] = 0;
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

            totalDistance = totalDistance + bestDistance;
            tour[i] = best;
            used[best] = true;

        }
    }

    double calculateDistanceInPath(int tours[]) {

        double totalDistance = 0;

        for (int i = 0; i < sizeof(tours); i++) {
            totalDistance = totalDistance + m_matrix[i][tours[i]];

        }

        return totalDistance;

    }

    int* twoOptSwap(int tours[], int edge1, int edge2) {


        int* newTour = new int[1000];

        for (int i = 0; i < edge1 - 1; i++) {
            newTour[i] = tours[i];
        }
        newTour[edge1] = tours[edge2];
        newTour[edge2] = tours[edge1];

        for (int i = edge2 + 1; i < sizeof(tours); i++) {
            newTour[i] = tours[i];
        }

        return newTour;


    }


    int* optimization(int tours[]) {
        double bestDistance = calculateDistanceInPath(tours);
        startAgain:
        int* newTourArray = new int[1000];
        for (int i = 0; i < sizeof(tours); i++) {
            for (int j = i + 1; i < sizeof(tours); i++) {



                int *a = twoOptSwap(tours, i, j);
                for (int i = 0; i<1000;i++) {
                    newTourArray[i] = *a;
                    a++;
                }
                double newDistance = calculateDistanceInPath(newTourArray);
                if (newDistance < bestDistance) {
                    bestDistance = newDistance;
                    goto startAgain;

                }


            }
        }
        return newTourArray;
    }


private:
    double m_matrix[1000][1000];
    std::vector<std::pair<double, double>> m_points;


};


int main() {


    auto input = readInput();
    Tsp *myTsp = new Tsp(input);
    myTsp->nearestNeighbour();
    int* bestDistanceVector = myTsp->optimization(myTsp->tour);
    for (int i = 0; i < input.size(); i++) {
        std::cout << bestDistanceVector[i] << std::endl;
    }


    return 0;


}
