#include "UserDatabase.h"
#include "User.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include <cassert>
#include <chrono>

// new stuff
#include "treemm.h"

using namespace std;


class Timer
{
public:
	Timer()
	{
		start();
	}
	void start()
	{
		m_time = std::chrono::high_resolution_clock::now();
	}
	double elapsed() const
	{
		std::chrono::duration<double, std::milli> diff =
			std::chrono::high_resolution_clock::now() - m_time;
		return diff.count();
	}
private:
	std::chrono::high_resolution_clock::time_point m_time;
};

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

class Dog
{
public:
	Dog(string name) { m_name = name; }
	string m_name;
};

void testTree()
{
	TreeMultimap<std::string, int> tmm;
	TreeMultimap<std::string, int>::Iterator it = tmm.find("carey");
	assert(!it.is_valid());
	tmm.insert("carey", 5);
	tmm.insert("carey", 6);
	tmm.insert("carey", 7);
	tmm.insert("david", 425);
	tmm.insert("david", 25);
	tmm.insert("jason", 3);
	it = tmm.find("Carey");
	assert(!it.is_valid());

	it = tmm.find("carey");
	assert(it.get_value() == 5);
	it.advance();
	assert(it.get_value() == 6);
	it.advance();
	assert(it.is_valid());
	assert(it.get_value() == 7);
	it.advance();
	assert(!it.is_valid());

	it = tmm.find("david");
	assert(it.get_value() == 425);

	it = tmm.find("jason");
	assert(it.get_value() == 3);


	vector<string> watchHistory;
	watchHistory.push_back("ID1234");
	watchHistory.push_back("ID1251");
	TreeMultimap<std::string, vector<string>> tmm2;
	tmm2.insert("hello", watchHistory);
	TreeMultimap<std::string, vector<string>>::Iterator it2 = tmm2.find("hello");
	assert(it2.is_valid());
	assert(it2.get_value() == watchHistory);
	watchHistory.push_back("234142");
	assert(it2.get_value() != watchHistory);

	TreeMultimap<string, int> tmm1;
	tmm1.insert("hi", 1);
	tmm1.insert("hello", 2);
	it = tmm1.find("hi");
	assert(it.is_valid());
	assert(it.get_value() == 1);
	tmm1.insert("hi", 3);
	it.advance();
	assert(it.get_value() == 3);

	TreeMultimap<int, string> tm;
	tm.insert(1, "hi1");
	tm.insert(2, "hi2");
	tm.insert(3, "hi3");
	tm.insert(4, "hi4");
	tm.insert(5, "hi5");
	tm.insert(-1, "hi-1");
	tm.insert(-2, "hi-2");
	tm.insert(-3, "hi-3");
	tm.insert(-4, "hi-4");
	tm.insert(1, "second_hi1");

	Dog* d1 = new Dog("Koda");
	Dog* d2 = new Dog("Spot");
	TreeMultimap<std::string, Dog*>* tmmptr = new TreeMultimap<std::string, Dog*>();
	tmmptr->insert("carey", d1);
	tmmptr->insert("cindy", d2);
	delete tmmptr; // TreeMultimap's destructor runs
	// It must not free the dog objects, just tree nodes
	// User is responsible for freeing any dynamically allocated
	// objects that were passed to insert to be added to the tree
	cerr << d1->m_name;
	delete d1;
	delete d2;

	TreeMultimap<std::string, int> tmm5;
	tmm5.insert("carey", 5);
	tmm5.insert("carey", 6);
	tmm5.insert("carey", 7);
	tmm5.insert("david", 25);
	tmm5.insert("david", 425);
	it = tmm5.find("carey");
	// prints 5, 6, and 7 in some order
	int i = 0;
	while (it.is_valid() && i < 3) {
		std::cout << it.get_value() << std::endl;
		it.advance();
		i++;
	}

	cout << "passed tree tests" << endl; 
}

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations) 
{
	Timer t;
	vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
	double time = t.elapsed();
	if (recommendations.empty())
		cout << "We found no movies to recommend :(.\n";
	else {
		for (int i = 0; i < recommendations.size(); i++) 
		{
			const MovieAndRank& mr = recommendations[i];
			Movie* m = md.get_movie_from_id(mr.movie_id);

			cout << i + 1 << ". " << m->get_title() << " ("
				<< m->get_release_year() << ")\n   Rating: "
				<< m->get_rating() << "\n   Compatibility Score: "
				<< mr.compatibility_score << "\n";
		}
	}
	cerr << time << endl;
}

int main()
{

	//testTree();

	UserDatabase testubd;
	testubd.load("users.txt");


	MovieDatabase testmbd;
	testmbd.load("movies.txt");

	Recommender r(testubd, testmbd);

	string email;
	cout << "enter email: ";
	getline(cin, email);
	findMatches(r, testmbd, email, 15);

	//for (;;)
	//{
	//	cout << "enter movie director to search for: ";
	//	string s;
	//	getline(cin, s);
	//	vector<Movie*> m = testmbd.get_movies_with_director(s);
	//	if (m.size() == 0)
	//		cout << "no movies found" << endl;
	//	else
	//	{
	//		for (int i = 0; i < m.size(); i++)
	//			cout << m[i]->get_title() << endl;
	//	}
	//	cout << "enter movie actor to search for: ";
	//	getline(cin, s);
	//	vector<Movie*> m2 = testmbd.get_movies_with_actor(s);
	//	if (m2.size() == 0)
	//		cout << "no movies found" << endl;
	//	else
	//	{
	//		for (int i = 0; i < m2.size(); i++)
	//			cout << m2[i]->get_title() << endl;
	//	}
	//	cout << "enter movie genre to search for: ";
	//	getline(cin, s);
	//	vector<Movie*> m3 = testmbd.get_movies_with_genre(s);
	//	if (m3.size() == 0)
	//		cout << "no movies found" << endl;
	//	else
	//	{
	//		for (int i = 0; i < m3.size(); i++)
	//			cout << m3[i]->get_title() << endl;
	//	}
	//	cout << "enter movie id: ";
	//	string id;
	//	getline(cin, id);
	//	Movie* m1 = testmbd.get_movie_from_id(id);
	//	if (m1 == nullptr)
	//		cerr << "Movie not Found" << endl;
	//	else
	//		cerr << m1->get_title() << endl;
	//}
	
	//UserDatabase udb;
	//if (!udb.load(USER_DATAFILE))  // In skeleton, load always return false
	//{
	//	cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
	//	return 1;
	//}
	//for (;;)
	//{
	//	cout << "Enter user email address (or quit): ";
	//	string email;
	//	getline(cin, email);
	//	if (email == "quit")
	//		return 0;
	//	User* u = udb.get_user_from_email(email);
	//	if (u == nullptr)
	//		cout << "No user in the database has that email address." << endl;
	//	else
	//	{
	//		cout << "Found " << u->get_full_name() << endl;
	//		cout << "Found " << u->get_email() << endl;
	//		cout << "Watch History Size " << u->get_watch_history().size() << endl;
	//		for (int i = 0; i < u->get_watch_history().size(); i++)
	//		{
	//			cout << u->get_watch_history()[i] << endl;
	//		}
	//	}
	//}
}
