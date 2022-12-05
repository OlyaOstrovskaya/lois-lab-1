/* Лабораторная работа 1 по дисциплине ЛОИС
Выполнена студентами группы 921702 БГУИР
Мурашко И.А. и Островская О.В. - разработали программу, в которой отображён алгоритм
Пыск Д.А. и Медведева М.С. - произвели анализ и отобразили результат этого в отчёте
Вариант 24 - Реализовать прямой нечеткий логический вывод, используя импликацию на основе драстического произведения
05.12.2022
Использованные материалы:
"Нечеткий логический вывод в системе принятия решений" А. А. Ахрем, М. Р. Ашинянц. С. А. Петров */

#include <Windows.h>  
#include "resource.h"
#include "fuzzy_logic.h"

HMODULE GCM() 
{
	HMODULE h_module = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&h_module);
	return h_module;
}

int len(string str)
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		length++;

	}
	return length;
}

vector<string> split(string str, char seperator)
{
	vector<string> strings;
	int currIndex = 0, i = 0;
	int startIndex = 0, endIndex = 0;
	while (i <= len(str))
	{
		if (str[i] == seperator || i == len(str))
		{
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			if (subStr.size() > 0)
			{
				strings.push_back(subStr);
			}
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
	return strings;
}

vector<string> parse_strings_from_file() {
	string result;
	HRSRC h_res = FindResource(GCM(), MAKEINTRESOURCE(DEMO_TEXT), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL h_data = LoadResource(GCM(), h_res);
	DWORD h_size = SizeofResource(GCM(), h_res);
	char* h_final = (char*)LockResource(h_data);
	result.assign(h_final, h_size);
	return split(result, '\n');
}

void enter_the_rule(set& set_x, set& set_y, fuzzy_sets sets) {
	while (true)
	{
		try {
			cout << "Enter the rule: ";
			string rule;
			cin >> rule;

			int impl_id = rule.find_first_of("~");
			if (impl_id <= 0 || impl_id == rule.size() - 2)
			{
				throw exception("");
			}
			string x = rule.substr(0, impl_id);
			set_x = find_by_name(sets, x);
			string y = rule.substr(impl_id + 2);
			set_y = find_by_name(sets, y);
			break;
		}
		catch (const std::exception&) {
			cout << "Wrong input X_x, try again" << endl;
		}

	}
}

void validation_for_user_input(float val) {
	if (val < 0 || val > 1)
	{
		throw exception("");
	}
}

arr enter_package2(impl_matrix matrix, set set_y) {
	arr package;
	cout << "enter degree of fuzzy [0,1]" << endl;
	string inp_var = anti_element_of_arr(set_y.second);

	size_t i = 0;
	while (i < matrix.size())
	{
		try
		{
			string str_val;
			string var = inp_var + to_string(i + 1);
			cout << "for " << var << ": ";
			cin >> str_val;
			float val = stof(str_val);
			validation_for_user_input(val);
			package.push_back(make_pair(var, val));
			i++;
		}
		catch (const std::exception&)
		{
			cout << "Invalid input, try again" << endl;
		}
	}

	return package;
}

set enter_package(fuzzy_sets sets) {
	while (true)
	{
		try
		{
			cout << "Choose a package: ";
			string str_package;
			cin >> str_package;
			return find_by_name(sets, str_package);
		}
		catch (const std::exception&)
		{
			cout << "Cannot find package, ";
		}
	}
}


void show_result(set set_x, set set_y, set package,  arr rez_arr) {
	string func = "(" + package.first + "/\\" + "(" + set_x.first + "~>" + set_y.first + "))";
	cout << "Result of " + func + ": ";

	for (size_t j = 0; j < rez_arr.size(); j++)
	{
		if (j != 0)
		{
			cout << ",";
		}
		cout << "(" << rez_arr[j].first << ",";
		cout << rez_arr[j].second << ")";
	}
}

int main()
{
	try 
	{
		fuzzy_sets sets = parse_fuzzy_sets_from_file(parse_strings_from_file());
		while (true)
		{
			system("cls");
			show_sets(sets);
			set set_x, set_y;
			enter_the_rule(set_x, set_y, sets);

			impl_matrix matrix = fill_implementation_matrix(set_x, set_y);
			set package = enter_package(sets);
			arr rez_arr = compute_direct_output(matrix, package.second);
			show_impl_matrix(matrix);
			show_result(set_x, set_y, package ,rez_arr);

			cout << endl << "Enter exit to exit or any symbol to continue" << endl;
			string ex;
			cin >> ex;
			cin.ignore();
			if (ex == "exit")
			{
				break;
			}
			else
			{
				continue;
			}
		}
	}
	catch (const std::exception&)
	{
		cout << "Invalid input\n";
	}
	return 0;
}