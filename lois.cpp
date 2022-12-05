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

int length(string str)
{
	int result = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		result++;
	}
	return result;
}

vector<string> split_by_newline(string str)
{
	vector<string> result;
	int i = 0;
	int currentIndex = 0;
	int startIndex = 0;
	int endIndex = 0;
	while (i <= length(str))
	{
		if (str[i] == '\n' || i == length(str))
		{
			endIndex = i;
			string substring = "";
			substring.append(str, startIndex, endIndex - startIndex);
			if (substring.size() > 0)
			{
				result.push_back(substring);
			}
			currentIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
	return result;
}

vector<string> parse_strings_from_file() {
	string result;
	HRSRC h_res = FindResource(GCM(), MAKEINTRESOURCE(DEMO_TEXT), MAKEINTRESOURCE(TEXTFILE));
	HGLOBAL h_data = LoadResource(GCM(), h_res);
	DWORD h_size = SizeofResource(GCM(), h_res);
	char* h_final = (char*)LockResource(h_data);
	result.assign(h_final, h_size);
	return split_by_newline(result);
}

void input_rule(set& set_x, set& set_y, fuzzy_sets sets) {
	while (true)
	{
		try {
			cout << "Input the rule: ";
			string rule;
			cin >> rule;

			int implication_id = rule.find_first_of("~");
			if (implication_id <= 0 || implication_id == rule.size() - 2)
			{
				throw exception("");
			}
			string x = rule.substr(0, implication_id);
			set_x = find_set_by_name(sets, x);
			string y = rule.substr(implication_id + 2);
			set_y = find_set_by_name(sets, y);
			break;
		}
		catch (const std::exception&) {
			cout << "Wrong input X_x, try again" << endl << endl;
		}
	}
}

set input_package(fuzzy_sets sets) {
	while (true)
	{
		try
		{
			cout << "Choose a package: ";
			string str_package;
			cin >> str_package;
			cout << endl;
			return find_set_by_name(sets, str_package);
		}
		catch (const std::exception&)
		{
			cout << "Cannot find package ";
		}
	}
}

void print_result(set set_x, set set_y, set package, arr result) {
	string func = "(" + package.first + "/\\" + "(" + set_x.first + "~>" + set_y.first + "))";
	cout << "Result of " + func + ": {";

	for (size_t i = 0; i < result.size(); i++)
	{
		if (i != 0)
		{
			cout << ", ";
		}
		cout << "<" << result[i].first << ", ";
		cout << fixed << setprecision(1) << result[i].second << ">";
	}
	cout << "}" << endl;
}

int main()
{
	try 
	{
		fuzzy_sets sets = parse_fuzzy_sets_from_strings(parse_strings_from_file());
		while (true)
		{
			system("cls");
			print_sets(sets);
			set set_x;
			set set_y;

			input_rule(set_x, set_y, sets);

			impl_matrix matrix = build_implementation_matrix(set_x, set_y);
			set package = input_package(sets);
			arr result = calculate_direct_output(matrix, package.second);
			print_implication_matrix(matrix);
			print_result(set_x, set_y, package, result);

			cout << endl << "Enter any symbol to continue" << endl;
			string ex;
			cin >> ex;
			cin.ignore();
			continue;
		}
	}
	catch (const std::exception&)
	{
		cout << "Invalid input\n";
	}
	return 0;
}