/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file VectorInt.h
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 *
 * Created on 30 de julio de 2025, 9:54
 */

#include <iostream>
#include <string>
#include <cmath>
#include "VectorInt.h"
#include "Location.h"


/**
 * @brief It builds a VectorInt object (vector of integers) with a
 * size and capacity equal to the provided value (@p size). Each element
 * will be filled with a value equal to 0.
 * @throw std::out_of_range Throws a std::out_of_range exception if
 * @p size < 0
 * @param size The size for the vector of integers in this object. Input
 * parameter
 */
VectorInt::VectorInt(int size){
    if (size < 0)
    {
        throw std::out_of_range("VectorInt size out of range");
    }

    _size = size;
    _capacity = size;
    ReservarMemoria();

    for (int i = 0; i < size; i++)
    {
        _values[i] = 0;
    }
}

/**
 * @brief Copy constructor
 * @param orig the VectorInt object used as source for the copy. Input
 * parameter
 */
VectorInt::VectorInt(const VectorInt &orig){
    Copiar(orig);
}

/**
 * @brief Destructor
 */
VectorInt::~VectorInt(){
    LiberarMemoria();
}

/**
 * @brief Overloading of the assignment operator for VectorInt class
 * Modifier method
 * @param orig the VectorInt object used as source for the assignment. Input
 * parameter
 * @return A reference to this object
 */
VectorInt &VectorInt::operator=(const VectorInt &orig){
    if(this != &orig){
        LiberarMemoria();
        Copiar(orig);
    }
    return *this;
}

/**
 * @brief Gets the number of elements in the vector of this object
 * Query method
 * @return The number of elements
 */
int VectorInt::getSize() const{
    return _size;
}

/**
 * @brief Gets the capacity of the vector in this object
 * Query method
 * @return The capacity of the vector in this object
 */
int VectorInt::getCapacity() const{
    return _capacity;
}

/**
 * @brief Compares the integer vectors of this object and the provided
 * object, and returns the number of identical elements in both of them. The
 * comparison is performed in order, element by element, in both vectors.
 * For example, given the following two vectors:
 * 2 1 2 3 5
 * 1 1 2 4 5
 * this method will return 3 (there is a match ​in positions 1, 2 and 4)
 * @throw std::invalid_argument Throws an std::invalid_argument exception
 * if the sizes of this and the provided object are different
 * Query method
 * @param other A VectorInt object. Input parameter
 * @return The number of identical elements in the vectors of this
 * and the provided object.
 */
int VectorInt::countIdenticalElements(const VectorInt &other){
    if (getSize() != other.getSize())
    {
        throw std::invalid_argument("VectorInt sizes are different");
    }
    int count = 0;
    for (int i = 0; i < getSize(); i++)
    {
        if (_values[i] == other._values[i])
        {
            count++;
        }
    }
    return count;
}


/**
 * @brief Obtains a string with information about this VectorInt object,
 * in the following format:
 * - First line, the number of elements in this vector.
 * - Second line, the elements in this vector, separated by a whitespace.
 * Take into account that a '\n' should appear after the last element
 * instead of a whitespace.
 * Query method
 * @return string with information about this VectorInt object
 */
std::string VectorInt::toString() const{
    std::string output;
    output += std::to_string(_size) + "\n";

    for (int i = 0; i < _size; i++)
    {
        output += std::to_string(_values[i]);
        if (i < _size - 1)
            output += " ";
    }

    output += "\n";
    return output;
}

/**
 * @brief Gets the Euclidean distance between this and the provided object.
 * The Euclidean distance between two points \f$ P=(p_1, p_2, ... , p_n) \f$
 * and \f$ Q=(q_1, q_2, ... , q_n) \f$ in an n-dimensional space \f$ R^n \f$
 * is the length of the line segment connecting them, calculated as the
 * square root of the sum of the squared differences of their components:
 * \f$ d(P,Q)=\sqrt{ \sum_{i=1}^{n} (p_i-q_i)^{2} } \f$
 * @throw std::invalid_argument Throws an std::invalid_argument exception
 * if the size of this object and the provided one are not equal
 * @throw std::invalid_argument Throws an std::invalid_argument exception
 * if the size of the provided object is zero
 * Query method
 * @param other A VectorInt. Input parameter
 * @return The Euclidean distance between this and the provided objects
 */
double VectorInt::distance(const VectorInt &other){
    if (other.getSize() == 0)
    {
        throw std::invalid_argument("VectorInt size is zero");
    }
    if (getSize() != other.getSize())
    {
        throw std::invalid_argument("VectorInt sizes are different");
    }

    double sum = 0.0;
    for (int i = 0; i < other.getSize(); i++)
    {
        sum += pow(_values[i] - other._values[i], 2);
    }
    return sqrt(sum);
}

/**
 * @brief Assigns the provided value to all the elements in this vector
 * Modifier method
 * @param value An integer value. Input parameter
 */
void VectorInt::assign(int value){
    for (int i = 0; i < getSize(); i++)
    {
        _values[i] = value;
    }
}

/**
 * @brief Appends the given integer value at the end (first free position)
 * of the array of integers in this object.
 * If the dynamic array of integers was full (its capacity was full), this
 * method automatically reallocates a new array with a capacity equal to
 * the current capacity plus an extra block of size equal to BLOCK_SIZE.
 * Modifier method
 * @param value the new integer value to be appended. Input parameter
 */
void VectorInt::append(int value){
    if (getSize() >= getCapacity())
    {
        Redimensionar();
    }
    _values[getSize()] = value;
    _size++;
}

/**
 * @brief Removes all the elements in this object, leaving the container
 * with a size equal to 0. It only need to set the number of elements
 * (_size field) to zero.
 * Modifier method
 */
void VectorInt::clear(){
    _size=0;
}

/**
 * @brief Gets a const reference to the integer element at the given
 * position
 * Query method
 * @param pos position in the VectorInt object. Input parameter
 * @throw std::out_of_range Throws an std::out_of_range exception if the
 * given position is not valid.
 * @return A const reference to the integer element at the given position
 */
const int &VectorInt::at(int pos) const{
    if (pos < 0 || pos >= getSize())
    {
        throw std::out_of_range("VectorInt position out of range");
    }
    return _values[pos];
}

/**
 * @brief Gets a reference to the integer element at the given position.
 * Modifier method
 * @param pos position in the VectorInt object. Input parameter
 * @throw std::out_of_range Throws an std::out_of_range exception if the
 * given position is not valid
 * @return A reference to the integer element at the given position.
 */
int &VectorInt::at(int pos){
     if (pos < 0 || pos >= getSize())
    {
        throw std::out_of_range("VectorInt position out of range");
    }
    return _values[pos];
}

void VectorInt::LiberarMemoria(){
    delete[] _values;
    _values = nullptr;
    _size = 0;
    _capacity = 0;
}

void VectorInt::ReservarMemoria(){
    _values = new int[_capacity];
}

void VectorInt::Copiar(const VectorInt &otro){
    _size = otro._size;
    _capacity = otro._capacity;
    ReservarMemoria();
    for(int i = 0; i < otro._size; i++){
        _values[i] = otro._values[i];
    }
}

void VectorInt::Redimensionar(){
    _capacity += BLOCK_SIZE;
    int* auxiliar = new int[_capacity];
    for(int i = 0; i < _size; i++){
        auxiliar[i] = _values[i];
    }
    delete[] _values;
    _values = auxiliar;
}
