#ifndef __PROGTEST__
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <memory>
using namespace std;
struct CCoord
{
  public:
                             CCoord                        ( int               x = 0,
                                                             int               y = 0 )
      : m_X ( x ),
        m_Y ( y )
    {
    }
    int                      m_X;
    int                      m_Y;
};
#endif /* __PROGTEST__ */

class CCommon { 
    public: 
        CCommon() {} 
        CCommon(const CCommon &src); 
        virtual bool pointInside(int cordX, int cordY) = 0; 
        virtual CCommon* copieClass() const = 0; 
 
        int m_Id; 
};

class CRectangle : public CCommon { 
    public: 
        CRectangle ( int id, int cordX1, int cordY1, int cordX2, int cordY2 ) {     
            m_TopLeft.m_X = min(cordX1, cordX2); 
            m_TopLeft.m_Y  = min(cordY1, cordY2); 
            m_BottomRight.m_X = max(cordX1, cordX2); 
            m_BottomRight.m_Y = max(cordY1, cordY2);         
     
            m_Id = id; 
        } 
 
        CRectangle(const CRectangle &src) { 
            m_Id = src.m_Id; 
            m_TopLeft.m_X = src.m_TopLeft.m_X; 
            m_TopLeft.m_Y = src.m_TopLeft.m_Y; 
            m_BottomRight.m_X = src.m_BottomRight.m_X; 
            m_BottomRight.m_Y = src.m_BottomRight.m_Y; 
        }     
         
        bool pointInside(int cordX, int cordY){ 
            return m_TopLeft.m_X <= cordX && m_BottomRight.m_X >= cordX && m_TopLeft.m_Y <= cordY && m_BottomRight.m_Y >= cordY; 
        } 
 
        CCommon* copieClass() const { 
            return new CRectangle(*this); 
        } 
 
        CCoord m_TopLeft, m_BottomRight; 
 
};

class CCircle : public CCommon { 
    public: 
        CCircle(const CCircle &src) { 
            m_Id = src.m_Id; 
            m_Nucleus.m_X = src.m_Nucleus.m_X; 
            m_Nucleus.m_Y = src.m_Nucleus.m_Y; 
            m_Radius = src.m_Radius; 
        } 
 
        CCircle ( int id, int cordX, int cordY, int radius ) { 
            m_Id = id; 
            m_Nucleus.m_X = cordX; 
            m_Nucleus.m_Y = cordY; 
            m_Radius = radius;        
        }    
 
        CCommon* copieClass() const {return new CCircle(*this);} 
 
        bool pointInside(int cordX, int cordY) { 
            long int dx = m_Nucleus.m_X - cordX; 
            long int dy = m_Nucleus.m_Y - cordY; 
            return dx * dx + dy * dy <= m_Radius * m_Radius; 
        } 
 
 
        CCoord m_Nucleus; 
        long int m_Radius; 
 
};

class CPolygon : public CCommon { 
 
  public: 
    CPolygon(int id, const CCoord* begin, const CCoord* end) { 
        m_Id = id; 
        m_NumItems = 0;                     
        m_NumItems = distance(begin, end);  
        m_PonsVector.reserve(m_NumItems);         
        copy(begin, end, back_inserter(m_PonsVector)); 
    } 
 
    template<typename Iter> 
    CPolygon(int id, Iter begin, Iter end){ 
        m_Id = id; 
        m_NumItems = 0;     
        m_NumItems = distance(begin, end);     
        m_PonsVector.reserve(m_NumItems); 
        transform(begin, end, back_inserter(m_PonsVector),[](const auto& cordC) { return CCoord(cordC.m_X, cordC.m_Y); }); 
    } 
 
    template <typename Container> 
    CPolygon(int id, const Container& container){ 
        m_Id = id; 
        m_NumItems = 0; 
        m_NumItems = container.size(); 
        m_PonsVector.reserve(m_NumItems); 
        transform(container.begin(), container.end(), back_inserter(m_PonsVector), 
            [](const auto& cordC) { return CCoord(cordC->m_X, cordC->m_Y); }); 
    } 
 
