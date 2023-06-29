#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

struct EmployeeStruct {
    string name;
    string surname;
    string email;
    unsigned int salary;

    bool operator == (EmployeeStruct employee) {
        if(name == employee.name && surname == employee.surname){
            return true;
        }
        if(email == employee.email) {
            return true;
        }
        return false;
    };
};

struct EmployeeSurName {
    string name;
    string surname;
    int indexEmpVectr;

    bool operator<(const EmployeeSurName& employee) const {
        if (surname != employee.surname) {
            return surname < employee.surname;
        }
        return name < employee.name;
    }
};

struct EmployeeEmail {
    string email;
    int indexEmpVectr;

    bool operator<(const EmployeeEmail& employee) const {
        return email < employee.email;
    }
};


struct EmployeeSalary {
    unsigned int salary;
//    int indexEmpVectr;

    bool operator<(const EmployeeSalary& employee) const {
        return salary < employee.salary;
    }
};

class CPersonalAgenda
{
  public:
    CPersonalAgenda  ( void ) = default;
    ~CPersonalAgenda ( void ) = default;

    bool add ( const string& name, const string& surname, const string& email, unsigned int salary ){
        int index = 0;
        EmployeeStruct employee = {name, surname, email, salary};


            for (EmployeeStruct elVector: empVector) {
                index++;
                if (elVector == employee) {
                    return false;
                }
            }

        empVector.push_back( employee );


        EmployeeSurName empSurName = {name, surname, index};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                              empSurName,
                                     sortingSurName);
        empVectorSurName.insert(itSurName, empSurName);

