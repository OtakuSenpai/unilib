// This file is part of UniLib.
//
// Copyright 2014 by Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// UniLib is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// UniLib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with UniLib.  If not, see <http://www.gnu.org/licenses/>.

// UniLib version: 0.9.9
// Unicode version: 6.3.0

#pragma once

#include <cstddef>
#include <iterator>
#include <string>

namespace ufal {
namespace unilib {

class utf8 {
 public:
  static bool valid(const char* str);
  static bool valid(const char* str, size_t len);
  static inline bool valid(const std::string& str);

  static inline char32_t decode(const char*& str);
  static inline char32_t decode(const char*& str, size_t& len);
  static inline char32_t first(const char* str);
  static inline char32_t first(const char* str, size_t len);
  static inline char32_t first(const std::string& str);

  static void decode(const char* str, std::u32string& decoded);
  static void decode(const char* str, size_t len, std::u32string& decoded);
  static inline void decode(const std::string& str, std::u32string& decoded);

  class string_decoder_helper {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline string_decoder_helper(const char* str);
    const char* str;
    friend class utf8;
  };
  static inline string_decoder_helper decoder(const char* str);
  static inline string_decoder_helper decoder(const std::string& str);

  class buffer_decoder_helper {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline buffer_decoder_helper(const char* str, size_t len);
    const char* str;
    size_t len;
    friend class utf8;
  };
  static inline buffer_decoder_helper decoder(const char* str, size_t len);

  static inline void append(char*& str, char32_t chr);
  static inline void append(std::string& str, char32_t chr);

  static void encode(const std::u32string& str, std::string& encoded);

  template<class F> static void map(F f, const char* str, std::string& result);
  template<class F> static void map(F f, const char* str, size_t len, std::string& result);
  template<class F> static void map(F f, const std::string& str, std::string& result);

