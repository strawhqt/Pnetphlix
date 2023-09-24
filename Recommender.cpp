#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

bool comp(const pair<Movie*, int> a, const pair<Movie*, int> b);

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    m_udb = &user_database;
    m_mdb = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    if (movie_count <= 0)
        return vector<MovieAndRank>();
    
    User* user = m_udb->get_user_from_email(user_email);
    vector<string> watch_history = user->get_watch_history();
    unordered_set<string> watch_history_set; 

    // map of every movie to its compatibility score
    unordered_map<Movie*, int> movie_to_rank;

    for (int i = 0; i < watch_history.size(); i++)
    {
        // watch history set allows for fast searching if a user has watched a movie before
        watch_history_set.insert(watch_history[i]);
    }

    for (int i = 0; i < watch_history.size(); i++)
    {
        Movie* m = m_mdb->get_movie_from_id(watch_history[i]);

        // for every director, find all movies with that director that the user
        // hasn't watched and add to its compatibility score
        vector<string> directors = m->get_directors();
        for (int j = 0; j < directors.size(); j++)
        {
            vector<Movie*> movies_with_director = m_mdb->get_movies_with_director(directors[j]);
            for (int k = 0; k < movies_with_director.size(); k++)
            {
                if(watch_history_set.find(movies_with_director[k]->get_id()) != watch_history_set.end())
                    continue;
                movie_to_rank[movies_with_director[k]] += 20;
            }
        }

        // for every actor, find all movies with that actor that the user
        // hasn't watched and add to its compatibility score
        vector<string> actors = m->get_actors();
        for (int j = 0; j < actors.size(); j++)
        {
            vector<Movie*> movies_with_actor = m_mdb->get_movies_with_actor(actors[j]);
            for (int k = 0; k < movies_with_actor.size(); k++)
            {
                if (watch_history_set.find(movies_with_actor[k]->get_id()) != watch_history_set.end())
                    continue;
                movie_to_rank[movies_with_actor[k]] += 30;
            }
        }

        // for every genre, find all movies with that genre that the user
        // hasn't watched and add to its compatibility score
        vector<string> genres = m->get_genres();
        for (int j = 0; j < genres.size(); j++)
        {
            vector<Movie*> movies_with_genre = m_mdb->get_movies_with_genre(genres[j]);
            for (int k = 0; k < movies_with_genre.size(); k++)
            {
                if (watch_history_set.find(movies_with_genre[k]->get_id()) != watch_history_set.end())
                    continue;
                movie_to_rank[movies_with_genre[k]] += 1;
            }
        }
    }

    // copy all values from the map into a vector of pairs so that we can sort the vector
    // using our own comparison function
    vector<pair<Movie*, int>> copy;
    unordered_map<Movie*, int>::iterator it = movie_to_rank.begin();
    while(it != movie_to_rank.end())
    {
        if((*it).second >= 1)
            copy.push_back(*it);
        it++;
    }
    sort(copy.begin(), copy.end(), comp);
 
    vector<MovieAndRank> v;

    // add the first movie_count movies to the final vector and return it
    for(int i = 0; i < movie_count && i < copy.size(); i++)
        v.push_back(MovieAndRank(copy[i].first->get_id(), copy[i].second));

    return v;
}

// custom comparison function to compare two movies based on compatiblity score,
// then rating, then title
bool comp(const pair<Movie*, int> a, const pair<Movie*, int> b)
{
    if (a.second > b.second)
        return true;
    else if (a.second < b.second)
        return false;
    else
    {
        if (a.first->get_rating() > b.first->get_rating())
            return true;
        else if (a.first->get_rating() < b.first->get_rating())
            return false;
        else
            return a.first->get_title() < b.first->get_title();
    }
}