/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file DataSet.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 30 de julio de 2025, 10:00
 */

#include "DataSet.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

const string DataSet::MAGIC_STRING_T = "MP-FRAUD_DATASET-T-1.0";

void DataSet::allocate( int numInstances, int numLocations)
{
    if (numInstances < 0 || numLocations < 0)
    {
        throw out_of_range("Negative size in DataSet::allocate()");
    }

    _nInstances = numInstances;
    _nLocations = numLocations;
    _values = nullptr;

    if (_nInstances > 0 && _nLocations > 0)
    {
        _values = new int *[_nInstances];

        for (int i = 0; i < _nInstances; i++)
        {
            _values[i] = nullptr;
        }

        try
        {
            for (int i = 0; i < _nInstances; i++)
            {
                _values[i] = new int[_nLocations];
            }
        }
        catch (...)
        {
            for (int i = 0; i < _nInstances; i++)
            {
                delete[] _values[i];
            }

            delete[] _values;
            _values = nullptr;
            _nInstances = 0;
            _nLocations = 0;
            throw;
        }
    }
}

void DataSet::deallocate()
{
    if (_values != nullptr)
    {
        for (int i = 0; i < _nInstances; i++)
        {
            delete[] _values[i];
        }

        delete[] _values;
        _values = nullptr;
    }

    _nInstances = 0;
    _nLocations = 0;
}

void DataSet::copyFrom(const DataSet &orig)
{
    allocate(orig._nInstances, orig._nLocations);

    _labels = orig._labels;
    _locations = orig._locations;

    for (int i = 0; i < _nInstances; i++)
    {
        for (int j = 0; j < _nLocations; j++)
        {
            _values[i][j] = orig._values[i][j];
        }
    }
}

void DataSet::checkInstanceIndex(int instanceIndex) const
{
    if (instanceIndex < 0 || instanceIndex >= _nInstances)
    {
        throw out_of_range("Invalid instance index");
    }
}

void DataSet::checkLocationIndex(int locationIndex) const
{
    if (locationIndex < 0 || locationIndex >= _nLocations)
    {
        throw out_of_range("Invalid location index");
    }
}

/**
 * @brief It builds a DataSet object with the provided number of instances
 * and number of locations. This class uses a bidimensional matrix with 
 * nInstances rows and nLocations columns to store the values of the set 
 * of instances.
 * Note that this constructor also builds the vector (with a size equal to 
 * nInstances) of labels and the vector of locations (with a size equal to 
 * nLocations).
 * This constructor initializes with a value of 0 all the values in the 2D
 * matrix. 
 * The vector of labels is initialized with a value of 0 for each instance.
 * The Location objects in the vector of locations are built with the 
 * Location default constructor.
 * @param nInstances An integer with the number of instances.
 * Input parameter
 * @param nLocations An integer with the number of locations.
 * Input parameter
 */
DataSet::DataSet(int nInstances, int nLocations)
    : _values(nullptr),
      _nInstances(0),
      _nLocations(0),
      _labels(),
      _locations()
{
    allocate(nInstances, nLocations);

    _labels = VectorInt(_nInstances);
    _locations = VectorLocation(_nLocations);

    initInstances(0);
    _labels.assign(0);
}

/**
 * @brief Copy constructor
 * @param orig the DataSet object used as source for the copy. 
 * Input parameter
 */
DataSet::DataSet(const DataSet &orig)
    : _values(nullptr),
      _nInstances(0),
      _nLocations(0),
      _labels(),
      _locations()
{
    copyFrom(orig);
}

/**
 * @brief Destructor
 */
DataSet::~DataSet()
{
    deallocate();
}

/**
 * @brief Overloading of the assignment operator for DataSet class
 * Modifier method
 * @param orig the DataSet object used as source for the assignment.
 * Input parameter
 * @return A reference to this object
 */
