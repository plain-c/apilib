#ifndef strmem_h__
#define strmem_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  char* stralloc_a(size_t length);
  wchar_t* stralloc_w(size_t length);

  void strfree_a(char* string);
  void strfree_w(wchar_t* string);

  char* strdup_a(const char* string);
  wchar_t* strdup_w(const wchar_t* string);

  char* strdup_na(const char* string, size_t length);
  wchar_t* strdup_nw(const wchar_t* string, size_t length);

  bool stradd_a(char** dst, const char* src);
  bool stradd_w(wchar_t** dst, const wchar_t* src);

  bool stradd_na(char** dst, const char* src, size_t maxcnt);
  bool stradd_nw(wchar_t** dst, const wchar_t* src, size_t maxcnt);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // strmem_h__