        EmployeeEmail empEmail = { email, index};
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                              empEmail, sortingEmail);
        empVectorEmail.insert(itEmail, empEmail);

        auto itSalary = std::lower_bound(empVectorSalary.begin(), empVectorSalary.end(),salary);
        empVectorSalary.insert(itSalary, salary);


        return true;
    }

    bool del ( const string& name,const string& surname ){
        EmployeeSurName empSurName = { name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                     empSurName);
        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname){
            string emailSearch = empVector[itSurName->indexEmpVectr].email;

            EmployeeEmail empEmail = {emailSearch};
            auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                       empEmail);

            auto itSalary = lower_bound(empVectorSalary.begin(),
                                        empVectorSalary.end(), empVector[itSurName->indexEmpVectr].salary);

            for(EmployeeSurName el : empVectorSurName) {
                if(itSurName->indexEmpVectr < el.indexEmpVectr){
                    el.indexEmpVectr-=1;
                }
            }
            for(EmployeeEmail el : empVectorEmail) {
                if(itEmail->indexEmpVectr < el.indexEmpVectr) {
                    el.indexEmpVectr-=1;
                }
            }


            empVectorSalary.erase(itSalary);
            empVector.erase(empVector.begin() + itSurName->indexEmpVectr);
            empVectorSurName.erase(itSurName);
            empVectorEmail.erase(itEmail);
            return true;
        }
        return false;
    };

    bool del ( const string& email ){
        EmployeeEmail empEmail = { email};
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                     empEmail);
        if(itEmail != empVectorEmail.end() && itEmail->email == email ){
            string nameSearch = empVector[itEmail->indexEmpVectr].name;
            string surnameSearch = empVector[itEmail->indexEmpVectr].surname;

            EmployeeSurName empSurName = {nameSearch, surnameSearch};
            auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                         empSurName);

            auto itSalary = lower_bound(empVectorSalary.begin(),
                                        empVectorSalary.end(), empVector[itSurName->indexEmpVectr].salary);

            for(EmployeeSurName el : empVectorSurName) {
                if(itSurName->indexEmpVectr < el.indexEmpVectr){
                    el.indexEmpVectr-=1;
                }
            }
            for(EmployeeEmail el : empVectorEmail) {
                if(itEmail->indexEmpVectr < el.indexEmpVectr) {
                    el.indexEmpVectr-=1;
                }
            }

            empVectorSalary.erase(itSalary);
            empVector.erase(empVector.begin() + itSurName->indexEmpVectr);
            empVectorSurName.erase(itSurName);
            empVectorEmail.erase(itEmail);
            return true;
        }
        return false;
    };

    bool changeName ( const string& email,const string& newName,const string& newSurname ){
        EmployeeEmail empEmail = { email };
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                   empEmail);

        if(itEmail != empVectorEmail.end() && itEmail->email == email){
            EmployeeSurName empSurName = { empVector[itEmail->indexEmpVectr].name,
                                           empVector[itEmail->indexEmpVectr].surname};
            auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                         empSurName);

            int index = itSurName->indexEmpVectr;
            EmployeeSurName empNewSurname = { newName, newSurname, index };
            auto itNewSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                            empNewSurname, sortingSurName);
            if(itNewSurName->name != newName && itNewSurName->surname != newSurname) {
                empVector[itEmail->indexEmpVectr].name = newName;
                empVector[itEmail->indexEmpVectr].surname = newSurname;

                itSurName->surname = newSurname;
                itSurName->name = newName;

                sort(empVectorSurName.begin(), empVectorSurName.end(), sortingSurName);
                return true;
            }
        }
        return false;
    };

    bool changeEmail ( const string& name,const string& surname,const string& newEmail ){
        EmployeeSurName empSurName = { name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                     empSurName);

        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname){
            EmployeeEmail empEmail = { empVector[itSurName->indexEmpVectr].email };
            auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                       empEmail);

            int index = empVectorEmail[itEmail->indexEmpVectr].indexEmpVectr;
            EmployeeEmail empNewEmail = { newEmail, index };
            auto itNewEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                          empNewEmail, sortingEmail);
            if( itNewEmail->email != newEmail){
                empVector[itSurName->indexEmpVectr].email = newEmail;

                itEmail->email = newEmail;

                sort(empVectorEmail.begin(), empVectorEmail.end(), sortingEmail);
                return true;
            }
        }
        return false;
    };

    bool  setSalary ( const string& name,const string& surname,unsigned int salary ){
        EmployeeSurName empSurName = {name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                     empSurName,
                                     sortingSurName);

        auto itSalary = lower_bound(empVectorSalary.begin(),
                                    empVectorSalary.end(), empVector[itSurName->indexEmpVectr].salary);

        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname){
            empVector[itSurName->indexEmpVectr].salary = salary;

            empVectorSalary.erase(itSalary);
            itSalary = lower_bound(empVectorSalary.begin(),
                                        empVectorSalary.end(), salary);
            empVectorSalary.insert(itSalary, salary);

            return true;
        }
        return false;
    }

    bool setSalary ( const string& email,unsigned int salary ){
        EmployeeEmail empEmail = { email };
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                   empEmail, sortingEmail);

        auto itSalary = lower_bound(empVectorSalary.begin(),
                                    empVectorSalary.end(), empVector[itEmail->indexEmpVectr].salary);

        if(itEmail != empVectorEmail.end() && itEmail->email == email){
            empVector[itEmail->indexEmpVectr].salary = salary;

            empVectorSalary.erase(itSalary);
            itSalary = lower_bound(empVectorSalary.begin(),
                                   empVectorSalary.end(), salary);
            empVectorSalary.insert(itSalary, salary);
            return true;
        }
        return false;
    }

    unsigned int  getSalary ( const string& name,const string& surname ) const{
        EmployeeSurName empSurName = {name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                     empSurName,
                                     sortingSurName);
        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname){
            return empVector[itSurName->indexEmpVectr].salary;
        }
        return 0;
    }

    unsigned int  getSalary ( const string& email ) const{
        EmployeeEmail empEmail = { email };
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),
                                   empEmail, sortingEmail);

        if(itEmail != empVectorEmail.end() && itEmail->email == email){
            return empVector[itEmail->indexEmpVectr].salary;

        }
        return 0;
    }

    bool getFirst ( string& outName,string& outSurname ) const {
        if(empVector.size() == 0){
            return false;
        } else {
            outName = empVectorSurName[0].name;
            outSurname = empVectorSurName[0].surname;
            return true;
        }
    };

    bool  getNext ( const string& name, const string& surname, string& outName, string& outSurname ) const{
        EmployeeSurName empSurName = {name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),
                                     empSurName,
                                     sortingSurName);

        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname ){
            auto nextItSurName = std::next(itSurName);
            if(nextItSurName != empVectorSurName.end()){
                outName = nextItSurName->name;
                outSurname = nextItSurName->surname;
                return true;
            }
        }
        return false;
    };

    bool getRank  ( const string& name,const string& surname,int& rankMin,int& rankMax ) const{
        EmployeeSurName empSurName = {name, surname};
        auto itSurName = lower_bound(empVectorSurName.begin(), empVectorSurName.end(),empSurName);
        if(itSurName != empVectorSurName.end() && itSurName->name == name && itSurName->surname == surname) {

            unsigned int salaryFromEmpVector = empVector[itSurName->indexEmpVectr].salary;

            auto itSalary = lower_bound(empVectorSalary.begin(), empVectorSalary.end(),salaryFromEmpVector);
            auto itUpperSalary = upper_bound(empVectorSalary.begin(), empVectorSalary.end(),salaryFromEmpVector);
            int max = itUpperSalary - empVectorSalary.begin();
            int min = itSalary - empVectorSalary.begin();

            rankMin = min;
            rankMax = max - 1;
            return true;
        }
        return false;
    }

    bool getRank ( const string& email, int& rankMin, int& rankMax ) const {
        EmployeeEmail empEmail = { email };
        auto itEmail = lower_bound(empVectorEmail.begin(), empVectorEmail.end(),empEmail);
        if(itEmail != empVectorEmail.end() && itEmail->email == email) {

            unsigned int salaryFromEmpVector = empVector[itEmail->indexEmpVectr].salary;

            auto itSalary = lower_bound(empVectorSalary.begin(), empVectorSalary.end(),salaryFromEmpVector);
            auto itUpperSalary = upper_bound(empVectorSalary.begin(), empVectorSalary.end(),salaryFromEmpVector);
            int max = itUpperSalary - empVectorSalary.begin();
            int min = itSalary - empVectorSalary.begin();



            rankMin = min;
            rankMax = max - 1;
            return true;
        }
        return false;
    }

  private:
    vector<EmployeeStruct> empVector;
    vector<EmployeeSurName> empVectorSurName;
    vector<EmployeeEmail> empVectorEmail;
    vector<unsigned int> empVectorSalary;

    static bool sortingSurName(const EmployeeSurName& emp1, const EmployeeSurName& emp2) {
        return emp1 < emp2;
    }

    static bool sortingEmail(const EmployeeEmail& emp1, const EmployeeEmail& emp2) {
        return emp1 < emp2;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
    assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
    assert ( b1 . setSalary ( "john", 32000 ) );
    assert ( b1 . getSalary ( "john" ) ==  32000 );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );
    assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . getSalary ( "john" ) ==  35000 );
    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . changeName ( "peter", "James", "Bond" ) );
    assert ( b1 . getSalary ( "peter" ) ==  23000 );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "james" ) ==  23000 );
    assert ( b1 . getSalary ( "peter" ) ==  0 );
    assert ( b1 . del ( "james" ) );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );
    assert ( b1 . del ( "John", "Miller" ) );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . del ( "john" ) );
    assert ( ! b1 . getFirst ( outName, outSurname ) );
    assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . getFirst ( outName, outSurname ) );
    assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . setSalary ( "joe", 90000 ) );
    assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . getSalary ( "joe" ) ==  0 );
    assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . getRank ( "joe", lo, hi ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . del ( "Joe", "Black" ) );
    assert ( ! b2 . del ( "joe" ) );
    assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . setSalary ( "peter", 37000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  0 );
    assert ( ! b2 . getRank ( "peter", lo, hi ) );
    assert ( ! b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . del ( "peter" ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  40000 );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
