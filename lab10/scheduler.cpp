#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>

enum EventStatus {
	NONE,
	SCHEDULED,
	REJECTED
};

struct Event {
	int profit;         // how much profit does event bring
	double time;        // the moment after which event losts its value
	EventStatus status;

	static double max_time;

	Event(double profit, double time)
		: profit(profit),
		  time(time),
		  status(NONE)
	{ }
};

double Event::max_time = 0;

std::ostream& operator<<(std::ostream& os, const Event& e)
{
	os << "[ p: " << e.profit << ", t: " << e.time << ", st: ";
	switch (e.status) {
		case SCHEDULED:
			os << "+";
			break;
		case REJECTED:
			os << "-";
			break;
		default:
			os << "_";
	}
	os << " ]";

	return os;
}

bool compare_profit(const Event& a, const Event& b)
{
	if (a.time > Event::max_time) {
		Event::max_time = a.time;
	}
	if (b.time > Event::max_time) {
		Event::max_time = b.time;
	}

	if (a.profit > b.profit)
		return true;
	else if (a.profit == b.profit
	         && a.time < b.time)
		return true;

	return false;
}

bool compare_times(const Event& a, const Event& b)
{
	if (a.time < b.time)
		return true;

	return false;
}

template <typename iter>
void make_schedule(iter begin, iter end, double init_time, double duration, int max_events)
{
	if (max_events <= 0)
		return;

	auto rest = begin + max_events;
	// sort by times
	std::sort(begin,
	          rest < end ? rest : end,
	          compare_times);

	std::cout << "sorted by times (" << max_events << " left):\n";
	auto out = begin;
	//while (out != end) {
		//std::cout << *out << "\n";
		//++out;
	//}
	//std::cout << "\n\n";

	// split events into segments according to max_events

	auto it = begin;
	// skip expired events
	while (it->time < init_time
	       && it != end)
	{
		it->status = REJECTED;
		++it;
	}

	auto segment_begin = it;
	auto segment_end = segment_begin + max_events;
	while (it->time >= init_time
	       && it->time < init_time + duration
	       && it != end && it != segment_end)
	{
		it->status = REJECTED;
		++it;
	}


	auto max = segment_begin;
	if (it != segment_begin) {
		std::cout << "found segment [" << init_time << ", " << init_time + duration << "):\n";
		out = segment_begin;
		while (out != it) {
			std::cout << *out << "\n";
			++out;
		}

		max = std::min_element(segment_begin, it, compare_profit);
	}
	else {
		std::cout << "segment not found, choosing the nearest\n";
		++it;
	}

	max->status = SCHEDULED;
	out = begin;
	std::cout << "scheduled\n";
	while (out != end) {
		std::cout << *out << "\n";
		++out;
	}
	std::cout << "\n\n";

	if (it == end)
		return;

	make_schedule(it, end, init_time + duration, duration, max_events-1);
}

template <typename C>
std::vector<Event> make_schedule(C& events, double duration)
{
	for (auto& x: events)
		x.status = NONE;

	// sort by profit (compare_profit stores max time in Event::max_time)
	std::sort(begin(events), end(events), compare_profit);
	unsigned int max_events = Event::max_time / duration + 1;
	if (max_events > events.size())
		max_events = events.size();

	std::cout << "duration = " << duration << "\n";
	std::cout << "Event::max_time = " << Event::max_time << "\n";
	std::cout << "max events: " << max_events << "\n";
	std::cout << "sorted by profit:\n";
	for (auto& e: events) {
		std::cout << e << "\n";
	}
	std::cout << "\n\n";

	make_schedule(events.begin(), events.end(), 0, duration, max_events);

	std::vector<Event> schedule;
	for (auto& x: events)
		if (x.status == SCHEDULED)
			schedule.push_back(x);

	return schedule;
}

std::vector<Event> read_file(std::string fname)
{
	std::vector<Event> data;
	std::fstream f(fname, std::ios::in);
	if (!f) {
		std::cout << "failed to open file " << fname << "\n";
		return std::vector<Event>();
	}

	Event e(0, 0);
	while (!f.eof()) {
		f >> e.profit;
		if (!f) {
			return std::vector<Event>();
		}
		f >> e.time;
		if (!f) {
			return std::vector<Event>();
		}
		while(f.get() != '\n');
		data.push_back(e);
	}

	return data;
}

std::vector<Event> gen_data(int count)
{
	std::vector<Event> data;
	while(count--) {
		data.push_back(Event(rand()%100, (double)(rand()%1000) / 100));
	}
	return data;
}

using namespace std;

int main(int argc, char** argv)
{
	srand(time(NULL));
	vector<Event> events;
	if(argc == 2) {
		events = read_file(argv[1]);
	}
	else if (argc == 1) {
		//events = {
			//Event { 13, 81.3 },
			//Event { 20, 88.9 },
			//Event { 92, 58.5 },
			//Event { 71, 19.5 },
			//Event { 49, 10.4 },
			//Event { 11, 63.9 },
			//Event { 98, 43.6 },
			//Event { 41, 99.2 },
			//Event { 44, 42.1 },
			//Event { 65, 17.3 }
		//};
		events = gen_data(10);
	}
	else {
		cout << "Usage: " << argv[0] << " [FILE]\n";
		return 1;
	}

	int total = 0;
	for (auto& e: events) {
		total += e.profit;
		cout << e << endl;
	}
	cout << "Total = " << total << "\n\n";

	auto schedule = make_schedule(events, rand()%200/100.0);

	total = 0;
	cout << "RESULT:\n";
	for (auto& e: schedule) {
		total += e.profit;
		cout << e << endl;
	}
	cout << "Total profit = " << total << "\n\n";

	system("sleep 1");

	return 0;
}
