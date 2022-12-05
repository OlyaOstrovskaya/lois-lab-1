/* Лабораторная работа 1 по дисциплине ЛОИС
Выполнена студентами группы 921702 БГУИР
Мурашко И.А. и Островская О.В. - разработали программу, в которой отображён алгоритм
Пыск Д.А. и Медведева М.С. - произвели анализ и отобразили результат этого в отчёте
Вариант 24 - Реализовать прямой нечеткий логический вывод, используя импликацию на основе драстического произведения
05.12.2022
Использованные материалы:
"Нечеткий логический вывод в системе принятия решений" А. А. Ахрем, М. Р. Ашинянц. С. А. Петров */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>

using namespace std;

using arr = vector<pair<string, float>>;
using set = pair<string, arr>;
using fuzzy_sets = vector<set>;
using impl_matrix = vector<arr>;

inline void validate_substring(string str) {
	if (str.empty() || str.find(')') != string::npos || str.find_last_of('(') != 0 || str.find_first_of(',') != str.find_last_of(','))
	{
		throw exception("");
	}
}

inline void validate_array_value(float array_value) {
	if (array_value < 0 || array_value > 1)
	{
		throw exception("");
	}
}

inline arr parse_array_out_of_string(string str) {
	arr result;
	while (!str.empty())
	{
		const size_t start = str.find_first_of('(');
		const size_t end = str.find_first_of(')');
		const size_t middle_comma = str.find_first_of(',');

		string substring = str.substr(start, end);
		validate_substring(substring);
		string array_variable = substring.substr(start + 1, middle_comma - start - 1);
		float array_value = stof(substring.substr(middle_comma + 1, end - middle_comma));
		validate_array_value(array_value);
		pair<string, float> pair = make_pair(array_variable, array_value);
		result.push_back(pair);
		str = str.substr(end + 2);
	}
	return result;
}

inline set parse_set_out_of_string(string str) {
	string set_name = str.substr(0, 1);
	arr array = parse_array_out_of_string(str.substr(str.find('=') + 2));
	return set(set_name, array);
}

inline fuzzy_sets parse_fuzzy_sets_from_strings(vector<string> strings) {
	fuzzy_sets result;
	for (size_t i = 0; i < strings.size(); i++)
	{
		result.push_back(parse_set_out_of_string(strings[i]));
	}
	return result;
}

inline set find_set_by_name(fuzzy_sets sets, string name) {
	set result;
	for (size_t i = 0; i < sets.size(); i++)
	{
		if (sets[i].first == name)
		{
			result = sets[i];
			break;
		}
	}
	if (result.first.empty())
	{
		throw invalid_argument("");
	}
	return result;
}

inline void print_sets(fuzzy_sets sets) {
	for (int i = 0; i < sets.size(); i++)
	{
		cout << sets[i].first << " = {";
		for (size_t j = 0; j < sets[i].second.size(); j++)
		{
			if (j != 0)
			{
				cout << ", ";
			}
			cout << "<" << sets[i].second[j].first << ", ";
			cout << fixed << setprecision(1) << sets[i].second[j].second << ">";
		}
		cout << "}" << endl;
	}
	cout << endl;
}

inline float lim(float n) {
	double s = 0;
	double q = 1;
	for (int i = 1; i <= n; i++) {
		q = q * 2;
		s += (2 * i - 1) / q;
	}
	return s;
}

inline float implication(float x, float y) {
	float z = rand();
	return min((float)1, (float)lim(exp(z * (log(y)))) + (float)y * exp(z * (log(x))));
}

inline void print_implication_matrix(impl_matrix matrix) {
	cout << "Implication matrix predicate: " << endl;
	cout << "{";
	for (size_t i = 0; i < matrix.size(); i++)
	{
		if (i != 0)
		{
			cout << " ";
		}
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			string variable = matrix[i][j].first;
			float value = matrix[i][j].second;
			cout << "<<" << variable << ">, " << fixed << setprecision(1) << value << ">";
			if (j != matrix[i].size() - 1)
			{
				cout << ",   ";
			}
		}
		if (i != matrix.size() - 1)
		{
			cout << endl;
		}
	}
	cout << "}" << endl << endl;
}

inline float t_norm(float x, float y) {
	return min(x, y);
}

inline impl_matrix build_implementation_matrix(set set_x, set set_y) {
	impl_matrix result;
	for (size_t i = 0; i < set_x.second.size(); i++)
	{
		arr array;
		float x = set_x.second[i].second;
		for (size_t j = 0; j < set_y.second.size(); j++)
		{
			float y = set_y.second[j].second;
			string vars = set_x.second[i].first + ", " + set_y.second[j].first;
			array.push_back(make_pair(vars, implication(x, y)));
		}
		result.push_back(array);
	}
	return result;
}

inline string anti_element_of_array(arr array) {
	string result;
	if (array[0].first.find('x') == string::npos)
	{
		result = "x";
	}
	else
	{
		result = "y";
	}
	return result;
}

inline arr calculate_direct_output(impl_matrix matrix, arr package) {
	arr result;
	for (size_t i = 0; i < matrix[0].size(); i++)
	{
		float value = 0;
		for (size_t j = 0; j < matrix.size(); j++)
		{
			float y = matrix[j][i].second;
			value = max(value, t_norm(package[j].second, y));
		}
		string variable = anti_element_of_array(package) + to_string(i + 1);
		result.push_back(make_pair(variable, value));
	}
	return result;
}