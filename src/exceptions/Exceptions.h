#pragma once

#include <exception>

class SizeMissmatchException : public std::exception
{
public:
    const char *what() const throw()
    {
        return "in_states and out_states size mismatch";
    }
};

class InputSizeMissmatchException : public std::exception
{
public:
    const char *what() const throw()
    {
        return "input size missmatch";
    }
};
