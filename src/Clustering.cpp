/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file Clustering.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 *
 * Created on 22 de octubre de 2025, 11:57
 */
#include <iostream>
#include <string>
#include <cstdlib>
#include "Clustering.h"
#include "Location.h"
#include "VectorInt.h"
#include "VectorLocation.h"

Clustering::Clustering()
{
    _K = 0;
    _isDone = false;
    _sumWCV = 0.0;
    _numIterations = 0;
    _seed = DEFAULT_RANDOM_SEED;
}
/**
 * @brief Gets the number of clusters
 * Query method
 * @return The number of clusters
 */

int Clustering::getK() const
{
    return _K;
}

/**
 * @brief Gets the current vector of centroids
 * Query method
 * @return A const reference to the vector of centroids (a VectorLocation
 * object).
 */
const VectorLocation& Clustering::getCentroids() const
{
    return _centroids;
}

/**
 * @brief Indicates whether the clustering algorithm (run() method) has
 * already been executed for this Clustering object.
 * Query method
 * @return true if the run() method has already been executed for this
 * Clustering object; false otherwise
 */
bool Clustering::isDone() const
{
    return _isDone;
}
/**
 * @brief Gets the number of locations in the set of locations of this
 * Clustering object.
 * Query method
 * @return The number of locations in the set of locations of this
 * Clustering
 */
int Clustering::getNumLocations() const
{
    return _locations.getSize();
}

/**
 * @brief Gets the cluster number for the Location determined by the
 * provided integer (@p locationIndex)
 * Query method
 * @param locationIndex An integer that determines the position of a
 * location in the vector of Location objects. Input parameter
 * @return The cluster number for the Location determined by
 * the provided integer (@p locationIndex). If the
 * clustering algorithm has not been run (_isDone is false), then it returns
 * -1.
 */
int Clustering::clusterOf(int locationIndex) const
{
    if (!_isDone)
    {
        return -1;
    }
    if (locationIndex < 0 || locationIndex >= _locations.getSize())
    {
        return -1;
    }
    return _clusters.at(locationIndex);
}

/**
 * @brief Gets the value of the sum of the within-cluster variances of this
 * Clustering. This value is obtained from the field _sumWCV
 * @note The returned value will likely be a garbage value if the run()
 * method has not yet been executed (_isDone is false)
 * Query method
 * @return A double with the sum of within-cluster variances of this
 * Clustering
 */
double Clustering::getSumWCV() const
{
    return _sumWCV;
}

/**
 * @brief  Gets the number of iterations used in the KMeans
 * algorithm until no cluster assignment change occurs.
 * It obtains the number of iterations (field _numberIterations) used
 * in the KMeans algorithm until no cluster change occurred.
 * @note The returned value will likely be a garbage value if the run()
 * method has not yet been executed (_isDone is false)
 * Query method
 * @return The number of iterations used in the KMeans algorithm
 */
int Clustering::getNumIterations() const
{
    return _numIterations;
}

/**
 * @brief  Gets a string with information about the provided cluster (index
 * of a cluster). That string contains a line for each location in the given
 * cluster. The information of each location consists of an integer with
 * its location index (whithin the _location field), a whitespace, and
 * the information returned by the Location::toString() method for that
 * location. An example is the following (a cluster with three locations):
0 3.300000 9.100000 Graduate College
6 12.600000 17.500000 Admission Information Center
7 12.500000 20.000000 Rockefeller College
 * Query method
 * @param cluster An integer, that defines the index of a cluster. Input
 * parameter
 * @return Gets a string with information about the provided cluster. If the
 * clustering algorithm has not been run (_isDone is false) or an
 * invalid value of cluster is provide, then it returns an empty string.
 */
std::string Clustering::clusterInfo(int cluster) const
{
    if (!_isDone || cluster < 0 || cluster >= _K)
    {
        return "";
    }
    std::string result;
    for (int i = 0; i < _locations.getSize(); i++)
    {
        if (_clusters.at(i) == cluster)
        {
            result += std::to_string(i) + " " + _locations.at(i).toString() + "\n";
        }
    }
    return result;
}

/**
 * @brief Obtains a string with the some statistics data about this
 * Clustering object.
 * The first line contains the value of K; the second line contains the
 * value of the sum of within-cluster variances; the third line contains
 * the number of iterations used in the KMeans algorithm.
 * Query method
 * @return A string with the content described above
 */