DataSet &DataSet::operator=(const DataSet &orig)
{
    if (this != &orig)
    {
        DataSet copy(orig);

        deallocate();

        _values = copy._values;
        _nInstances = copy._nInstances;
        _nLocations = copy._nLocations;
        _labels = copy._labels;
        _locations = copy._locations;

        copy._values = nullptr;
        copy._nInstances = 0;
        copy._nLocations = 0;
    }

    return *this;
}

/**
 * @brief Gets the number of instances in this DataSet
 * Query method
 * @return The number of instances in this DataSet
 */
int DataSet::getNumInstances() const
{
    return _nInstances;
}

/**
 * @brief Gets the number of localizations in this DataSet
 * Query method
 * @return The number of localizations in this DataSet
 */
int DataSet::getNumLocations() const
{
    return _nLocations;
}

/**
 * @brief Gets the value for the instance instanceIndex at the localization
 * locationIndex.
 * @throw std::out_of_range Throws a std::out_of_range exception if 
 * instanceIndex is not a valid index for an instance in this DataSet.
 * @throw std::out_of_range Throws a std::out_of_range exception if 
 * locationIndex is not a valid index for an location in this DataSet.
 * Query method
 * @param instanceIndex An integer with the index of the selected instance. 
 * Input parameter
 * @param locationIndex An integer with the index of the selected location. 
 * Input parameter
 * @return The value for the instance instanceIndex at the localization
 * locationIndex
*/   
int DataSet::getValue(int instanceIndex, int locationIndex) const
{
    checkInstanceIndex(instanceIndex);
    checkLocationIndex(locationIndex);

    return _values[instanceIndex][locationIndex];
}

/**
 * @brief Gets the label (integer value) of the instance at the provided
 * position (instanceIndex)
 * @throw std::out_of_range Throws a std::out_of_range exception if 
 * instanceIndex is not a valid index for an instance in this DataSet.
 * Query method
 * @param instanceIndex An integer with the index of the selected instance.
 * Input parameter
 * @return The label (integer value) of the instance at the provided
 * position
 */
int DataSet::getLabel(int instanceIndex) const
{
    checkInstanceIndex(instanceIndex);

    return _labels.at(instanceIndex);
}

/**
 * @brief Gets a const reference to the vector of Location objects in this
 * DataSet
 * Query method
 * @return A const reference to the vector of Location objects in this
 * DataSet
 */
const VectorLocation &DataSet::getVectorLocation() const
{
    return _locations;
}

/**
 * @brief Gets a const reference to the vector of labels in this DataSet
 * Query method
 * @return A const reference to the vector of labels in this DataSet
 */
const VectorInt &DataSet::getVectorLabels() const
{
    return _labels;
}

/**
 * @brief Obtains a string with information about this DataSet object, 
 * in the following format:
 * -Information about the set of locations of this DataSet:
 *    - A line with the number of locations
 *    - For each location, a line with its information, converted to
 *      a string with the Location::toString() method.
 * - Information about the labels of each instance in this DataSet:
 *    - A line with the number of instances
 *    - A line with the labels (space separated) of the set of instances.
 * - Instances of the Dataset. This is the information contained in the 
 *   2D matrix of this DataSet.
 *     - For each instance, a line with the values (space separated) of 
 *       that instance.
 * Query method
 * @return string with information about this CrimeSet object
 */
string DataSet::toString() const
{
    ostringstream output;

    output << _nLocations << '\n';

    for (int i = 0; i < _nLocations; i++)
    {
        output << _locations.at(i).toString() << '\n';
    }

    output << _nInstances << '\n';

    for (int i = 0; i < _nInstances; i++)
    {
        output << _labels.at(i);

        if (i < _nInstances - 1)
        {
            output << ' ';
        }
    }

    output << '\n';

    for (int i = 0; i < _nInstances; i++)
    {
        for (int j = 0; j < _nLocations; j++)
        {
            output << _values[i][j];

            if (j < _nLocations - 1)
            {
                output << ' ';
            }
        }

        output << '\n';
    }

    return output.str();
}

