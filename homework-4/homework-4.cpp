#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M>
class CContest {
public:
    CContest() = default;
    ~CContest() = default;

    enum Visited {
        NOT_VISITED,
        VISITED
    };

    CContest& addMatch(const string& player1, const string& player2, M result) {      
        for (const auto& match : m_Matches) {
            if ((player1 == match.first && player2 == match.second.first) ||
                (player2 == match.first && player1 == match.second.first)) {
                throw logic_error("Tento hra jiz pridana");
            }
        }

        m_Matches.emplace_back(make_pair(player1, make_pair(player2, result)));
        return *this;
    }

    template <typename Comparator>
    bool isOrdered(const Comparator& compare) const {      
        return processOrder(compare, false).first;
    }

    template <typename Comparator>
    list<string> results(const Comparator& compare) const {
        auto [validOrder, result] = processOrder(compare, true);
        if (!validOrder) {
            throw logic_error("Spatny el");
        }
        return result;
    }

private:
    vector<pair<string, pair<string, M>>> m_Matches;

    template <typename Comparator>
    pair<bool, list<string>> processOrder(const Comparator& compare, bool generateList) const {        
        queue<string> toVisit;
        map<string, Visited> visited;
        map<string, set<string>> DAG;        
        
        int count = 0;
        list<string> resList;

        for (const auto& match : m_Matches) {
            const auto& player1 = match.first;
            const auto& player2 = match.second.first;
            const auto& result = match.second.second;

            if (compare(result) > 0) {
                DAG[player1].insert(player2);
                visited[player1] = NOT_VISITED;
                visited[player2] = NOT_VISITED;
            } else if (compare(result) < 0) {
                DAG[player2].insert(player1);
                visited[player1] = NOT_VISITED;
                visited[player2] = NOT_VISITED;
            } else {
                return {false, resList};
            }
        }

        for (const auto& [key, value] : DAG) {
            for (const auto& v : value) {
                if (visited[v] == NOT_VISITED) {
                    visited[v] = VISITED;
                } else {
                    visited[v] = static_cast<Visited>(visited[v] + 1);
                }
            }
        }

        for (const auto& [key, value] : visited) {
            if (value == NOT_VISITED) {
                toVisit.push(key);
            }
        }

        if (toVisit.size() > 1) {
            return {false, resList};
        }

        while (!toVisit.empty()) {
            string current = toVisit.front();
            toVisit.pop();
            if (generateList) {
                resList.push_back(current);
            }

            for (auto& itr : DAG[current]) {
                visited[itr] = static_cast<Visited>(visited[itr] - 1);
                if (visited[itr] == NOT_VISITED) {
                    toVisit.push(itr);
                }
            }

            if (toVisit.size() > 1) {
                return {false, resList};
            }
            count++;
        }

        bool validOrder = (count == static_cast<int>(DAG.size()));
        return {validOrder, resList};
  }
};

#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int                main                                    ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