    CPolygon(int id, const initializer_list<CCoord>& container) { 
        m_Id = id; 
        m_NumItems = 0;     
        m_NumItems = container.size();         
        m_PonsVector.reserve(m_NumItems);         
        transform(container.begin(), container.end(), back_inserter(m_PonsVector), 
                    [](const auto& cordC) { return CCoord(cordC.m_X, cordC.m_Y); }); 
    } 
 
    template< typename T, typename... CoordTypes > void variadicPushBack(const T & t, const CoordTypes&... сoordTypes) { 
        m_PonsVector.push_back(t); 
        m_NumItems++; 
        variadicPushBack(сoordTypes...); 
    } 
 
    template<typename IdType, typename... CoordTypes> 
    CPolygon (const IdType& typeId, const CoordTypes&... сoordTypes ) { 
       m_Id = typeId; 
       m_NumItems = 0; 
       variadicPushBack(сoordTypes...); 
    } 
 
    void variadicPushBack() {} 
 
    CPolygon(const CPolygon& src) : m_NumItems(src.m_NumItems), m_PonsVector(src.m_PonsVector){ 
        m_Id = src.m_Id; 
    } 
 
    CCommon * copieClass () const { 
        return new CPolygon(*this); 
    } 
 
    bool pointInside(int cordX, int cordY) { 
        bool inside = false; 
        int cnt = m_PonsVector.size(); 
        for (int i = 0, j = cnt - 1; i < cnt; j = i++) { 
            bool cond1 = (m_PonsVector[i].m_Y <= cordY) != (m_PonsVector[j].m_Y <= cordY); 
            bool cond2 = (m_PonsVector[j].m_Y - m_PonsVector[i].m_Y) != 0;             
            double deltaX = m_PonsVector[j].m_X - m_PonsVector[i].m_X; 
            double deltaY = m_PonsVector[j].m_Y - m_PonsVector[i].m_Y; 
            double factor = (cordY - m_PonsVector[i].m_Y) / deltaY; 
            double intersection_x = m_PonsVector[i].m_X + deltaX * factor; 
 
            if (cond1 && cond2 && cordX > intersection_x) { 
                inside = !inside; 
            } 
        } 
        return inside; 
    } 
 
   vector<CCoord> m_PonsVector; 
   int m_NumItems; 
};

class CTriangle : public CCommon { 
 
    public: 
        CTriangle(int id, const CCoord cordA, const CCoord cordB, const CCoord cordC) : 
            cordX1(cordA.m_X), cordY1(cordA.m_Y), cordX2(cordB.m_X), 
            cordY2(cordB.m_Y), x3(cordC.m_X), y3(cordC.m_Y) { 
            m_Id = id; 
        } 
     
 
        CTriangle(const CTriangle& src) :         
            cordX1(src.cordX1), cordY1(src.cordY1), cordX2(src.cordX2), 
            cordY2(src.cordY2), x3(src.x3), y3(src.y3) { 
                m_Id = src.m_Id; 
        } 
 
        CCommon * copieClass () const { 
            return new CTriangle(*this); 
        } 
 
        bool pointInside(int cordX, int cordY) { 
            auto dx1 = cordX1 - cordX, dy1 = cordY1 - cordY; 
            auto dx2 = cordX2 - cordX, dy2 = cordY2 - cordY; 
            auto dx3 = x3 - cordX, dy3 = y3 - cordY; 
 
            auto r1 = dx1 * (cordY2 - cordY1) - dy1 * (cordX2 - cordX1); 
            auto r2 = dx2 * (y3 - cordY2) - dy2 * (x3 - cordX2); 
            auto r3 = dx3 * (cordY1 - y3) - dy3 * (cordX1 - x3); 
 
            return ((r1 ^ r2) >= 0 && (r2 ^ r3) >= 0) || r1 == 0 || r2 == 0 || r3 == 0; 
 
        }     
 
        long cordX1, cordX2, x3, cordY1, cordY2, y3; 
};

class CScreen { 
    public: 
        CScreen() {} 
        vector<CCommon*> m_Objects; 
 
