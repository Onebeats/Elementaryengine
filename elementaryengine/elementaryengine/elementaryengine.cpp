// Elementaryengine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EEngine.h"
#include <iostream>


int main()
{
	EEngine* engine = new EEngine();

	try {
		engine->start();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}

