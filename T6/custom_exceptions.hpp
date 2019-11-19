#pragma once

#include <iostream>
#include <exception>

struct DepedencyException : public std::exception
{
    static const int TYPE = 0x01;
	const char * what () const throw ()
    {
    	return "Circular Depedency Exception";
    }
};

struct SyntaxException : public std::exception
{
    static const int TYPE = 0x02;
	const char * what () const throw ()
    {
    	return "Syntax Error Exception";
    }
};