std::string Clustering::getStatistics() const
{
    std::string result;
    result += "K=" + std::to_string(_K) + "\n";
    result += "Sum of within-cluster variances: " + std::to_string(_sumWCV) + "\n";
    result += "Number of iterations: " + std::to_string(_numIterations) + "\n";
    return result;
}

/**
 * @brief Indicates whether this Clustering object is equivalent to the
 * provided Clustering object (@p other). Two Clustering objects are
 * considered equivalent if they have the same K value,
 * the same sum of within-cluster variances and the same number of
 * iterations in the KMeans algorithm.
 * If either this Clustering object or the provided Clustering object
 * (@p other) has not yet run the clustering algorithm (the run() method),
 * then they are not considered equivalent.
 * Query method
 * @param other A Clustering object. Input parameter
 * @return true if this Clustering object is equivalent to the provided
 * Clustering object (@p other); false otherwise
 */
bool Clustering::isEquivalentTo(const Clustering &other) const
{
    if (!_isDone || !other._isDone)
    {
        return false;
    }
    return _K == other._K && _sumWCV == other._sumWCV && _numIterations == other._numIterations;
}

/**
 * @brief Sets the vector of locations (_locations), the value of K (_K)
 * and the seed (_seed) with the provided values.
 * The fields _isDone, _clusters and _centroids_ are also initialized
 * appropriately: _isDone is set to false; _clusters is initialized with
 * a VectorInt object with size equal to the number of locations;
 * _centroids is initialized with a VectorLocation object with size K;
 * the fields _sumWCV and _numIterations are set to 0.
 * Modifier method
 * @throw std::invalid_argument Throws an std::invalid_argument exception
 * if K<=0
 * @param locations The vector of locations to used in the clustering
 * algorithm. Input parameter
 * @param K The parameter K for the K-Means algorithm. Input parameter
 * @param seed The seed used to initialize the random number generator
 * (with srand(seed)) in the initialClusterAssignment() method.
 * Input parameter
 */
void Clustering::set(const VectorLocation &locations, int K, unsigned int seed)
{
    if (K <= 0)
    {
        throw std::invalid_argument("K must be greater than 0");
    }

    _locations = locations;
    _K = K;
    _seed = seed;

    _isDone = false;
    _clusters = VectorInt(_locations.getSize());
    _centroids = VectorLocation(_K);
    _sumWCV = 0.0;
    _numIterations = 0;
}

/**
 * @brief Run the clustering algorithm. The clustering algorithm implemented
 * in this method is the KMeans algorithm.
 * After running this method the following postcondition should be verified:
 * @post The _centroids field should contain the vector of the means of
 * locations in each group
 * @post _clusters (a VectorInt object) should contain the cluster number
 * for each location in the vector of locations (_locations)
 * @post The _isDone field should be set to true
 * @post The _sumWCV field should be set to the sum of the within-cluster
 * variances using the claculateSumWithinClusterVariances() method.
 * @post The _numberIterations field should be set to the number of
 * iterations used by the KMeans algorithm.
 *
 * A brief description of the KMeans algorithm is the following:
 *
 * -# Perform an initial assignment of a cluster to each of the Location
 * objects. This must be done by initializing the random number
 * generator using srand(seed);
 * Then, for each location i, assign cluster number rand() % _K
 * to that location. Note that this step is done by modifying the _clusters
 * field.
 * -# Repeat while any location changes its cluster number:
 *     - For each cluster, recalculate its centroid (update _centroids
 *       field).
 *     - Assign each location to a cluster (update _clusters field).
 *       Each location is assigned to the cluster that provides
 *       the shortest distance to its centroid.
 *
 * Query method
 */
