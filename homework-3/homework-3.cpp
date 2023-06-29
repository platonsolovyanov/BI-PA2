#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

class CWidget {
  public:               
    virtual CWidget* getThisClass() { return this; }        

    virtual int getId() const { return -1; }        

    virtual CWidget * clone() const = 0;    
    virtual void print(ostream &out, int indent = 0) const = 0;

    virtual ~CWidget() = default;

    friend ostream &operator<<(ostream &out, const CWidget &widget); 
};

class CButton : public CWidget {
  public:
      CButton(int id, const CRect& relPos, const string& name) : m_Id(id), m_RelPos(relPos), m_Name(name), m_AbsPos(CRect(0,0,0,0)) {}      

      void setAbsPos (CRect newAbsPos) {        
        m_AbsPos = newAbsPos;
      }

      virtual CButton* getThisClass() override { return this; }

      virtual int getId() const override { return m_Id; }

      CRect getRelPos () {
        return m_RelPos;
      }      

      CWidget* clone() const override {
          return new CButton(*this);
      }       

      void print(ostream &out, int indent = 0) const override{
          out << "[" << m_Id << "] Button \"" << m_Name << "\" " << m_AbsPos << "\n";
      }

  private:
      int m_Id;
      CRect m_RelPos;
      string m_Name;
      CRect m_AbsPos;
};

class CLabel : public CWidget {
  public:
    CLabel(int id, const CRect& relPos, const string& label) 
      : m_Id(id), m_RelPos(relPos), m_Label(label), m_AbsPos(CRect(0,0,0,0)) {}
    
    void setAbsPos (CRect newAbsPos) {
      m_AbsPos = newAbsPos;
    }

    CRect getRelPos () {
      return m_RelPos;
    }

    virtual CLabel* getThisClass() override { return this; }

    virtual int getId() const override { return m_Id; }

    CWidget* clone() const override {
      return new CLabel(*this);
    }         

    void print(ostream &out, int indent = 0) const override { 
      out << "[" << m_Id << "] Label \"" << m_Label << "\" " << m_AbsPos << endl;
    }

  private:
    int m_Id;
    CRect m_RelPos;    
    string m_Label;
    CRect m_AbsPos;
};

class CInput : public CWidget {
  public:
    CInput(int id, const CRect& relPos, const string& value) 
      : m_Id(id), m_RelPos(relPos), m_Value(value),  m_AbsPos(CRect(0,0,0,0)) {}    

    void setAbsPos (CRect newAbsPos) {
      m_AbsPos = newAbsPos;
    }

    void setValue ( string newValue) {
      m_Value = newValue;
    }

    CRect getRelPos () {
      return m_RelPos;
    }

    string getValue() {
      return m_Value;
    }

    virtual CInput* getThisClass() override { return this; }

    virtual int getId() const override { return m_Id; }  

    CWidget* clone() const override {
      return new CInput(*this);
    }    

    void print(ostream &out, int indent = 0) const override { 
      out << "[" << m_Id << "] Input \"" << m_Value << "\" " << m_AbsPos << endl;
    }

  private:
    int m_Id;
    CRect m_RelPos;    
    string m_Value;
    CRect m_AbsPos;
};

class CComboBox : public CWidget {
  public:
    CComboBox ( int id,const CRect& relPos ) 
      : m_Id(id), m_RelPos(relPos), m_AbsPos(CRect(0,0,0,0)), m_ActivSelect(0), m_LastEl(false) {}
    
    CComboBox &add(const string selectedStr) {
        m_Selected.emplace_back(selectedStr);
        return *this;
    }    

    void setSelected (int newSelected) {          
      m_ActivSelect = newSelected;           
    }

    void setAbsPos (CRect newAbsPos) {
      m_AbsPos = newAbsPos;    
    }

    void setLastEl (bool flag) {
      m_LastEl = flag;
    }

    bool getLastEl () {
      return m_LastEl;
    }

    int getSelected() const {      
      return m_ActivSelect;
    }

    CRect getRelPos () const {
      return m_RelPos;
    }

    virtual int getId() const override { return m_Id; }

    virtual CComboBox* getThisClass() override { return this; }

    CWidget *clone() const override {
      return new CComboBox(*this);
    }    

    void print(ostream &out, int indent = 0) const override {
        out << "[" << m_Id << "] ComboBox " << m_AbsPos << endl;  

        int count = 0;
        for (const auto el : m_Selected){
          

          if(m_LastEl) {
            out << "|" << string(indent, ' ');            
          } else {
            out << string(indent, ' ') ;
          }
          out << "+-";

          if(m_ActivSelect == count){
            out << ">" << el << "<" << endl;
          } else {
            out << " " << el << endl;
          }

          count++;
        }
        count = 0;        
    }
  private:
    int m_Id;    
    CRect m_RelPos;
    CRect m_AbsPos;
    int m_ActivSelect;
    bool m_LastEl;

    vector<string> m_Selected;

};

class CWindow : public CWidget {  
  public:
    CWindow(int id, const string &title, const CRect &absPos)
        : m_Id(id), m_title(title), m_AbsPos(absPos) {}
    

