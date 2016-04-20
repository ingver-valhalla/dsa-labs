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

std::vector<std::string> streets;
std::set<Address> addresses;

bool addr_eq(Address addr1, Address addr2)
{
	return addr1.street == addr2.street
		   && addr1.building == addr2.building;
}

std::ostream& operator<<(std::ostream& os, Address addr)
{
	os << addr.street << ", " << addr.building;
	return os;
}

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
hintf hash_int = hash_int1;

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

Address gen_addr()
{
	static std::set<Address> addresses;
	Address addr;

	do {
		addr.street = streets[rand()%streets.size()];
		addr.building = rand() % 500;
	} while (addresses.find(addr) != addresses.end());

	addresses.insert(addr);

	return addr;
}

double hi_square(int* stats, int M, int keys)
{
	double sum = 0;
	for (int i = 0; i < M; ++i) {
		sum += pow(stats[i] - (double)keys / M, 2.0);
	}
	return ((double)M / keys) * sum;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " INPUT_FILE\n";
		exit(1);
	}

	srand(time(NULL));

	const int TABLE_SIZE = 511;
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
		Address addr = gen_addr();
		int h = hash_addr(addr, TABLE_SIZE);
		//cout << "hash_addr(" << addr << ") = "
			 //<< h << endl;
		++stats[h];
	}

	fstream fdata("data.txt", ios::out);
	if(!fdata) {
		cout << "Can't open data.txt\n";
	}

	fdata << "stats:\n";
	int min = KEYS, max = 0;
	for (int i = 0; i < TABLE_SIZE; ++i) {
		fdata << i << "," << stats[i] << endl;
		if (stats[i] < min)
			min = stats[i];
		if (stats[i] > max)
			max = stats[i];
	}
	cout << endl;

	cout << "max difference = " << max - min << "\n\n";

	cout << "hi^2 = " << hi_square(stats, TABLE_SIZE, KEYS) << endl;
	cout << "M - M^(1/2) = " << (TABLE_SIZE - sqrt(TABLE_SIZE)) << endl;
	cout << "M + M^(1/2) = " << (TABLE_SIZE + sqrt(TABLE_SIZE)) << endl;
	cout << "1 - 1/c = " << 1 - 1 / ((double)KEYS / TABLE_SIZE) << endl;
	
	system("sleep 1");

	return 0;
}
