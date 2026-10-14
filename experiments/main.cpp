#include "1/experiment1.hpp"
#include "2/experiment2.hpp"
#include "3/experiment3.hpp"
#include "4/experiment4.hpp"
#include "5/experiment5.hpp"
#include "6/experiment6.hpp"
#include "7/experiment7.hpp"
#include <iostream>
#include <string>

void run_all_experiments() {
	run_experiment1();
	system("pause");
	system("cls");

	run_experiment2();
	system("pause");
	system("cls");

	run_experiment3();
	system("pause");

	run_experiment4();
	system("pause");

	run_experiment5();
	system("pause");

	run_experiment6();
	system("pause");

	run_experiment7();
	system("pause");
	std::cout << "\n\neksperimentai baigti\n\n";
}

int main() {
	std::string input = "";
	std::cout << "iveskite nuo 1 - 7 atitinkamam eksperimentui atlikti, 0 - iseiti, 100 - run all experiments\n";
	std::getline(std::cin, input);
	int choice = std::stoi(input);
	switch (choice)
	{
	case(0):
		std::cout << "bye";
		return 0;
	case(100):
		run_all_experiments();
		break;
	case(1):
		run_experiment1();
		break;
	case(2):
		run_experiment2();
		break;
	case(3):
		run_experiment3();
		break;
	case(4):
		run_experiment4();
		break;
	case(5):
		run_experiment5();
		break;
	case(6):
		run_experiment6();
		break;
	case(7):
		run_experiment7();
		break;
	default:
		std::cout << "iveskite nuo 1 - 7 atitinkamam eksperimentui atlikti, 100 - visus \n";
		break;
	}
	return 0;
}