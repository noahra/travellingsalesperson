#include <iostream>
#include <vector>
#include <tuple>
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

    std::vector<int> tour;

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

    double calculateDistanceInPath(std::vector<int> tour)
    {
        double totalDistance = 0;
        for (int i = 1; i < tour.size(); i++)
            totalDistance += m_matrix[tour[i - 1]][tour[i]];

        return totalDistance;
    }

    std::vector<int> twoOptSwap(int start, int end)
    {
        std::vector<int> newTour(tour.size());
        std::copy(tour.begin(), tour.end(), newTour.begin());
        std::reverse(newTour.begin() + start, newTour.begin() + end);
        return newTour;
    }

    void twoOpt(std::chrono::steady_clock::time_point startTime)
    {
        int swaps = 1;
        double bestDist = calculateDistanceInPath(tour);
        while (swaps != 0)
        {
            swaps = 0;
            for (int i = 0; i < m_points.size(); ++i)
            {
                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
                if (ms > 1000)
                    return;
                for (int k = i + 1; k <= m_points.size(); ++k)
                {
                    auto newTour = twoOptSwap(i, k);
                    double newDist = calculateDistanceInPath(newTour);
                    if (newDist < bestDist)
                    {
                        bestDist = newDist;
                        tour = newTour;
                        ++swaps;
                    }
                }
            }
        }
    }

    double reverseSegmentIfBetter(int i, int j, int k){

        std::pair<double,double> A, B, C, D ,E, F;
        A = m_points[tour[i-1]];
        B = m_points[tour[i]];
        C = m_points[tour[j-1]];
        D = m_points[tour[j]];
        E = m_points[tour[k-1]];
        F = m_points[tour[k % (tour.size())]];
        double d0 = distanceCalculate(A,B) + distanceCalculate(C,D) + distanceCalculate(E,F);
        double d1 = distanceCalculate(A,C) + distanceCalculate(B,D) + distanceCalculate(E,F);
        double d2 = distanceCalculate(A,B) + distanceCalculate(C,E) + distanceCalculate(D,F);
        double d3 = distanceCalculate(A,D) + distanceCalculate(E,B) + distanceCalculate(C,F);
        double d4 = distanceCalculate(F,B) + distanceCalculate(C,D) + distanceCalculate(E,A);

        if (d0 > d1){
            std::reverse(tour.begin()+ i,tour.begin() + j);
            return -d0 + d1;
        }
        else if(d0 > d2){
            std::reverse(tour.begin()+ j,tour.begin() + k);
            return -d0 + d2;
        }
        else if(d0 > d4){
            std::reverse(tour.begin()+ i,tour.begin() + k);
            return -d0 + d4;
        }
        else if(d0 > d3){
            std::vector<int> tmp((k-j) + (j-i));
            std::copy(tour.begin() + j, tour.begin() + k, tmp.begin());
            std::copy(tour.begin() + i ,tour.begin() + j,tmp.begin() + (k-j-1));
            std::copy(tmp.begin(),tmp.end(),tour.begin() + i);
            return -d0 + d3;
        }
        return 0;
    }

    void threeOpt(std::chrono::steady_clock::time_point startTime){
        int n = tour.size();
        while(true){
            double delta = 0;
            for(int i = 1; i < n; ++i){
                for(int j = i + 1; j < n ; ++j){
                    for(int k = j + 2; k < n + i; ++k){
                        if(k<tour.size() && j< tour.size())

                            delta += reverseSegmentIfBetter(i,j,k);
                        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
                        if (ms > 1900)
                            return;
                    }
                }
            }
            if (delta >=0)
                break;
        }
    }

    void anneal()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, m_points.size() - 1);
        for (int i = 0; i < 10; ++i)
        {
            int a = distrib(gen);
            int b = distrib(gen);
            a > b ? twoOptSwap(b, a) : twoOptSwap(a, b);
        }
    }

private:
    double m_matrix[1000][1000];
    std::vector<std::pair<double, double>> m_points;
};


int main() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    auto input = readInput();
    Tsp* myTsp = new Tsp(input);

    myTsp->nearestNeighbour();

    /*
     for (int i = 0; i < input.size(); i++) {
        std::cout << myTsp->tour[i] << std::endl;
    }
    */

    //myTsp->twoOpt(startTime);
    //myTsp->anneal();

    //myTsp->twoOpt(startTime);
    myTsp -> threeOpt(startTime);
    for (int i = 0; i < input.size(); i++) {
        std::cout << myTsp->tour[i] << std::endl;
    }

    //std::cout << myTsp->calculateDistanceInPath(myTsp->tour);

    return 0;
}