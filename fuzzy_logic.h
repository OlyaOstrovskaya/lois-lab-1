/* Лабораторная работа 1 по дисциплине ЛОИС
Выполнена студентами группы 921702 БГУИР
Мурашко И.А. и Островская О.В. - разработали программу, в которой отображён алгоритм
Пыск Д.А. и Медведева М.С. - произвели анализ и отобразили результат этого в отчёте
Вариант 24 - Реализовать прямой нечеткий логический вывод, используя импликацию на основе драстического произведения
05.12.2022
Использованные материалы:
"Нечеткий логический вывод в системе принятия решений" А. А. Ахрем, М. Р. Ашинянц. С. А. Петров */

#include <iostream>
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

inline void valid_substring(string str) {
	if (str.empty() || str.find(')') != string::npos || str.find_last_of('(') != 0 || str.find_first_of(',') != str.find_last_of(','))
	{
		throw exception("");
	}
}

inline void valid_arr_val(float arr_val) {
	if (arr_val < 0 || arr_val > 1)
	{
		throw exception("");
	}
}


inline arr parse_arr_out_of_string(string str) {
	str = str.substr(str.find('=') + 2);
	arr temp_arr;
	while (!str.empty())
	{
		const size_t start = str.find_first_of('(');
		const size_t end = str.find_first_of(')');
		const size_t middle_comma = str.find_first_of(',');

		string temp = str.substr(start, end);
		valid_substring(temp);
		string arr_var = temp.substr(start + 1, middle_comma - start - 1);
		float arr_val = stof(temp.substr(middle_comma + 1, end - middle_comma));
		valid_arr_val(arr_val);
		pair<string, float> temp_pair = make_pair(arr_var, arr_val);
		temp_arr.push_back(temp_pair);
		str = str.substr(end + 2);
	}
	return temp_arr;
}

inline set parse_set_out_of_string(string str) {
	const size_t i = str.find('=');
	string set_name = str.substr(0, 1);
	arr temp_arr = parse_arr_out_of_string(str);
	set parsed_set(set_name, temp_arr);
	return parsed_set;
}

inline fuzzy_sets parse_fuzzy_sets_from_file(vector<string> strings) {
	fuzzy_sets sets;

	for (size_t i = 0; i < strings.size(); i++)
	{
		set read_set = parse_set_out_of_string(strings[i]);
		sets.push_back(read_set);
	}
	return sets;
}

inline set find_by_name(fuzzy_sets sets, string name) {
	set rez;
	for (size_t i = 0; i < sets.size(); i++)
	{
		if (sets[i].first == name)
		{
			rez = sets[i];
			break;
		}
	}
	if (rez.first.empty())
	{
		throw invalid_argument("");
	}
	return rez;
}

inline void show_sets(fuzzy_sets sets) {
	for (int i = 0; i < sets.size(); i++)
	{
		cout << sets[i].first << "={";
		for (size_t j = 0; j < sets[i].second.size(); j++)
		{
			if (j != 0)
			{
				cout << ",";
			}
			cout << "(" << sets[i].second[j].first << ",";
			cout << sets[i].second[j].second << ")";
		}
		cout << "}" << endl;
	}
}


inline float lim(float n) {
	double s = 0, q = 1;
	for (int i = 1; i <= n; i++) {
		q = q * 2;
		s += (2 * i - 1) / q;
	}
	return s;
}

inline float implication(float x, float y) {
	float z = rand();
	float res = min((float) 1, (float) lim(exp(z * (log(y)))) + (float) y * exp(z * (log(x))));
	return res;
}

inline void show_impl_matrix(impl_matrix matrix) {
	cout << "Implementation matrix predicate: " << endl;
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			string var = matrix[i][j].first;
			float val = matrix[i][j].second;
			cout << var << ": " <<val << "\t";
		}
		cout << endl;
	}
}

inline float tnorm(float x, float y) {
	return min(x, y);
}

inline impl_matrix fill_implementation_matrix(set set_x, set set_y) {
	impl_matrix matrix;
	for (size_t i = 0; i < set_x.second.size(); i++)
	{
		arr temp_arr;
		float x = set_x.second[i].second;
		for (size_t j = 0; j < set_y.second.size(); j++)
		{
			float y = set_y.second[j].second;
			string vars = set_x.second[i].first + ", " + set_y.second[j].first;
			temp_arr.push_back(make_pair(vars, implication(x, y)));
		}
		matrix.push_back(temp_arr);
	}
	return matrix;
}

inline string anti_element_of_arr(arr in_arr) {
	string anti;
	if (in_arr[0].first.find('x') == string::npos)
	{
		anti = "x";
	}
	else
	{
		anti = "y";
	}
	return anti;
}

inline arr compute_direct_output(impl_matrix matrix, arr package) {
	arr rez_arr;
	for (size_t i = 0; i < matrix[0].size(); i++)
	{
		float val = 0;
		for (size_t j = 0; j < matrix.size(); j++)
		{
			float y = matrix[j][i].second;
			val = max(val, tnorm(package[j].second, y));
		}
		string var = anti_element_of_arr(package) + to_string(i + 1);
		rez_arr.push_back(make_pair(var, val));
	}
	return rez_arr;
}