        vector<int> test ( int cordX, int cordY ) const {             
            vector<int> result; 
            for (auto el : m_Objects){ 
                if(el ->pointInside(cordX, cordY)) { 
                    result.emplace_back(el->m_Id);  
                }                 
            }                 
            return result;             
        } 
 
        void add ( const CCommon &obj ) { 
            m_Objects.emplace_back(obj.copieClass());  
        }  
 
        void optimize ( void ) {};                         
};



#ifndef __PROGTEST__
int                          main                          ( void )
{
  CScreen s0;
  s0 . add ( CRectangle ( 1, 10, 20, 30, 40 ) );
  s0 . add ( CRectangle ( 2, 20, 10, 40, 30 ) );
  s0 . add ( CTriangle ( 3, CCoord ( 10, 20 ), CCoord ( 20, 10 ), CCoord ( 30, 30 ) ) );
  s0 . optimize();
  assert ( s0 . test ( 0, 0 ) == (vector<int>{ }) );
  assert ( s0 . test ( 21, 21 ) == (vector<int>{ 1, 2, 3 }) );
  assert ( s0 . test ( 16, 17 ) == (vector<int>{ 3 }) );
  assert ( s0 . test ( 30, 22 ) == (vector<int>{ 1, 2 }) );
  assert ( s0 . test ( 35, 25 ) == (vector<int>{ 2 }) );

  CScreen s1;
  s1 . add ( CCircle ( 1, 10, 10, 15 ) );
  s1 . add ( CCircle ( 2, 30, 10, 15 ) );
  s1 . add ( CCircle ( 3, 20, 20, 15 ) );
  s1 . optimize();
  assert ( s1 . test ( 0, 0 ) == (vector<int>{ 1 }) );
  assert ( s1 . test ( 15, 15 ) == (vector<int>{ 1, 3 }) );
  assert ( s1 . test ( 20, 11 ) == (vector<int>{ 1, 2, 3 }) );
  assert ( s1 . test ( 32, 8 ) == (vector<int>{ 2 }) );

  CScreen s2;
  CCoord vertex21[] = { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) };
  s2 . add ( CPolygon ( 1, vertex21, vertex21 + 4 ) );
  CCoord vertex22[] = { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) };
  s2 . add ( CPolygon ( 2, vertex22, vertex22 + 5 ) );
  s2 . optimize();
  assert ( s2 . test ( 25, 15 ) == (vector<int>{ 1, 2 }) );
  assert ( s2 . test ( 25, 25 ) == (vector<int>{ 2 }) );
  assert ( s2 . test ( 15, 3 ) == (vector<int>{ 1 }) );
  assert ( s2 . test ( 11, 10 ) == (vector<int>{ }) );

  CScreen s3;
  std::initializer_list<CCoord> vertex31 = { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) };
  s3 . add ( CPolygon ( 1, vertex31 . begin (), vertex31 . end () ) );
  std::list<CCoord> vertex32 = { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) };
  s3 . add ( CPolygon ( 2, vertex32 . begin (), vertex32 . end () ) );
  s3 . optimize();
  assert ( s3 . test ( 25, 15 ) == (vector<int>{ 1, 2 }) );
  assert ( s3 . test ( 25, 25 ) == (vector<int>{ 2 }) );
  assert ( s3 . test ( 15, 3 ) == (vector<int>{ 1 }) );
  assert ( s3 . test ( 11, 10 ) == (vector<int>{ }) );
  s3 . add ( CPolygon ( 3, std::initializer_list<CCoord> { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) } ) );
  s3 . add ( CPolygon ( 4, { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) } ) );
  s3 . add ( CPolygon ( 5, CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) ) );
  s3 . optimize();
  assert ( s3 . test ( 25, 15 ) == (vector<int>{ 1, 2, 3, 4, 5 }) );
  assert ( s3 . test ( 25, 25 ) == (vector<int>{ 2, 4, 5 }) );
  assert ( s3 . test ( 15, 3 ) == (vector<int>{ 1, 3 }) );
  assert ( s3 . test ( 11, 10 ) == (vector<int>{ }) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
