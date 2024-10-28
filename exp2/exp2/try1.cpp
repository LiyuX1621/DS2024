#include <iostream>
#include <cctype>
#include <cmath>
#include <vector>

using namespace std;

template <typename T>
class Stack
{
public:
	Stack(int size);
	~Stack();
	bool IsEmpty() { return top == -1; }
	bool IsFull() { return top == maxTop; }
	T Top();
	void Push(const T x);
	T Pop();
	void DisplayStack();
private:
	int maxTop;
	int top;
	T* values;
};

template <typename T>
Stack<T>::Stack(int size)
{
	maxTop = size - 1;
	values = new T[size];
	top = -1;
}

template <typename T>
Stack<T>::~Stack()
{
	delete[] values;
}

template <typename T>
void Stack<T>::Push(const T x)
{
	if (IsFull())
		cout << "Error: the stack is full." << endl;
	else
		values[++top] = x;
}

template <typename T>
T Stack<T>::Pop()
{
	if (IsEmpty())
	{
		cout << "Error: the stack is empty." << endl;
		return T();
	}
	else
	{
		return values[top--];
	}
}

template <typename T>
T Stack<T>::Top()
{
	if (IsEmpty())
	{
		cout << "Error: the stack is empty." << endl;
		return T();
	}
	else
		return values[top];
}

template <typename T>
void Stack<T>::DisplayStack()
{
	cout << "top -->";
	for (int i = top; i >= 0; i--)
		cout << "\t|\t" << values[i] << "\t|" << endl;
	cout << "\t|---------------|" << endl;
}
/// 




const int N_OPTR = 9;

const char pri[N_OPTR][N_OPTR] =
{
	{'>', '>', '<', '<', '<', '<', '<', '>', '>'},
	{'>', '>', '<', '<', '<', '<', '<', '>', '>'},
	{'>', '>', '>', '>', '<', '<', '<', '>', '>'},
	{'>', '>', '>', '>', '<', '<', '<', '>', '>'},
	{'>', '>', '>', '>', '>', '<', '<', '>', '>'},
	{'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
	{'<', '<', '<', '<', '<', '<', '<', '=', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{'<', '<', '<', '<', '<', '<', '<', ' ', '='}
};

int optrIndex(char op) {
	switch (op) {
	case '+': return 0;
	case '-': return 1;
	case '*': return 2;
	case '/': return 3;
	case '^': return 4;
	case '!': return 5;
	case '(': return 6;
	case ')': return 7;
	case '\0': return 8;
	default: return -1;
	}
}

char getPriority(char topOp, char currentOp)
{
	int i = optrIndex(topOp);
	int j = optrIndex(currentOp);
	if (i == -1 || j == -1) {
		cout << "错误的运算符" << endl;
		exit(-1);
	}
	return pri[i][j];
}

double factorial(int n)
{
	if (n < 0)
	{
		cout << "Error" << endl;
		exit(-1);
	}
	double result = 1;
	for (int i = 1; i <= n; ++i)
	{
		result *= i;
	}
	return result;
}

double calcu(double operand1, char op, double operand2 = 0)
{
	switch (op)
	{
	case '+':
		return operand1 + operand2;
	case '-':
		return operand1 - operand2;
	case '*':
		return operand1 * operand2;
	case '/':
		if (operand2 == 0)
		{
			cout << "Error" << endl;
			exit(-1);
		}
		return operand1 / operand2;
	case '^':
		return pow(operand1, operand2);
	default:
		cout << "Error" << endl;
		exit(-1);
	}
}

double calcu(char op, double operand)
{
	switch (op)
	{
	case '!':
		if (operand < 0 || operand != floor(operand))
		{
			cout << "Error" << endl;
			exit(-1);
		}
		return factorial(static_cast<int>(operand));
	default:
		cout << "Error" << endl;
		exit(-1);
	}
}


double evaluate(const char* s)
{
	Stack<double> opnd(100);
	Stack<char> optr(100);
	optr.Push('\0');
	while (!optr.IsEmpty())
	{
		if (isdigit(*s))
		{
			double num = 0;
			while (isdigit(*s))
			{
				num = num * 10 + (*s - '0');
				s++;
			}
			opnd.Push(num);
		}
		else
		{
			switch (getPriority(optr.Top(), *s))
			{
			case '<':
				optr.Push(*s);
				s++;
				break;

			case '>':
			{
				char op = optr.Pop();
				if ('!' == op)
				{
					opnd.Push(calcu(op, opnd.Pop()));
				}
				else
				{
					double opnd2 = opnd.Pop();
					double opnd1 = opnd.Pop();
					opnd.Push(calcu(opnd1, op, opnd2));
				}
				break;
			}

			case '=':
				optr.Pop();
				s++;
				break;
			}
		}
	}
	return opnd.Pop();
}

int largestRectangleArea(const vector<int>& heights)
{
	Stack<int> s(100);
	int maxArea = 0;
	int n = heights.size();

	for (int i = 0; i < n; i++)
	{
		while (!s.IsEmpty() && heights[i] < heights[s.Top()])
		{
			int h = heights[s.Top()];
			s.Pop();
			int width = s.IsEmpty() ? i : i - s.Top() - 1;
			maxArea = max(maxArea, h * width);
		}
		s.Push(i);
	}

	while (!s.IsEmpty())
	{
		int h = heights[s.Top()];
		s.Pop();
		int width = s.IsEmpty() ? n : n - s.Top() - 1;
		maxArea = max(maxArea, h * width);
	}

	return maxArea;
}

vector<int> Random(int a, int b)
{

	int numBars = rand() % a + 1;
	vector<int> heights;
	for (int i = 0; i < numBars; i++)
	{
		heights.push_back(rand() % (b + 1));
	}

	return heights;
}
int main()
{
	srand(time(0));
	const char* s = "8+5*6+4^3*4+7/2+6!+9*(2*5+3)";
	cout << "算式: " << s << endl;
	cout << evaluate(s) << endl;
	for (int i = 0; i < 10; i++)
	{
		vector<int> heights = Random(15, 7);
		cout << "数组是" << endl;
		for (size_t i = 0; i < heights.size(); i++)
		{
			cout << heights[i];
			if (i < heights.size() - 1)
				cout << ",";
		}
		cout << endl;
		cout << "面积是" << largestRectangleArea(heights) << endl;
	}
}