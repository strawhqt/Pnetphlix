#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    m_loaded = false; // database is unloaded in the beginning
}

bool UserDatabase::load(const string& filename) 
{
    if (m_loaded)
        return false;
    m_loaded = true;

    ifstream infile(filename);
    if (!infile)
        return false;

    // Use following placeholder variables to store information for each user
    string name, email, movie_id, empty; 
    int movie_count;
    vector<string> movies;

    while (getline(infile, name)) // while can continue reading in new names
    {
        getline(infile, email);
        infile >> movie_count;
        getline(infile, empty);
        for (int i = 0; i < movie_count; i++) // iterate through all the movies for each user
        {
            getline(infile, movie_id);
            movies.push_back(movie_id);
        }
        getline(infile, empty);
        User* user = new User(name, email, movies); // create User based on given info
        m_user_database.insert(email, user);
        m_users_pointers.push_back(user);
        movies.clear(); // clear movie vector so each user starts with empty vector
    }
    return true;
}

UserDatabase::~UserDatabase()
{
    // loop through and delete all Users in the database
    for (int i = 0; i < m_users_pointers.size(); i++)
    {
        delete m_users_pointers[i];
        m_users_pointers[i] = nullptr;
    }
}


User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = m_user_database.find(email);
    if (!it.is_valid())
        return nullptr;
    else
        return it.get_value();
}
