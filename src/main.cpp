/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file main.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 * 
 * Created on 24 de octubre de 2025, 9:27
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <exception>

#include "DataSet.h"
#include "Clustering.h"

using namespace std;

const int DEFAULT_K = 5;
const string DEFAULT_OUTPUT_FILE = "tests/output/output.dts";

/**
 * Shows help about the use of this program in the given output stream.
 * IMPORTANT: this function is void and has no return statement.
 */
void showHelp(ostream &outputStream, const string &message)
{
    outputStream << "ERROR in Fraud3 parameters. " << message << endl;
    outputStream << "Run with the following arguments: " << endl;
    outputStream << "Fraud3 [-K <K>] [-o <outputFile.dts>] <inputFile.dts>" << endl;
    outputStream << endl;
    outputStream << "Parameters: " << endl;
    outputStream << "-K <K>: an integer with the number of clusters to use "
                 << "(5 by default)" << endl;
    outputStream << "-o <outputFile.dts>: name of the output dataset file "
                 << "(tests/output/output.dts by default)" << endl;
    outputStream << "<inputFile.dts>: name of the input dataset file" << endl;
}

/**
 * Converts a C-string into an int, checking that the whole string is numeric.
 */
bool stringToInt(const char *text, int &value)
{
    bool ok = true;

    if (text == nullptr || text[0] == '\0')
    {
        ok = false;
    }
    else
    {
        char *end = nullptr;
        errno = 0;
        long result = strtol(text, &end, 10);

        if (errno != 0 || end == text || *end != '\0' || result < INT_MIN || result > INT_MAX)
        {
            ok = false;
        }
        else
        {
            value = static_cast<int>(result);
        }
    }

    return ok;
}

/**
 * Checks whether an argument looks like an option.
 */
bool isOption(const char *text)
{
    return text != nullptr && text[0] == '-';
}

int main(int argc, char *argv[])
{
    int k = DEFAULT_K;
    string outputFile = DEFAULT_OUTPUT_FILE;
    string inputFile;

    int arg = 1;
    bool validParameters = true;
    string errorMessage;

    while (validParameters && arg < argc && isOption(argv[arg]))
    {
        string option = argv[arg];

        if (option == "-K")
        {
            if (arg + 1 >= argc || isOption(argv[arg + 1]))
            {
                validParameters = false;
                errorMessage = "Number of clusters not provided after -K";
            }
            else
            {
                int readK = DEFAULT_K;

                if (!stringToInt(argv[arg + 1], readK) || readK <= 0)
                {
                    validParameters = false;
                    errorMessage = "Invalid number of clusters after -K";
                }
                else
                {
                    k = readK;
                    arg += 2;
                }
            }
        }
        else if (option == "-o")
        {
            if (arg + 1 >= argc || isOption(argv[arg + 1]))
            {
                validParameters = false;
                errorMessage = "Output file not provided after -o";
            }
            else
            {
                outputFile = argv[arg + 1];
                arg += 2;
            }
        }
        else
        {
            validParameters = false;
            errorMessage = "Unknown parameter " + option;
        }
    }

    if (validParameters)
    {
        if (arg >= argc)
        {
            validParameters = false;
            errorMessage = "Input file not provided";
        }
        else
        {
            inputFile = argv[arg];
            arg++;
        }
    }

    if (validParameters && arg < argc)
    {
        validParameters = false;
        errorMessage = "Too many input files";
    }

    if (!validParameters)
    {
        showHelp(cerr, errorMessage);
        return 1;
    }

    try
    {
        DataSet inputDataSet;
        inputDataSet.load(inputFile);

        Clustering clustering;
        clustering.set(inputDataSet.getVectorLocation(), k);
        clustering.run();

        DataSet outputDataSet = inputDataSet.getReducedDataSet(clustering);
        outputDataSet.save(outputFile);
    }
    catch (const exception &e)
    {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}