void Clustering::run()
{
    initialClusterAssignment();

    bool changed = true;
    _numIterations = 0;

    while (changed)
    {
        changed = false;
        _numIterations++;

        // Recalcular centroides
        for (int c = 0; c < _K; c++)
        {
            double sumX = 0.0;
            double sumY = 0.0;
            int count = 0;

            for (int i = 0; i < _locations.getSize(); i++)
            {
                if (_clusters.at(i) == c)
                {
                    sumX += _locations.at(i).getX();
                    sumY += _locations.at(i).getY();
                    count++;
                }
            }

            if (count > 0)
            {
                _centroids.at(c).set(sumX / count, sumY / count, "");
            }
            else
            {
                _centroids.at(c).set(0.0, 0.0, "");
            }
        }

        // Reasignar cada localización al centroide más cercano
        for (int i = 0; i < _locations.getSize(); i++)
        {
            int bestCluster = 0;
            double bestDistance = _locations.at(i).squaredDistance(_centroids.at(0));

            for (int c = 1; c < _K; c++)
            {
                double dist = _locations.at(i).squaredDistance(_centroids.at(c));
                if (dist < bestDistance)
                {
                    bestDistance = dist;
                    bestCluster = c;
                }
            }

            if (_clusters.at(i) != bestCluster)
            {
                _clusters.at(i) = bestCluster;
                changed = true;
            }
        }
    }

    calculateSumWCV();

    _isDone = true;
}

/**
 * @brief Obtains a string with the fields of this Clustering object.
 * The first line contains the value of K; the second line contains the
 * value of the sum of within-cluster variances; the third line contains
 * the number of iterations used in the KMeans algorithm.
 * In the next three lines, information about the cluster number for each
 * location.
 * If follows information about each centroid
 * Next, for each cluster, information about its locations
 * Here, an example is shown:
K=5
Sum of within-cluster variances: 161.136889
Number of iterations: 5
Cluster number for each location:
21
0 2 2 2 2 2 1 1 1 1 1 4 4 4 4 4 4 4 4 4 3
Centroids:
5
3.300000 9.100000
14.880000 21.280000
12.600000 9.460000
18.500000 6.700000
26.088889 15.366667
Cluster 0 information:
0 3.300000 9.100000 Graduate College
Cluster 1 information:
6 12.600000 17.500000 Admission Information Center
7 12.500000 20.000000 Rockefeller College
8 13.700000 22.800000 Art Museum Store
9 17.000000 23.300000 Princeton University Store
10 18.600000 22.800000 Labyrinth Books
Cluster 2 information:
1 11.800000 6.900000 Wawa Public Restrooms
2 12.600000 7.300000 Princeton Station
3 10.100000 9.700000 Forbes College
4 13.300000 12.200000 Dinky Bar & Kitchen
5 15.200000 11.200000 Whitman College
Cluster 3 information:
20 18.500000 6.700000 Yeh College
Cluster 4 information:
11 20.900000 15.400000 Frist Campus Center
12 23.700000 14.800000 Cannon Dial Elm
13 24.800000 14.900000 Quadrangle
14 25.600000 14.900000 Ivy
15 26.400000 14.900000 Cottage
16 27.300000 14.500000 Cap & Gown
17 28.200000 15.000000 Cloister
18 29.100000 15.200000 Charter
19 28.800000 18.700000 Engineering Quadrangle
 * Query method
 * @return A string with the content described above
 */

std::string Clustering::toString() const
{
    std::string result;
    result += getStatistics();
    result += "Cluster number for each location:\n";
    result += _clusters.toString();
    result += "Centroids:\n";
    result += _centroids.toString();
    for (int i = 0; i < _K; i++)
    {
        result += "Cluster " + std::to_string(i) + " information:\n";
        result += clusterInfo(i);
    }

    return result;
}

/**
     * @brief Performs an initial assignment of a cluster to each one of the
     * Location objects. This method begins by initializing the random number
     * generator using:
     * srand(seed);
     * Then, for each location i, assign cluster number rand() % _K to that
     * location.
     * Modifier method
     */
    void Clustering::initialClusterAssignment(){
        srand(_seed);

        for (int i = 0; i < _locations.getSize(); i++){
            _clusters.at(i) = rand() % _K;
        }
    }

    /**
     * @brief Calculates the sum of the within-cluster variances of this
     * Clustering object.
     * The within-cluster variance of a cluster C_j is calcultated with:
     * $\sum_{p_i \in C_j} squaredDistance(p_i, centroid_{C_j})$
     * @return A double with the sum of within-cluster variances of this
     * Clustering
     * Modifier method
     */
    double Clustering::calculateSumWCV(){
        _sumWCV = 0.0;

        for (int i = 0; i < _locations.getSize(); i++){
            int cluster = _clusters.at(i);
            _sumWCV += _locations.at(i).squaredDistance(_centroids.at(cluster));
        }

        return _sumWCV;
    }
