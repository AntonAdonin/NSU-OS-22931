#include "StringList.h"

namespace lab4 {
using std::make_unique;
using std::endl;

class StringList::Impl {
 public:
  void Read(istream& stream) {
    for (auto string = String(); stream >> string;) {
      if (string.IsTerminator())
        break;

      if (!head_)
        head_ = make_unique<ListEntry>(std::move(string));

      ListEntry* last;
      for (last = head_.get(); last->next_; last = last->next_.get()) {
      }

      last->next_ = make_unique<ListEntry>(std::move(string));
    }
  }

  void Write(ostream& stream) {
    for (ListEntry* entry = head_.get(); entry; entry = entry->next_.get())
      stream << entry->string_ << endl;
  }

 private:
  struct ListEntry {
    explicit ListEntry(String string) :
        string_{std::move(string)} {
    }

    String string_;

    unique_ptr<ListEntry> next_;
  };

  unique_ptr<ListEntry> head_;
};

StringList::StringList() :
    pimpl_{make_unique<Impl>()} {
}

istream& operator>>(istream& input, StringList& string_list) {
  string_list.pimpl_->Read(input);
  return input;
}

ostream& operator<<(ostream& output, StringList& string_list) {
  string_list.pimpl_->Write(output);
  return output;
}

StringList::~StringList() = default;
}