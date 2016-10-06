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
	double deadline;    // the moment after which event losts its value
	EventStatus status;

	static double max_deadline;

	Event(double profit, double deadline)
		: profit(profit),
		  deadline(deadline),
		  status(NONE)
	{ }
};

double Event::max_deadline = 0;

std::ostream& operator<<(std::ostream& os, const Event& e)
{
	os << "[ p: " << e.profit << ", t: " << e.deadline << ", st: ";
	switch (e.status) {
		case SCHEDULED:
			os << "+";
			break;
		case REJECTED:
			os << "x";
			break;
		default:
			os << "_";
	}
	os << " ]";

	return os;
}

bool compare_profit(const Event& a, const Event& b)
{
	if (a.deadline > Event::max_deadline) {
		Event::max_deadline = a.deadline;
	}
	if (b.deadline > Event::max_deadline) {
		Event::max_deadline = b.deadline;
	}

	if (a.profit > b.profit)
		return true;
	else if (a.profit == b.profit
	         && a.deadline < b.deadline)
		return true;

	return false;
}

bool compare_deadlines(const Event& a, const Event& b)
{
	return a.deadline < b.deadline;
}

template <typename iter>
void make_schedule(iter it_begin, iter it_end, double init_time, double duration, int rem_events)
{
	if (rem_events <= 0)
		return;

	auto last = it_begin + rem_events;
	if (last > it_end)
		last = it_end;

	// sort by deadlines
	std::sort(it_begin, last, compare_deadlines);

	//std::cout << "time segment: [" << init_time << ", " << init_time + duration << ")\n";
	//std::cout << "sorted by deadlines (" << rem_events << " left):\n";
	auto out = it_begin;
	//while (out != last) {
		//std::cout << *out << "\n";
		//++out;
	//}


	// split events into segments according to remaining events

	auto it = it_begin;
	// skip expired events
	while (it->deadline < init_time
	       && it != it_end)
	{
		it->status = REJECTED;
		++it;
	}

	//if (it != it_begin) {
		//out = it_begin;
		//std::cout << "expired: \n";
		//while (out != it) {
			//std::cout << *out << std::endl;
			//++out;
		//}
	//}

	if (it == it_end)
		return;

	auto segment_begin = it;
	auto segment_end = segment_begin + rem_events;
	while (it->deadline >= init_time
	       && it->deadline < init_time + duration
	       && it != it_end && it != segment_end)
	{
		it->status = REJECTED;
		++it;
	}


	auto max = segment_begin;
	if (it != segment_begin) {
		//std::cout << "found segment:\n";
		//out = segment_begin;
		//while (out != it) {
			//std::cout << *out << "\n";
			//++out;
		//}

		max = std::min_element(segment_begin, it, compare_profit);
	}
	else {
		//std::cout << "segment not found, choosing the nearest\n";
		++it;
	}

	max->status = SCHEDULED;
	out = it_begin;
	//std::cout << "scheduled\n";
	//while (out != it_end) {
		//std::cout << *out << "\n";
		//++out;
	//}
	//std::cout << "\n\n";

	if (it == it_end)
		return;

	make_schedule(it, it_end, init_time + duration, duration, rem_events-1);
}

template <typename C>
std::vector<Event> make_schedule(C& events, double duration)
{
	for (auto& x: events)
		x.status = NONE;

	// sort by profit (compare_profit stores max deadline in Event::max_deadline)
	std::sort(begin(events), end(events), compare_profit);
	unsigned int max_events = Event::max_deadline / duration + 1;
	if (max_events > events.size())
		max_events = events.size();

	//std::cout << "duration = " << duration << "\n";
	//std::cout << "Event::max_deadline = " << Event::max_deadline << "\n";
	//std::cout << "max events: " << max_events << "\n";
	//std::cout << "sorted by profit:\n";
	//for (auto& e: events) {
		//std::cout << e << "\n";
	//}
	//std::cout << "\n\n";

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
		f >> e.deadline;
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
	int total = 0;
	if(argc == 2) {
		events = read_file(argv[1]);
	}
	else if (argc == 1) {
		//events = {
			//Event { 98, 3.01 },
			//Event { 89, 3.01 },
			//Event { 83, 3.01 },
			//Event { 73, 3.01 },
			//Event { 71, 3.01 },
			//Event { 69, 3.01 },
			//Event { 64, 9.31 },
			//Event { 56, 2.36 },
			//Event { 46, 1.08 },
			//Event { 42, 5.95 }
		//};
		events = gen_data(15);
		cout << "generated events: \n";
		for (auto& e: events) {
			total += e.profit;
			cout << e << endl;
		}
		cout << "Total = " << total << "\n\n";
	}
	else {
		cout << "Usage: " << argv[0] << " [FILE]\n";
		return 1;
	}

	double duration = rand()%100/100.0 + 2;
	cout << "duration: " << duration << "\n\n";
	//double duration = 2.63;
	auto events_copy(events);
	auto schedule = make_schedule(events_copy, duration);

	cout << "RESULT:\n";
	cout << "max deadline: " << Event::max_deadline << "\n";
	cout << "max events " << int(Event::max_deadline / duration + 1) << "\n\n";


	//cout << "After scheduling:\n";
	//for (auto& e: events_copy) {
		//total += e.profit;
		//cout << e << endl;
	//}
	//cout << endl;


	total = 0;
	cout << "schedule: \n";
	for (auto& e: schedule) {
		total += e.profit;
		cout << e << endl;
	}
	cout << "Total profit = " << total << "\n\n";

	system("sleep 1");

	return 0;
}
