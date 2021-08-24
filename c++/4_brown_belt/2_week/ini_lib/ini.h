#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

namespace Ini {
    using namespace std;
    using Section = unordered_map<string, string>;

    class Document {
    public:
        Section& AddSection(string name);
        const Section& GetSection(const string& name) const;
        size_t SectionCount() const;

        const unordered_map<string, Section>& A() const {
            return sections;
        }

    private:
        unordered_map<string, Section> sections;
    };

    Document Load(istream& input);
}