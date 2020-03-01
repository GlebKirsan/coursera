#include "animals.h"
#include <test_runner.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

using Zoo = vector<unique_ptr<Animal>>;

enum class AnimalType {
        tiger,
        wolf,
        fox,
        undefined
};

AnimalType Decide(const string& animal) {
        if (animal == "Tiger")
                return AnimalType::tiger;
        else if (animal == "Wolf")
                return AnimalType::wolf;
        else if (animal == "Fox")
                return AnimalType::fox;
        else
                return AnimalType::undefined;
}

// Эта функция получает на вход поток ввода и читает из него описание зверей.
// Если очередное слово этого текста - Tiger, Wolf или Fox, функция должна
// поместить соответствующего зверя в зоопарк. В противном случае она должна
// прекратить чтение и сгенерировать исключение runtime_error.
Zoo CreateZoo(istream& in) {
        Zoo zoo;
        string word;
        while (in >> word) {
                AnimalType type = Decide(word);

                switch (type) {
                        case AnimalType::tiger: {
                                zoo.push_back(make_unique<Tiger>());
                                break;
                        }
                        case AnimalType::fox: {
                                zoo.push_back(make_unique<Fox>());
                                break;
                        }
                        case AnimalType::wolf: {
                                zoo.push_back(make_unique<Wolf>());
                                break;
                        }
                        default:
                                throw runtime_error("Unknown animal!");
                }
        }
        return zoo;
}

// Эта функция должна перебрать всех зверей в зоопарке в порядке их создания
// и записать в выходной поток для каждого из них результат работы виртуальной
// функции voice. Разделяйте голоса разных зверей символом перевода строки '\n'.
void Process(const Zoo& zoo, ostream& out) {
        for (const auto& animal : zoo) {
                out << animal->Voice() << "\n";
        }
}

void TestZoo() {
        istringstream input("Tiger Wolf Fox Tiger");
        ostringstream output;
        Process(CreateZoo(input), output);

        const string expected =
                "Rrrr\n"
                "Wooo\n"
                "Tyaf\n"
                "Rrrr\n";

        ASSERT_EQUAL(output.str(), expected);
}

int main() {
        TestRunner tr;
        RUN_TEST(tr, TestZoo);
}
