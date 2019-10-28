#pragma once

#include <iostream>
#include <exception>

struct DepedencyException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Circular Depedency Exception";
    }
};

struct SyntaxException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Syntax Error Exception";
    }
};