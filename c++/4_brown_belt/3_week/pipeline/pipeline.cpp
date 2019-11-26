#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
        string from;
        string to;
        string body;
};


class Worker {
public:
        virtual ~Worker() = default;
        virtual void Process(unique_ptr<Email> email) = 0;
        virtual void Run() {
                // только первому worker-у в пайплайне нужно это
                // имплементировать
                throw logic_error("Unimplemented");
        }

protected:
        // реализации должны вызывать PassOn, чтобы передать объект дальше
        // по цепочке обработчиков
        void PassOn(unique_ptr<Email> email) const {
                if (next_) {
                        next_->Process(move(email));
                }
        }

public:
        void SetNext(unique_ptr<Worker> next) {
                next_ = move(next);
        }

private:
        unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
        explicit Reader(istream& in) : is_(in) {}

        vector<Email> ReadEmails() {
                vector<Email> emails;

                for (string from; getline(is_, from);) {
                        string to;
                        string body;

                        getline(is_, to);
                        getline(is_, body);

                        emails.emplace_back(Email{from, to, body});
                }

                return emails;
        }

        void Process(unique_ptr<Email> email) override {
                PassOn(move(email));
        }

        void Run() override {
                vector<Email> emails = ReadEmails();

                for (Email& email : emails) {
                        Process(make_unique<Email>(email));
                }
        }

private:
        istream& is_;
};


class Filter : public Worker {
public:
        using Function = function<bool(const Email&)>;

public:
        explicit Filter(Function predicate) : predicate_(move(predicate)) {}

        void Process(unique_ptr<Email> email) override {
                if (predicate_(*email)) {
                        PassOn(move(email));
                }
        }

private:
        Function predicate_;
};


class Copier : public Worker {
public:
        explicit Copier(string recipient) : recipient_(move(recipient)) {}

        void Process(unique_ptr<Email> email) override {
                Email copy = *email;
                PassOn(move(email));

                if (copy.to != recipient_) {
                        copy.to = recipient_;
                        PassOn(make_unique<Email>(copy));
                }
        }

private:
        string recipient_;
};


class Sender : public Worker {
public:
        explicit Sender(ostream& os) : os_(os) {}

        void Process(unique_ptr<Email> email) override {
                os_ << email->from << '\n';
                os_ << email->to << '\n';
                os_ << email->body << '\n';
                PassOn(move(email));
        }
private:
        ostream& os_;
};


// реализуйте класс
class PipelineBuilder {
public:
        // добавляет в качестве первого обработчика Reader
        explicit PipelineBuilder(istream& in) {
                workers.emplace_back(make_unique<Reader>(in));
        }

        // добавляет новый обработчик Filter
        PipelineBuilder& FilterBy(Filter::Function filter) {
                workers.emplace_back(make_unique<Filter>(move(filter)));
                return *this;
        }

        // добавляет новый обработчик Copier
        PipelineBuilder& CopyTo(string recipient) {
                workers.emplace_back(make_unique<Copier>(move(recipient)));
                return *this;
        }

        // добавляет новый обработчик Sender
        PipelineBuilder& Send(ostream& out) {
                workers.emplace_back(make_unique<Sender>(out));
                return *this;
        }

        // возвращает готовую цепочку обработчиков
        unique_ptr<Worker> Build() {
                for (size_t i = workers.size(); i > 1; --i) {
                        workers[i - 2]->SetNext(move(workers[i - 1]));
                        workers.pop_back();
                }

                return move(workers.front());
        }

private:
        vector<unique_ptr<Worker>> workers;
};


void TestSanity() {
        string input = (
                "erich@example.com\n"
                "richard@example.com\n"
                "Hello there\n"

                "erich@example.com\n"
                "ralph@example.com\n"
                "Are you sure you pressed the right button?\n"

                "ralph@example.com\n"
                "erich@example.com\n"
                "I do not make mistakes of that kind\n"
        );
        istringstream inStream(input);
        ostringstream outStream;

        PipelineBuilder builder(inStream);
        builder.FilterBy([](const Email& email) {
                return email.from == "erich@example.com";
        });
        builder.CopyTo("richard@example.com");
        builder.Send(outStream);
        auto pipeline = builder.Build();

        pipeline->Run();

        string expectedOutput = (
                "erich@example.com\n"
                "richard@example.com\n"
                "Hello there\n"

                "erich@example.com\n"
                "ralph@example.com\n"
                "Are you sure you pressed the right button?\n"

                "erich@example.com\n"
                "richard@example.com\n"
                "Are you sure you pressed the right button?\n"
        );

        ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
        TestRunner tr;
        RUN_TEST(tr, TestSanity);
        return 0;
}
