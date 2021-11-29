#include <iostream>
#include <vector>
#include <numeric>      // std::iota
#include <algorithm>
#include <math.h>
#include <chrono>
#include <random>

int mod(int k, int n) {
    return ((k %= n) < 0) ? k + n : k;
}

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
        min_link = 0;
        max_link = 0;
        //Vector used to store and sort neighbours for each node in TSP AKA neighbor "Matrix"
        std::vector<double> test(tour.size());
        test.clear();

        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points.size(); j++) {
                m_matrix[i][j] = distanceCalculate(points[i], points[j]);
                test.push_back(m_matrix[i][j]);
                if (m_matrix[i][j] < min_link)
                    min_link = m_matrix[i][j];
                if (m_matrix[i][j] > max_link)
                    max_link = m_matrix[i][j];
            }
            m_closest_neighbour[i] = sort_indexes(test);
            test.clear();
        }
    }

    //creates a vector of indicies of nodes in m_points that are closest to a node. Used in TSP  constructor.
    static std::vector<int> sort_indexes(const std::vector<double>& v) {
        std::vector<int> idx(v.size());
        std::iota(idx.begin(), idx.end(), 0);
        std::stable_sort(idx.begin(), idx.end(), [v](double i1, double i2) {
            return v[i1] < v[i2];
            });
        return idx;
    }

    void fast_two_opt(std::chrono::steady_clock::time_point startTime) {
        int swaps = 1;
        double bestDist = calculateDistanceInPath(tour);
        int n = tour.size();

        while (swaps != 0)
        {
            swaps = 0;
            for (int n_1 = 0; n_1 < n; ++n_1) {
                int m_1 = mod(n_1 - 1 + n, n);
                for (int j_2 = 0; j_2 < n - 1; ++j_2) {
                    int c_2 = m_closest_neighbour[n_1][j_2];
                    int n_2 = m_cities[c_2];
                    int m_2 = mod(n_2 - 1 - n, n);

                    if (n_1 == n_2 || m_1 == m_2)
                        break;

                    if (m_matrix[tour[n_1]][tour[n_2]] + min_link > m_matrix[tour[m_1]][tour[n_1]] + max_link) 
                        break;

                    if (m_matrix[tour[n_1]][tour[n_2]] + m_matrix[tour[m_1]][tour[m_2]] < m_matrix[tour[m_1]][tour[n_1]] + m_matrix[tour[m_2]][tour[n_2]]) {
                        tour = n_1 < m_2 ? twoOptSwap(n_1, m_2) : twoOptSwap(m_2, n_1);
                        break;
                    }

                    /*auto newTour = n_1 < m_2 ? twoOptSwap(n_1, m_2) : twoOptSwap(m_2, n_1);
                    double newDist = calculateDistanceInPath(newTour);
                    if (newDist < bestDist) {
                        bestDist = newDist;
                        ++swaps;
                        tour = newTour;
                        break;
                    }*/

                    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
                    if (ms > 1950)
                        return;
                }
            }
            //std::cout << swaps << std::endl;
        }
    }

    std::vector<int> tour;

    void nearestNeighbour() {
        bool used[1000] = { false };
        tour.push_back(0);
        m_cities[0] = 0;
        used[0] = true;
        double bestDistance;
        max_link = -1.0;

        for (int i = 1; i < m_points.size(); i++) {
            int best = -1;
            bestDistance = 100000000.0;
            for (int j = 0; j < m_points.size(); j++) {
                if (!used[j] && m_matrix[tour[i - 1]][j] < bestDistance) {
                    best = j;
                    bestDistance = m_matrix[tour[i - 1]][j];
                }
            }

            if (bestDistance > max_link)
                max_link = bestDistance;

            tour.push_back(best);
            m_cities[best] = tour.size() - 1;
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
        int s = start;
        int e = end;
        std::vector<int> newTour(tour.size());
        std::copy(tour.begin(), tour.end(), newTour.begin());
        std::reverse(newTour.begin() + start, newTour.begin() + end);

        while (s < e) {
            int temp = m_cities[s];
            m_cities[s] = m_cities[e];
            m_cities[e] = temp;
            ++s;
            --e;
        }

        return newTour;
    }


private:
    double max_link;
    double min_link;
    double m_matrix[1000][1000];
    std::vector<int> m_closest_neighbour[1000];
    int m_cities[1000];
    std::vector<std::pair<double, double>> m_points;
};


int main() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    auto input = readInput();
    Tsp* myTsp = new Tsp(input);

    myTsp->nearestNeighbour();
    myTsp->fast_two_opt(startTime);
    for (int i = 0; i < input.size(); i++) {
        std::cout << myTsp->tour[i] << std::endl;
    }

    std::cout << myTsp->calculateDistanceInPath(myTsp->tour);

    return 0;
}