 private:
  static const char REPLACEMENT_CHAR = '?';
};

bool utf8::valid(const std::string& str) {
  return valid(str.c_str());
}

char32_t utf8::decode(const char*& str) {
  const unsigned char*& ptr = (const unsigned char*&) str;
  if (*ptr < 0x80) return *ptr++;
  else if (*ptr < 0xC0) return ++ptr, REPLACEMENT_CHAR;
  else if (*ptr < 0xE0) {
    char32_t res = (*ptr++ & 0x1F) << 6;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((*ptr++) & 0x3F);
  } else if (*ptr < 0xF0) {
    char32_t res = (*ptr++ & 0x0F) << 12;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((*ptr++) & 0x3F) << 6;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((*ptr++) & 0x3F);
  } else if (*ptr < 0xF8) {
    char32_t res = (*ptr++ & 0x07) << 18;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((*ptr++) & 0x3F) << 12;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((*ptr++) & 0x3F) << 6;
    if (*ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((*ptr++) & 0x3F);
  } else return ++ptr, REPLACEMENT_CHAR;
}

char32_t utf8::decode(const char*& str, size_t& len) {
  const unsigned char*& ptr = (const unsigned char*&) str;
  if (!len) return 0; len--;
  if (*ptr < 0x80) return *ptr++;
  else if (*ptr < 0xC0) return ++ptr, REPLACEMENT_CHAR;
  else if (*ptr < 0xE0) {
    char32_t res = (*ptr++ & 0x1F) << 6;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((--len, *ptr++) & 0x3F);
  } else if (*ptr < 0xF0) {
    char32_t res = (*ptr++ & 0x0F) << 12;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((--len, *ptr++) & 0x3F) << 6;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((--len, *ptr++) & 0x3F);
  } else if (*ptr < 0xF8) {
    char32_t res = (*ptr++ & 0x07) << 18;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((--len, *ptr++) & 0x3F) << 12;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    res += ((--len, *ptr++) & 0x3F) << 6;
    if (len <= 0 || *ptr < 0x80 || *ptr >= 0xC0) return REPLACEMENT_CHAR;
    return res + ((--len, *ptr++) & 0x3F);
  } else return ++ptr, REPLACEMENT_CHAR;
}

char32_t utf8::first(const char* str) {
  return decode(str);
}

char32_t utf8::first(const char* str, size_t len) {
  return decode(str, len);
}

char32_t utf8::first(const std::string& str) {
  return first(str.c_str());
}

void utf8::decode(const std::string& str, std::u32string& decoded) {
  decode(str.c_str(), decoded);
}

class utf8::string_decoder_helper::iterator : public std::iterator<std::input_iterator_tag, char32_t> {
 public:
  iterator(const char* str) : codepoint(0), next(str) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next) {}
  iterator& operator++() { if (next) { codepoint = decode(next); if (!codepoint) next = nullptr; } return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next == other.next; }
  bool operator!=(const iterator& other) const { return next != other.next; }
  const char32_t& operator*() { return codepoint; }
 private:
  char32_t codepoint;
  const char* next;
};

utf8::string_decoder_helper::string_decoder_helper(const char* str) : str(str) {}

utf8::string_decoder_helper::iterator utf8::string_decoder_helper::begin() {
  return iterator(str);
}

utf8::string_decoder_helper::iterator utf8::string_decoder_helper::end() {
  return iterator(nullptr);
}

utf8::string_decoder_helper utf8::decoder(const char* str) {
  return string_decoder_helper(str);
}

utf8::string_decoder_helper utf8::decoder(const std::string& str) {
  return string_decoder_helper(str.c_str());
}

class utf8::buffer_decoder_helper::iterator : public std::iterator<std::input_iterator_tag, char32_t> {
 public:
  iterator(const char* str, size_t len) : codepoint(0), next(str), len(len) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next), len(it.len) {}
  iterator& operator++() { if (next) { codepoint = decode(next, len); if (len <= 0) next = nullptr; } return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next == other.next; }
  bool operator!=(const iterator& other) const { return next != other.next; }
  const char32_t& operator*() { return codepoint; }
 private:
  char32_t codepoint;
  const char* next;
  size_t len;
};

utf8::buffer_decoder_helper::buffer_decoder_helper(const char* str, size_t len) : str(str), len(len) {}

utf8::buffer_decoder_helper::iterator utf8::buffer_decoder_helper::begin() {
  return iterator(str, len);
}

utf8::buffer_decoder_helper::iterator utf8::buffer_decoder_helper::end() {
  return iterator(nullptr, 0);
}

utf8::buffer_decoder_helper utf8::decoder(const char* str, size_t len) {
  return buffer_decoder_helper(str, len);
}

void utf8::append(char*& str, char32_t chr) {
  if (chr < 0x80) *str++ = chr;
  else if (chr < 0x800) { *str++ = 0xC0 + (chr >> 6); *str++ = 0x80 + (chr & 0x3F); }
  else if (chr < 0x10000) { *str++ = 0xE0 + (chr >> 12); *str++ = 0x80 + ((chr >> 6) & 0x3F); *str++ = 0x80 + (chr & 0x3F); }
  else if (chr < 0x200000) { *str++ = 0xF0 + (chr >> 18); *str++ = 0x80 + ((chr >> 12) & 0x3F); *str++ = 0x80 + ((chr >> 6) & 0x3F); *str++ = 0x80 + (chr & 0x3F); }
  else *str++ = REPLACEMENT_CHAR;
}

void utf8::append(std::string& str, char32_t chr) {
  if (chr < 0x80) str += chr;
  else if (chr < 0x800) { str += 0xC0 + (chr >> 6); str += 0x80 + (chr & 0x3F); }
  else if (chr < 0x10000) { str += 0xE0 + (chr >> 12); str += 0x80 + ((chr >> 6) & 0x3F); str += 0x80 + (chr & 0x3F); }
  else if (chr < 0x200000) { str += 0xF0 + (chr >> 18); str += 0x80 + ((chr >> 12) & 0x3F); str += 0x80 + ((chr >> 6) & 0x3F); str += 0x80 + (chr & 0x3F); }
  else str += REPLACEMENT_CHAR;
}

template<class F> void utf8::map(F f, const char* str, std::string& result) {
  result.clear();

  for (char32_t chr; (chr = decode(str)); )
    append(result, f(chr));
}

template<class F> void utf8::map(F f, const char* str, size_t len, std::string& result) {
  result.clear();

  while (len)
    append(result, f(decode(str, len)));
}

template<class F> void utf8::map(F f, const std::string& str, std::string& result) {
  map(f, str.c_str(), result);
}

} // namespace unilib
} // namespace ufal
