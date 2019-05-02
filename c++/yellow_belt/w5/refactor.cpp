#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Human{
public:
    Human(const string &name):
    Name(name){}
    Human(const string &name, const string &profession):
    Name(name), Profession(profession){}

    virtual void Walk(const string &destination) const{
        Call() << " walks to: " << destination << endl;
    }

    ostream& Call() const {
        cout << Profession << ": " << Name;
        return cout;
    }

    string GetName() const {
        return Name;
    }

    string GetProf() const {
        return Profession;
    }

protected:
    const string Name;
    const string Profession;
};


class Student: public Human {
public:
    Student(const string &name, const string &favouriteSong):
    Human(name, "Student"), FavouriteSong(favouriteSong){}

    void Learn() {
        Call() << " learns" << endl;
    }

    void Walk(const string &destination) const override{
        Human::Walk(destination);
        SingSong();
    }

    void SingSong() const {
        Call() << " sings a song: " << FavouriteSong << endl;
    }

private:
    const string FavouriteSong;
};


class Teacher: public Human {
public:

    Teacher(const string &name, const string &subject):
    Human(name, "Teacher"), Subject(subject){}

    void Teach() {
        Call() << " teaches: " << Subject << endl;
    }

private:
    const string Subject;
};


class Policeman: public Human {
public:
    Policeman(const string &name):
    Human(name, "Policeman"){}

    void Check(const Human &h) {
        Call() << " checks " << h.GetProf() <<". " <<
        h.GetProf() << "'s name is: " << h.GetName() << endl;
    }

};

void VisitPlaces(Human &h, vector<string> places) {
    for (const auto &p : places) {
        h.Walk(p);
    }
}


int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