/**
 * @brief Sets a new value for the instance instanceIndex at the 
 * localization locationIndex.
 * @throw std::out_of_range Throws a std::out_of_range exception if 
 * instanceIndex is out of the valid range
 * @throw std::out_of_range Throws a std::out_of_range exception if 
 * locationIndex is out of the valid range
 * Modifier method
 * @param instanceIndex An integer with the index of the selected instance. 
 * Input parameter
 * @param locationIndex An integer with the index of the selected location. 
 * Input parameter
 * @param value An integer with the new value for the selected instance.
 * Input parameter
 */
void DataSet::setValue(int instanceIndex, int locationIndex, int value)
{
    checkInstanceIndex(instanceIndex);
    checkLocationIndex(locationIndex);

    _values[instanceIndex][locationIndex] = value;
}

/**
 * @brief Sets a new the label for the provided instance.
 * Modifier method
 * @param instanceIndex An integer with the index of the selected instance. 
 * Input parameter
 * @param label An integer with the new value for the label of the selected
 * instance.
 * Input parameter
 */
void DataSet::setLabel(int instanceIndex, int label)
{
    checkInstanceIndex(instanceIndex);

    _labels.at(instanceIndex) = label;
}

/**
 * Assigns the provided value to each instance and location. That is, all 
 * instances of this DataSet are initialized with the provided value.
 * Modifier method
 * @param value An integer with the value to assign to every instance and
 * location
 * Input parameter
 */
void DataSet::initInstances(int value)
{
    for (int i = 0; i < _nInstances; i++)
    {
        for (int j = 0; j < _nLocations; j++)
        {
            _values[i][j] = value;
        }
    }
}

/**
 * @brief Removes all the information in this DataSet object:
 * - The vector of Location objects (_locations) is left empty. That is,
 * it should represent a vector with 0 locations.
 * - The vector of labels (_labels) is left empty. That is, it should 
 * represent a vector with 0 labels.
 * - The matrix (_values) with the values for each instance is left empty. 
 * That is, it should represent a matrix with 0 instances and 0 locations.
 * - The remaining fields of this object are updated accordingly.
 * Modifier method
 */
void DataSet::clear()
{
    deallocate();
    _labels.clear();
    _locations.clear();
}

/**
 * @brief Saves this DataSet object in the given file. See files *.dts in 
 * the folder DataSets as example of files with DataSet information. 
 * Note that the format of the output file is the same as the string that
 * returns the toString() method, except that the save() method should 
 * insert the magic string at the beginning of the output file.
 * @throw std::ios_base::failure Throws a std::ios_base::failure exception 
 * if the given file cannot be opened or if an error occurs while writing
 * to the file.
 * Query method
 * @param fileName The name of the file where the DataSet will be saved. 
 * Input parameter
 */
void DataSet::save(const string &fileName) const
{
    ofstream output(fileName);

    if (!output)
    {
        throw ios_base::failure("Could not open output file: " + fileName);
    }

    output << MAGIC_STRING_T << '\n';
    output << toString();

    if (!output)
    {
        throw ios_base::failure("Error writing output file: " + fileName);
    }
}

/**
 * @brief Loads into this object the DataSet information stored in the 
 * given file. See files *.dts in the folder DataSets as example of files
 * with DataSet information. 
 * @note Note that this method should remove any previous information that
 * this object previously contained.
 * @note This method throws an exception in some error cases (see below). 
 * Before throwing the corresponding exception, this method clears
 * the object (it calls to clear() method) to leave the object in a 
 * consistent state.
 * @throw throw std::invalid_argument Throws a std::invalid_argument 
 * exception if an invalid magic string is found in the given file
 * @throw std::out_of_range Throws a std::out_of_range exception if the 
 * number of locations or the number of instances in the given file is 
 * negative.
 * Modifier method
 * @param fileName The name of the file where the DataSet is stored. 
 * Input parameter
 * @throw std::ios_base::failure Throws a std::ios_base::failure exception 
 * if the given file cannot be opened or if an error occurs while reading
 * from the file. 
 */
