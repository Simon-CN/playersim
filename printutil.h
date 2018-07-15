#include<iostream>

class PrintUtil
{
public:
	static void printResult(const char * errStr)
	{
		std::cout << "-ERR\t" << errStr << std::endl;
		std::cout << ">";
	}

	static void printResult()
	{
		std::cout << "+OK" << std::endl;
		std::cout << ">";
	}
};