    CWindow(const CWindow &other) 
        : m_Id(other.m_Id), m_title(other.m_title), m_AbsPos(other.m_AbsPos) {
      for(auto & widget : other.m_Widget) {        
        add(*widget);
      }
    }

    CRect calculateAbsPosEl (CRect& relPosEl) {      
      double m_X = (m_AbsPos.m_W * relPosEl.m_X) + m_AbsPos.m_X;
      double m_Y = (m_AbsPos.m_H * relPosEl.m_Y) + m_AbsPos.m_Y;
      double m_W = m_AbsPos.m_W * relPosEl.m_W;
      double m_H = m_AbsPos.m_H * relPosEl.m_H;
      return CRect(m_X, m_Y, m_W, m_H);
    }

    CWindow &add(const CWidget &widget) {
      unique_ptr<CWidget> newItem(widget.clone());

      if (CButton *button = dynamic_cast<CButton *>(newItem.get())) {
        CRect relPosEl = button->getRelPos();
        button->setAbsPos(calculateAbsPosEl(relPosEl));
      } else if (CLabel *label = dynamic_cast<CLabel *>(newItem.get())) {
        CRect relPosEl = label->getRelPos();
        label->setAbsPos(calculateAbsPosEl(relPosEl));
      } else if (CInput *input = dynamic_cast<CInput *>(newItem.get())) {
        CRect relPosEl = input->getRelPos();
        input->setAbsPos(calculateAbsPosEl(relPosEl));
      } else if (CComboBox *comboBox = dynamic_cast<CComboBox *>(newItem.get())) {
        CRect relPosEl = comboBox->getRelPos();
        comboBox->setAbsPos(calculateAbsPosEl(relPosEl));
      }

      m_Widget.emplace_back(move(newItem));
      return *this;
    }

    CWindow &operator=(const CWindow &other) {
      if (this != &other) {
        m_Id = other.m_Id;
        m_title = other.m_title;
        m_AbsPos = other.m_AbsPos;
        m_Widget.clear();

        for (auto &widget : other.m_Widget) {
          unique_ptr<CWidget> newItem(widget->clone());
          m_Widget.emplace_back(move(newItem));
        }
      }
      return *this;
    }

    CWidget *clone() const override{
        return new CWindow(*this);
    }
    
    void setPosition(CRect newAbsPosWindow) {
      m_AbsPos = newAbsPosWindow;      
      vector<unique_ptr<CWidget>> tempWidgets;

      for (const auto& widget : m_Widget) {
        unique_ptr<CWidget> newItem(widget->clone());
        
        if (CButton *button = dynamic_cast<CButton *>(newItem.get())) {
          CRect relPosEl = button->getRelPos();
          button->setAbsPos(calculateAbsPosEl(relPosEl));
        } else if (CLabel *label = dynamic_cast<CLabel *>(newItem.get())) {
          CRect relPosEl = label->getRelPos();
          label->setAbsPos(calculateAbsPosEl(relPosEl));
        } else if (CInput *input = dynamic_cast<CInput *>(newItem.get())) {
          CRect relPosEl = input->getRelPos();
          input->setAbsPos(calculateAbsPosEl(relPosEl));
        } else if (CComboBox *comboBox = dynamic_cast<CComboBox *>(newItem.get())) {
          CRect relPosEl = comboBox->getRelPos();
          comboBox->setAbsPos(calculateAbsPosEl(relPosEl));
        }

        tempWidgets.emplace_back(move(newItem));
      }
      
      m_Widget.clear();
      m_Widget = move(tempWidgets);
    }

    void print(ostream &out, int indent = 0) const  override{ 
      out << "[" << m_Id << "] Window \"" << m_title << "\" " << m_AbsPos << endl;

      size_t count = 0;
      size_t countComboBox = 0;
      size_t lastElementIndex = m_Widget.size()-1;
      
      for (const auto& el : m_Widget){  
        out << "+- ";
        if(CComboBox *comboBox = dynamic_cast<CComboBox*>(el.get())){                    
          if(lastElementIndex == count && countComboBox == 0) {
            comboBox->setLastEl(false);
            el->print(out, 3);
          } else if (lastElementIndex != count && countComboBox > 0 ) {
            comboBox->setLastEl(true);
            el->print(out, 2);
          } else {
            comboBox->setLastEl(true);
            el->print(out, 2);
          }          

        } else {
          el->print(out, 0);
        }
        
        count++;
      }
      count = 0;
      
     
    }

    CWidget* search(int id) {              
        for (const auto& widget : m_Widget) {
          int widgetId = widget->getId();                   
          if ( widgetId == id) { 
            if(CComboBox *comboBox = dynamic_cast<CComboBox*>(widget.get())) {
              comboBox->setLastEl(false);
            } 
            return widget->getThisClass();
            
          }          
        }    
      return nullptr;    
    }

    friend ostream &operator<<(ostream &out, const CWindow &window) {
        window.print(out);
        return out;
    }    
    
  private:
    int m_Id;
    string m_title;
    CRect m_AbsPos;    
    
    vector< unique_ptr<CWidget> > m_Widget;
};

ostream &operator<<(ostream &out, const CWidget &widget) {
  widget.print(out);
  return out;
}

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */