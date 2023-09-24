#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;
    ~MovieDatabase();

  private:
      bool m_loaded;
      TreeMultimap<std::string, Movie*> m_movies_to_id;
      TreeMultimap<std::string, Movie*> m_movies_to_director;
      TreeMultimap<std::string, Movie*> m_movies_to_actor;
      TreeMultimap<std::string, Movie*> m_movies_to_genre;
      std::vector<Movie*> m_movies_pointers; // vector storing Movie pointers to be destructed

      std::string lower(std::string s) const;
};

#endif // MOVIEDATABASE_INCLUDED
