/**
 * @file stringManager.h
 * @author Rhet O. Hailey (rhet.hailey@auburn.edu)
 * @brief String functions used for the DARE project to utilize convenient UDP Communications
 * @version 0.1
 * @date 2024-06-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// data parsing
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>

/// @brief convert double vector to a delimited string
/// @param myVec - vector
/// @param delim - custom delimeter
/// @return delimited string
std::string vec2str(std::vector<double> myVec, char* delim)
{
    // create ostringstream, delimit, rename
    std::ostringstream oss;
    std::copy(myVec.begin(), myVec.end(), std::ostream_iterator<int>(oss, delim));
    std::string message(oss.str());

    return message;
}

/// @brief convert double vector to a delimited string
/// @param myVec - vector
/// @return comma delimited string
std::string vec2str(std::vector<double> myVec)
{
    const char* delim = ",";
    
    // create ostringstream, delimit, rename
    std::ostringstream oss;
    std::copy(myVec.begin(), myVec.end(), std::ostream_iterator<int>(oss, delim));
    std::string message(oss.str());

    return message;
}

/// @brief trim whitespace from string vector
/// @param str 
/// @return string
std::string removeWhitespace(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) { return std::isspace(c); }), result.end());
    return result;}

// std::string trimString(const std::string& str)
// {
//     auto start = str.begin();

//     while (start != str.end() && std::isspace(*start))
//     {   
//         start++;
//     }

//     auto end = str.end();
//     do
//     {
//         end--;
//     } while (std::distance(start, end) > 0 && std::isspace(*end));

//     return std::string(start, end + 1);
// }

/// @brief convert string to double vector
/// @param str 
/// @param delim - custom delimeter, default to "," if not used
/// @return double vector
std::vector<double> str2vec(const std::string &str, char delim)
{

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delim))
    {
        token = removeWhitespace(token);

        if (!token.empty()){
            tokens.push_back(token);
        }
    }

    std::vector<double> doubleVector(tokens.size());
    std::transform(tokens.begin(), tokens.end(), doubleVector.begin(), [](const std::string &val)
                   { return std::stod(val); });

    return doubleVector;
}

/// @brief convert string to double vector
/// @param str 
/// @return double vector
std::vector<double> str2vec(const std::string &str)
{
    const char delim = ',';
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delim))
    {
        token = removeWhitespace(token);

        if (!token.empty()){
            tokens.push_back(token);
        }
    }

    std::vector<double> doubleVector(tokens.size());
    std::transform(tokens.begin(), tokens.end(), doubleVector.begin(), [](const std::string &val)
                   { return std::stod(val); });

    return doubleVector;
}