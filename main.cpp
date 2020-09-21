#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <map>

using namespace std;

string subString(int start, int end, string in)
{
	string out = "";
	for(int i = start ; i < end; i++)
		out += in[i];
	return out;
}

int parseCommand(string command)
{
	int i = 0;
	while(command[i] == ' ')
		i++;
	if(command[i] == '!')
		return 2;
	else if(command[i] == '?')
		return 1;
	else
		return 0;
}

class OutputInstruction
{
private:
	vector<string> terms;
	vector<char> operators;
public:
	OutputInstruction(string instruction);
	void printOutput(map<string, int> &variables);
};

OutputInstruction::OutputInstruction(string instruction)
{
	int i = 0;
	while(instruction[i] == ' ' || instruction[i] == '!')
		i++;
	while(1)
	{
		int startVar = i;
		while((instruction[i] >= 'a' && instruction[i] <= 'z') ||
			(instruction[i] >= 'A' && instruction[i] <= 'Z') || 
			(instruction[i] >= '0' && instruction[i] <= '9'))
			i++;
		terms.push_back(subString(startVar, i, instruction));
		while(instruction[i] == ' ')
			i++;
		if(i >= instruction.length())
			break;
		else
		{
			if(instruction[i] == '+' || instruction[i] == '-' || instruction[i] == '/' || instruction[i] == '*')
				operators.push_back(instruction[i]);
			i++;
		}
		while(instruction[i] == ' ')
			i++;
	}
}

void OutputInstruction::printOutput(map<string, int> &variables)
{
	int j = operators.size() - 1;
	int value;
	if(isalpha(terms[terms.size() - 1][0]))
	{
		auto itr = variables.find(terms[terms.size() - 1]);
		if(itr == variables.end())
		{
			variables.insert(make_pair(terms[terms.size() - 1], 0));
			value = 0;
		}
		else
			value = itr -> second;
	}
	else
		value = stoi(terms[terms.size() - 1]);
	for(int i = terms.size() - 2 ; i >= 0 ; i--)
	{
		int termVal;
		if(isalpha(terms[i][0]))
		{
			auto itr = variables.find(terms[i]);
			if(itr == variables.end())
			{
				variables.insert(make_pair(terms[i], 0));
				termVal = 0;
			}
			else	
				termVal = itr -> second;
		}
		else
			termVal = stoi(terms[i]);
		if(operators[j] == '+')
			value = termVal + value;
		else if(operators[j] == '-')
			value = termVal - value;
		else if(operators[j] == '*')
			value = termVal * value;
		else
			value = termVal / value;
		j--;
	}
	cout << value << endl;
}

class InputInstruction
{
private:
	string variable;
public:
	InputInstruction(string instruction);
	void getInput(map<string, int> &variables);	
};

InputInstruction::InputInstruction(string instruction)
{
	int i = 0;
	while(instruction[i] == ' ' || instruction[i] == '?')
		i++;
	int startVar = i;
	while(isalpha(instruction[i]))
		i++;
	variable = subString(startVar, i, instruction);
}

void InputInstruction::getInput(map<string, int> &variables)
{
	int input;
	cin >> input;
	auto itr = variables.find(variable);
	if(itr == variables.end())
		variables.insert(make_pair(variable, input));
	else
		itr -> second = input;
}

class AssignInstruction
{
private:
	vector<string> terms;
	vector<char> operators;
	string left;
public:
	AssignInstruction(string instruction);
	void evaluate(map<string, int> &variables);
};

AssignInstruction:: AssignInstruction(string instruction)
{
	int i = 0;
	while(instruction[i] == ' ')
		i++;
	int startLeft = i;
	while(isalpha(instruction[i]))
		i++;
	left = subString(startLeft, i, instruction);
	while(instruction[i] == ' ' || instruction[i] == '=')
		i++;
	while(1)
	{
		int startVar = i;
		while((instruction[i] >= 'a' && instruction[i] <= 'z') ||
			(instruction[i] >= 'A' && instruction[i] <= 'Z') || 
			(instruction[i] >= '0' && instruction[i] <= '9'))
			i++;
		terms.push_back(subString(startVar, i, instruction));
		while(instruction[i] == ' ')
			i++;
		if(i >= instruction.length())
			break;
		else
		{
			if(instruction[i] == '+' || instruction[i] == '-' || instruction[i] == '/' || instruction[i] == '*')
				operators.push_back(instruction[i]);
			i++;
		}
		while(instruction[i] == ' ')
			i++;
	}
}

void AssignInstruction::evaluate(map<string, int> &variables)
{
	int j = operators.size() - 1;
	int value;
	if(isalpha(terms[terms.size() - 1][0]))
	{
		auto itr = variables.find(terms[terms.size() - 1]);
		if(itr == variables.end())
		{
			variables.insert(make_pair(terms[terms.size() - 1], 0));
			value = 0;
		}
		else
			value = itr -> second;
	}
	else
		value = stoi(terms[terms.size() - 1]);
	for(int i = terms.size() - 2 ; i >= 0 ; i--)
	{
		int termVal;
		if(isalpha(terms[i][0]))
		{
			auto itr = variables.find(terms[i]);
			if(itr == variables.end())
			{
				variables.insert(make_pair(terms[i], 0));
				termVal = 0;
			}
			else	
				termVal = itr -> second;
		}
		else
			termVal = stoi(terms[i]);
		if(operators[j] == '+')
			value = termVal + value;
		else if(operators[j] == '-')
			value = termVal - value;
		else if(operators[j] == '*')
			value = termVal * value;
		else
			value = termVal / value;
		j--;
	}
	auto itr = variables.find(left);
	if(itr == variables.end())
		variables.insert(make_pair(left, value));
	else
		itr -> second = value;
}

int main()
{
	map<string, int>variables;
	string command;
	AssignInstruction *assignInst;
	OutputInstruction *outInst;
	InputInstruction *inInst;
	while(getline(cin, command))
	{
		if(command.empty())
			continue;
		int option = parseCommand(command);;
		if(option == 0)
		{
			assignInst = new AssignInstruction(command);
			assignInst->evaluate(variables);
			delete assignInst;
		}
		else if(option == 1)
		{
			inInst = new InputInstruction(command);
			inInst->getInput(variables);
			delete inInst;
		}
		else
		{
			outInst = new OutputInstruction(command);
			outInst->printOutput(variables);
			delete outInst;
		}
	}
}