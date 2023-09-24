#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

MovieDatabase::MovieDatabase()
{
    m_loaded = false; // database is unloaded in the beginning
}

bool MovieDatabase::load(const string& filename)
{
    if (m_loaded)
        return false;
    m_loaded = true;

    ifstream infile(filename);
    if (!infile)
        return false;

    // Use following placeholder variables to store information for each movie
    string id, name, release_year, directors, actors, genres, temp;
    float rating;
    vector<string> vdirectors;
    vector<string> vactors;
    vector<string> vgenres;

    while (getline(infile, id)) // while can continue reading in id's
    {
        getline(infile, name);
        getline(infile, release_year);

        // Use stringstreams to read in directors, actors, and genres separated by commas
        getline(infile, directors);
        stringstream dir(directors);
        while (getline(dir, temp, ','))
        {
            vdirectors.push_back(lower(temp)); 
        }

        getline(infile, actors);
        stringstream act(actors);
        while (getline(act, temp, ','))
        {
            vactors.push_back(lower(temp));
        }

        getline(infile, genres);
        stringstream gen(genres);
        while (getline(gen, temp, ','))
        {
            vgenres.push_back(lower(temp));
        }

        infile >> rating;
        getline(infile, temp);
        getline(infile, temp);

        // create new Movie based on given information
        Movie* movie = new Movie(id, name, release_year, vdirectors, vactors, vgenres, rating);

        // add the movie to respective director, actor, and genre
        for (int i = 0; i < vdirectors.size(); i++)
            m_movies_to_director.insert(vdirectors[i], movie);
        for (int i = 0; i < vactors.size(); i++)
            m_movies_to_actor.insert(vactors[i], movie);
        for (int i = 0; i < vgenres.size(); i++)
            m_movies_to_genre.insert(vgenres[i], movie);
        m_movies_to_id.insert(lower(id), movie);
        m_movies_pointers.push_back(movie);

        // each movie starts off with empty directors, actors, genres
        vdirectors.clear();
        vactors.clear();
        vgenres.clear();
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie*>::Iterator it = m_movies_to_id.find(lower(id));
    if (!it.is_valid())
        return nullptr;
    else
        return it.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    TreeMultimap<string, Movie*>::Iterator it = m_movies_to_director.find(lower(director));
    vector<Movie*> v(0);
    while (it.is_valid())
    {
        v.push_back(it.get_value());
        it.advance();
    }
    return v;  
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    TreeMultimap<string, Movie*>::Iterator it = m_movies_to_actor.find(lower(actor));
    vector<Movie*> v(0);
    while (it.is_valid())
    {
        v.push_back(it.get_value());
        it.advance();
    }
    return v;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    TreeMultimap<string, Movie*>::Iterator it = m_movies_to_genre.find(lower(genre));
    vector<Movie*> v(0);
    while (it.is_valid())
    {
        v.push_back(it.get_value());
        it.advance();
    }
    return v;
}

MovieDatabase::~MovieDatabase()
{
    // delete all Movie pointers in the database
    for (int i = 0; i < m_movies_pointers.size(); i++)
    {
        delete m_movies_pointers[i];
        m_movies_pointers[i] = nullptr;
    }
}

string MovieDatabase::lower(string s) const
{
    // function to change all letters to lowercase in a string
    string lowercase_string = "";
    for (int i = 0; i < s.length(); i++)
    {
        lowercase_string += tolower(s[i]);
    }
    return lowercase_string;
}
