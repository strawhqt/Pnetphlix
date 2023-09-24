#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;
    ~UserDatabase();

  private:
      TreeMultimap<std::string, User*> m_user_database;
      std::vector<User*> m_users_pointers; // vector of Users to be destructed
      bool m_loaded;
};

#endif // USERDATABASE_INCLUDED
