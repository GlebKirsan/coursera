#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class INotifier{
public:
    virtual void Notify(const string &message) const = 0;
};

class SmsNotifier: public INotifier{
public:
    SmsNotifier(const string &number):
    number(number){}

    void Notify(const string &message) const override{
        SendSms(number, message);
    }
    const string number;
};

class EmailNotifier: public INotifier{
public:
    EmailNotifier(const string &email):
    email(email){}

    void Notify(const string &message) const override{
        SendEmail(email, message);
    }
    const string email;
};


//int main() {
//    SmsNotifier sms{"+7-495-777-77-77"};
//    EmailNotifier email{"na-derevnyu@dedushke.ru"};
//
//    Notify(sms, "I have White belt in C++");
//    Notify(email, "And want a Yellow one");
//    return 0;
//}