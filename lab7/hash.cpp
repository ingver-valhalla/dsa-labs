// hash.c

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <set>
#include <cmath>
using namespace std;

struct Address{
	std::string street;
	int building;

	bool operator<(const Address& a) const
	{
		return this->street < a.street
			   || this->building < a.building;
	}
};

std::ostream& operator<<(std::ostream& os, Address addr)
{
	os << addr.street << ", " << addr.building;
	return os;
}

std::set<Address> addresses;

int hash_int1(int i, int M)
{
	return (int)(0.616161 * (float)i) % M;
}

int hash_int2(int i, int M)
{
	return i & (M-1);
}

int hash_int3(int i, int M)
{
	return (16161 * (unsigned) i) % M;
}

int hash_int4(int i, int M)
{
	return (31415 * (unsigned) i) % M;
}

typedef int (*hintf)(int, int);
hintf hash_int = hash_int4;

int hash_addr(Address addr, int M)
{
	int hash = 0, a = 31415, b = 27183;
	for (size_t i = 0;
	     i < addr.street.size();
	     a = a*b % (M-1), ++i)
	{
		hash = (a * hash + (unsigned char)addr.street[i]) % M;
	}
	hash = (a * hash + hash_int(addr.building, M)) % M;
	return hash;
}

Address gen_addr(const std::vector<std::string>& streets)
{
	Address addr;

	do {
		addr.street = streets[rand()%streets.size()];
		addr.building = rand() % 500;
	} while (addresses.find(addr) != addresses.end());

	addresses.insert(addr);

	return addr;
}

double chi_square(std::vector<int> stats, int M, int keys)
{
	double sum = 0;
	for (int i = 0; i < M; ++i) {
		sum += pow(stats[i] - (double)keys / M, 2.0);
	}
	return ((double)M / keys) * sum;
}

void test_hash_quality(const std::vector<std::string>& streets, const int table_size, const int keys)
{
	const int TESTS = 1000;
	double chi2 = 0;
	std::vector<int> stats;
	int hit = 0;
	double chi2_lower_bound = table_size - sqrt(table_size);
	double chi2_upper_bound = table_size + sqrt(table_size);
	double theor_probability = 1 - 1 / ((double)keys / table_size);
	double frequency = 0;

	for (int i = 0; i < TESTS; ++i) {
		addresses.clear();
		stats = std::vector<int>(table_size);
		for (int j = 0; j < keys; ++j) {
			Address addr = gen_addr(streets);
			int h = hash_addr(addr, table_size);
			if (h < 0 || h >= table_size) {
				cout << "Invalid hash (" << h << ")\n";
				return;
			}
			++stats[h];
		}

		chi2 = chi_square(stats, table_size, keys);
		if (chi2_lower_bound <= chi2 && chi2 <= chi2_upper_bound) {
			++hit;
		}
	}
	frequency = (double)hit / TESTS;

	cout << "M - M^(1/2) = " << chi2_lower_bound << endl;
	cout << "M + M^(1/2) = " << chi2_upper_bound << endl;
	cout << "1 - 1/c = " << theor_probability << endl;
	cout << "frequency " << frequency << endl;
	if (theor_probability <= frequency) {
		cout << "hash satisfies \"chi squared\" critetria" << endl;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " INPUT_FILE\n";
		exit(1);
	}

	srand(time(NULL));

	vector<string> streets;
	const int TABLE_SIZE = 512;
	const int KEYS = 2000;
	static int stats[TABLE_SIZE];

	fstream f(argv[1], ios::in);
	if(!f) {
		cout << "Failed to open file" << argv[1] << endl;
		exit(1);
	}

	string s;
	while (getline(f, s)) {
		streets.push_back(s);
	}

	for (int i = 0; i < KEYS; ++i) {
		Address addr = gen_addr(streets);
		int h = hash_addr(addr, TABLE_SIZE);
		if (h < 0 || h >= TABLE_SIZE) {
			cout << "Invalid hash (" << h << ")\n";
			return 0;
		}
		//cout << "hash_addr(" << addr << ") = "
			 //<< h << endl;
		++stats[h];
	}

	fstream fdata("data.txt", ios::out);
	if(!fdata) {
		cout << "Can't open data.txt\n";
	}

	fdata << "stats:\n";
	//cout << "stats:\n";
	int min = KEYS, max = 0;
	for (int i = 0; i < TABLE_SIZE; ++i) {
		fdata << i << "," << stats[i] << endl;
		//cout << stats[i] << " ";
		if (stats[i] < min)
			min = stats[i];
		if (stats[i] > max)
			max = stats[i];
	}
	cout << "\n\n";


	test_hash_quality(streets, TABLE_SIZE, KEYS);
	
	system("sleep 1");

	return 0;
}
