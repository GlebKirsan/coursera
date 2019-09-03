#include <string>
#include <list>
#include <algorithm>
#include <iterator>
#include "test_runner.h"
using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor();
  
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;

private:
  using Iterator = list<char>::iterator;
  Iterator Advance(Iterator it, int steps) const {
    while (steps > 0 and it != end(text)){
      ++it;
      --steps;
    }
    while (steps < 0 and it != begin(text)){
      --it;
      ++steps;
    }
    return it;
  }

  list<char> buffer;
  list<char> text;

  Iterator cursor_position;
};

Editor::Editor():
  cursor_position{end(text)} {}

void Editor::Insert(char token){
  text.insert(cursor_position, token);
}

void Editor::Left(){
  cursor_position = Advance(cursor_position, -1);
}

void Editor::Right(){
  cursor_position = Advance(cursor_position, 1);
}

void Editor::Paste(){
  text.insert(cursor_position, begin(buffer), end(buffer));
}

void Editor::Copy(size_t tokens){
  auto end_position = Advance(cursor_position, static_cast<int>(tokens));
  buffer.assign(cursor_position, end_position);
}

void Editor::Cut(size_t tokens){
  auto end_position = Advance(cursor_position, static_cast<int>(tokens));
  buffer.assign(cursor_position, end_position);
  cursor_position = text.erase(cursor_position, end_position);
}

string Editor::GetText() const {
  return {begin(text), end(text)};
}

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}