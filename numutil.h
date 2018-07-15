#include <limits.h>

class NumUtil
{
public:
	static bool isIntegerPlusOverflow(int num1, int num2)
	{
		int sum = num1 + num2;
		return 	((sum ^ num1) < 0 && (sum^num2) < 0);
	}

	static bool isIntegerMulOverflow(int a, int b)
	{
		if (a >= 0 && b >= 0)
		{
			return INT_MAX / a < b;
		}
		else if (a < 0 && b < 0)
		{
			return INT_MAX / a > b;
		}
		else if (a * b == INT_MIN)
		{
			return 0;
		}
		else
		{
			return a < 0 ? isIntegerMulOverflow(-a, b) : isIntegerMulOverflow(a, -b);
		}
	}
 };
