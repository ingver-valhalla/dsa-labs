#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::vector<double>& cont)
{
	os << "{ ";
	for (auto& x: cont)
		os << x << " ";
	os << "}";
	return os;
}

double s(double a, double b, double c)
{
	// expression based on triangle area formula
	// (got rid of division and sqrt):
	//     S = sqrt( p * (p - a) * (p - b) * (p - c) ),
	//       where p = (a + b + c) / 2
	return -(pow(a, 4) + pow(b, 4) + pow(c,4))
	       + 2 * (a*a*b*b + b*b*c*c + a*a*c*c);
}

std::vector<double> get_biggest_area_segments(std::vector<double>& segments)
{
	if (segments.size() < 3)
		return std::vector<double>();

	std::sort(segments.begin(), segments.end(), std::greater<double>());
	std::vector<double> result(3);
	std::vector<double> temp(3);
	double biggest_area = 0;

	for (int i = 0; i < 3; ++i)
		temp[i] = segments[i];
	
	//std::cout << "biggest 3 segments:\n";
	//std::cout << temp << "\n";

	auto it = segments.begin() + 3;

	while (it != segments.end()) {
		// check triangle inequality:
		// a < b + c
		if (temp[0] < temp[1] + temp[2]) {
			double area = s(temp[0], temp[1], temp[2]);
			if (biggest_area < area) {
				biggest_area = area;
				result = temp;
			}
		}
		temp[0] = temp[1];
		temp[1] = temp[2];
		temp[2] = *it;
		++it;
	} //while (it != segments.end());

	return result;
}

std::vector<double> stupid_method(std::vector<double>& segments)
{
	if (segments.size() < 3)
		return std::vector<double>();
	//std::cout << "Stupid method:\n";
	std::sort(segments.begin(), segments.end(), std::greater<double>());

	double max_area = 0;
	std::vector<double> result;
	std::vector<double> triplet;
	double a, b, c;

	for (auto it1 = segments.begin(); it1 < segments.end()-2; ++it1) {
		a = *it1;
		for (auto it2 = it1+1; it2 < segments.end()-1; ++it2) {
			b = *it2;
			for (auto it3 = it2 + 1; it3 < segments.end(); ++it3) {
				c = *it3;
				triplet = {a, b, c};

				//std::cout << triplet;

				if (a >= b + c) {
					//std::cout << " !!! denied\n";
					continue;
				}

				double area = s(a,b,c);
				//std::cout << " area = " << area << "\n";

				if (max_area < area) {
					max_area = area;
					result = triplet;
				}
			}
		}
	}

	//std::cout << "max_area = " << max_area << "\n";

	return result;
}

using namespace std;

int main()
{
	srand(time(NULL));
	//vector<double> vec {
		//3, 8, 5, 9, 1, 2, 6
	//};

	while(1) {
		cout << "********************************************************\n";
		vector<double> vec(10);
		for (int i = 0; i < 10; ++i)
			vec[i] = rand()%1000/100.0;

		cout << "segments generated:\n";
		cout << vec << "\n";

		auto stupid = stupid_method(vec);
		cout << "solution found with naive method:\n";
		cout << stupid << " (s = " << s(stupid[0], stupid[1], stupid[2]) << ")\n";


		bool success = false;
		std::vector<double> temp(3);
		for (int i = 0; i < 3; ++i)
			temp[i] = vec[i];
		if (temp[0] < temp[1] + temp[2])
			success = true;

		
		std::cout << "biggest 3 segments:\n";
		std::cout << temp << " (s = " << s(temp[0], temp[1], temp[2]) << ")\n";

		auto result = get_biggest_area_segments(vec);
		if (result.size() > 0)
			cout << "result = " << result << " (s = " << s(result[0], result[1], result[2]) << ")\n";
		else
			cout << "result is empty\n";

		if (temp != result && success)
			break;
		
		//cin.get();
	}

	return 0;
}
