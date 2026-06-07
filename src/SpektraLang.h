#pragma once

#include <cstdint>
#include <cstring>

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#  include <winnls.h>
#elif defined(__APPLE__)
#  include <CoreFoundation/CoreFoundation.h>
#else
#  include <locale>
#endif

#include "SpektraGeneratedTranslations.h"

namespace spektrafilm {

inline const char *hostLangCode() {
#if defined(_WIN32)
  static char buf[16] = {};
  if (buf[0] != '\0') return buf;

  WCHAR wbuf[16] = {};
  if (GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SISO639LANGNAME, wbuf, 16) > 0) {
    int len = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, 16, nullptr, nullptr);
    if (len > 0) {
      for (int i = 0; buf[i]; ++i) buf[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(buf[i])));
      return buf;
    }
  }
  buf[0] = 'e'; buf[1] = 'n'; buf[2] = '\0';
  return buf;
#elif defined(__APPLE__)
  static char buf[16] = {};
  if (buf[0] != '\0') return buf;

  CFArrayRef langs = CFLocaleCopyPreferredLanguages();
  if (langs && CFArrayGetCount(langs) > 0) {
    CFStringRef lang = static_cast<CFStringRef>(CFArrayGetValueAtIndex(langs, 0));
    if (lang) {
      CFStringGetCString(lang, buf, 16, kCFStringEncodingUTF8);
    }
  }
  if (langs) CFRelease(langs);
  if (buf[0] == '\0') { buf[0]='e'; buf[1]='n'; buf[2]='\0'; }
  return buf;
#else
  static char buf[16] = {};
  if (buf[0] != '\0') return buf;
  try {
    std::string loc = std::locale("").name();
    if (loc.size() >= 2 && loc.size() < 15) {
      for (size_t i = 0; i < 2 && i < loc.size(); ++i) buf[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(loc[i])));
      buf[loc[0]=='"'?0:2] = '\0';
    }
  } catch (...) {}
  if (buf[0] == '\0') { buf[0]='e'; buf[1]='n'; buf[2]='\0'; }
  return buf;
#endif
}

inline Language detectLanguage(const char *ofxLang) {
  const char *code = ofxLang && ofxLang[0] ? ofxLang : hostLangCode();
  if (!code || !code[0]) return Language::EN;

  if (std::strcmp(code, "zh-CN") == 0 || std::strcmp(code, "zh") == 0 ||
      std::strcmp(code, "zh-Hans") == 0 || std::strcmp(code, "zh_hans") == 0 ||
      std::strcmp(code, "chinese") == 0) {
    return Language::ZH_CN;
  }
  if (std::strcmp(code, "zh-TW") == 0 || std::strcmp(code, "zh-Hant") == 0 ||
      std::strcmp(code, "zh_hant") == 0 || std::strcmp(code, "zh-HK") == 0) {
    return Language::ZH_TW;
  }
  if (std::strcmp(code, "ja") == 0 || std::strcmp(code, "japanese") == 0) {
    return Language::JA;
  }
  if (std::strcmp(code, "ko") == 0 || std::strcmp(code, "korean") == 0) {
    return Language::KO;
  }
  return Language::EN;
}

} // namespace spektrafilm
