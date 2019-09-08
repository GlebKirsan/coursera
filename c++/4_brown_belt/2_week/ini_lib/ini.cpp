#include "ini.h"

#include <string_view>

namespace Ini {

    Section& Document::AddSection(string name) {
        string section;

        string::iterator start = name[0] == '[' && name.back() == ']' ?
                                 next(begin(name)) : begin(name);
        string::iterator finish = name[0] == '[' && name.back() == ']' ?
                                  prev(end(name)) : end(name);;
        move(start, finish, back_inserter(section));

        return sections[move(section)];
    }

    const Section& Document::GetSection(const std::string& name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    pair<string, string> Split(string_view line, char c) {
        auto char_pos = line.find(c);
        string left{line.substr(0, char_pos)};
        line.remove_prefix(++char_pos);
        string right{line.substr(0)};

        return {move(left), move(right)};
    }

    void ParseSection(Section& section, istream& input) {
        for (string line; input.peek() != '[' &&
                          getline(input, line) &&
                          !line.empty();) {
            auto[key, value] = Split(line, '=');
            section[move(key)] = move(value);
        }
    }

    Document Load(istream& input) {
        Document new_document;
        for (string line; getline(input, line);) {
            if (line.empty())
                continue;
            ParseSection(new_document.AddSection(move(line)), input);
        }

        return new_document;
    }
}