void DataSet::load(const string &fileName)
{
    ifstream input(fileName);

    if (!input)
    {
        clear();
        throw ios_base::failure("Could not open input file: " + fileName);
    }

    try
    {
        string magicString;
        getline(input, magicString);

        if (magicString != MAGIC_STRING_T)
        {
            throw invalid_argument("Invalid magic string in dataset file");
        }

        int numLocations;
        input >> numLocations;

        if (!input)
        {
            throw ios_base::failure("Error reading number of locations");
        }

        if (numLocations < 0)
        {
            throw out_of_range("Negative number of locations");
        }

        VectorLocation readLocations(numLocations);

        for (int i = 0; i < numLocations; i++)
        {
            Location location;
            location.load(input);
            readLocations.at(i) = location;
        }

        int numInstances;
        input >> numInstances;

        if (!input)
        {
            throw ios_base::failure("Error reading number of instances");
        }

        if (numInstances < 0)
        {
            throw out_of_range("Negative number of instances");
        }

        DataSet readDataSet(numInstances, numLocations);
        readDataSet._locations = readLocations;

        for (int i = 0; i < numInstances; i++)
        {
            int label;
            input >> label;

            if (!input)
            {
                throw ios_base::failure("Error reading labels");
            }

            readDataSet._labels.at(i) = label;
        }

        for (int i = 0; i < numInstances; i++)
        {
            for (int j = 0; j < numLocations; j++)
            {
                int value;
                input >> value;

                if (!input)
                {
                    throw ios_base::failure("Error reading dataset values");
                }

                readDataSet._values[i][j] = value;
            }
        }

        *this = readDataSet;
    }
    catch (...)
    {
        clear();
        throw;
    }
}

/**
 * @brief Gets a new DataSet from this DataSet. The DataSet will contain 
 * the same number of instances as this DataSet, but the number of locations
 * is given by the number of clusters in the provided Clustering object.
 * Therefore, the returned DataSet will have a reduced number of columns 
 * and an equal number of rows compared to this DataSet. 
 * The provided Clustering object defines how to reduce the set of columns.
 * Column 0 will contain the sum of the columns of this DataSet belonging 
 * to cluster 0, column 1 the sum of the columns belonging to cluster 1, 
 * and so on.
 * The vector of locations will be a copy of the vector of centroids of the
 * provided Clustering object.
 * The vector of labels will be a copy of the vector of labels in this 
 * DataSet.
 * @throw throw std::invalid_argument Throws a std::invalid_argument 
 * exception if the method Clustering::isDone() return false with the 
 * provided Clustering object.
 * @throw throw std::invalid_argument Throws a std::invalid_argument 
 * exception if the number of locations in the provided Clustering object
 * is not equal to the number of locations in this DataSet.
 * Query method
 * @param clustering A Clustering object.
 * Input parameter
 * @return A reduced DataSet
 */
DataSet DataSet::getReducedDataSet(const Clustering &clustering) const
{
    if (!clustering.isDone())
    {
        throw invalid_argument("The clustering algorithm has not been run");
    }

    if (clustering.getNumLocations() != _nLocations)
    {
        throw invalid_argument("The number of locations in clustering does not match DataSet");
    }

    int k = clustering.getK();
    DataSet reduced(_nInstances, k);

    reduced._locations = clustering.getCentroids();
    reduced._labels = _labels;
    reduced.initInstances(0);

    for (int col = 0; col < _nLocations; col++)
    {
        int clusterNumber = clustering.clusterOf(col);

        if (clusterNumber < 0 || clusterNumber >= k)
        {
            throw out_of_range("Invalid cluster number");
        }

        for (int row = 0; row < _nInstances; row++)
        {
            reduced._values[row][clusterNumber] += _values[row][col];
        }
    }

    return reduced;
}
