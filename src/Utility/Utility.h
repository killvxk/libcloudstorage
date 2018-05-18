/*****************************************************************************
 * Utility.h : prototypes for utility functions
 *
 *****************************************************************************
 * Copyright (C) 2016-2016 VideoLAN
 *
 * Authors: Paweł Wegner <pawel.wegner95@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "IHttpServer.h"
#include "IItem.h"

#ifdef _WIN32
#include <windows.h>

#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#define WINRT
#endif

#endif

namespace Json {
class Value;
}  // namespace Json

namespace cloudstorage {

struct Range;

bool operator==(const Range&, const Range&);
bool operator!=(const Range&, const Range&);

namespace util {

using Output = std::shared_ptr<std::ostream>;
using Input = std::shared_ptr<std::istream>;

struct FileId {
  FileId(bool, const std::string&);
  FileId(const std::string&);

  operator std::string() const;

  bool folder_;
  std::string id_;
};

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T, class U = T>
T exchange(T& obj, U&& new_value) {
  T old_value = std::move(obj);
  obj = std::forward<U>(new_value);
  return old_value;
}

std::string to_lower(std::string);
std::string remove_whitespace(const std::string& str);
Range parse_range(const std::string& str);
std::unordered_map<std::string, std::string> parse_form(const std::string& str);
std::unordered_map<std::string, std::string> parse_cookie(
    const std::string& cookie);
std::string range_to_string(Range);
std::string to_mime_type(const std::string& extension);
IItem::TimeStamp parse_time(const std::string& time);
std::string login_page(const std::string& provider);
std::string success_page(const std::string& provider);
std::string error_page(const std::string& provider);
const char* libcloudstorage_ascii_art();
namespace json {
std::string to_string(const Json::Value&);
Json::Value from_string(const std::string&);
Json::Value from_stream(std::istream&&);
Json::Value from_stream(std::istream&);
}  // namespace json

class CLOUDSTORAGE_API Url {
 public:
  Url(const std::string&);

  static std::string unescape(const std::string&);
  static std::string escape(const std::string&);
  static std::string escapeHeader(const std::string&);

  std::string protocol() const;
  std::string host() const;
  std::string path() const;
  std::string query() const;

 private:
  struct {
    std::string protocol_;
    std::string host_;
    std::string path_;
    std::string query_;
  } data_;
};

CLOUDSTORAGE_API IHttpServer::IResponse::Pointer response_from_string(
    const IHttpServer::IRequest&, int code,
    const IHttpServer::IResponse::Headers&, const std::string&);

CLOUDSTORAGE_API std::string temporary_directory();
CLOUDSTORAGE_API std::string home_directory();
CLOUDSTORAGE_API std::tm gmtime(time_t);
CLOUDSTORAGE_API time_t timegm(const std::tm&);
CLOUDSTORAGE_API std::string to_base64(const std::string&);
CLOUDSTORAGE_API std::string from_base64(const std::string&);

namespace Error {

constexpr auto INVALID_XML = "invalid xml";
constexpr auto FAILED_TO_PARSE_XML = "failed to parse xml";
constexpr auto INVALID_CREDENTIALS = "invalid credentials";
constexpr auto INVALID_AUTHORIZATION_CODE = "invalid authorization code";
constexpr auto PLAYERS_NOT_FOUND = "players not found";
constexpr auto COULD_NOT_FIND_DECIPHER_CODE = "couldn't find decipher code";
constexpr auto COULD_NOT_FIND_PACKED_SCRIPT = "couldn't find packed script";
constexpr auto COULD_NOT_EXTRACT_PACKED_ARGUMENTS =
    "couldn't extract packed arguments script";
constexpr auto COULD_NOT_FIND_MP4_URL = "couldn't find mp4 url";
constexpr auto COULD_NOT_FIND_SESSION_TOKEN = "couldn't find session token";
constexpr auto UNKNOWN_RESPONSE_RECEIVED = "unknown response received";
constexpr auto UNSUPPORTED_PLAYER = "unsupported player";
constexpr auto COULD_NOT_READ_FILE = "couldn't read file";
constexpr auto INVALID_NODE = "invalid node";
constexpr auto INVALID_RANGE = "invalid range";
constexpr auto INVALID_REQUEST = "invalid request";
constexpr auto INVALID_KIND = "invalid kind";
constexpr auto ABORTED = "aborted";
constexpr auto NODE_NOT_FOUND = "node not found";
constexpr auto COULD_NOT_FIND_DESCRAMBLER_NAME =
    "couldn't find descrambler name";
constexpr auto COULD_NOT_FIND_HELPER_FUNCTIONS =
    "couldn't find helper functions";
constexpr auto COULD_NOT_FIND_DESCRABMLER_DEFINITION =
    "couldn't find descrambler definition";
constexpr auto INVALID_TRANSFORMATION_FUNCTION =
    "invalid transformation function";
constexpr auto UNKNOWN_TRANSFORMATION = "unknown transformation";
constexpr auto VIDEO_URL_NOT_FOUND = "video url not found";
constexpr auto AUDIO_URL_NOT_FOUND = "audio url not found";
constexpr auto NOT_A_DIRECTORY = "not a directory";
constexpr auto INVALID_RANGE_HEADER_RESPONSE = "invalid range header response";
constexpr auto INVALID_PATH = "invalid path";
constexpr auto ITEM_NOT_FOUND = "item not found";
constexpr auto URL_UNAVAILABLE = "url unavailable";
constexpr auto CALLBACK_NOT_SET = "callback not set";
constexpr auto RESOLVER_NOT_SET = "resolver not set";
constexpr auto COULD_NOT_START_HTTP_SERVER = "couldn't start http server";
constexpr auto YOUTUBE_CONFIG_NOT_FOUND = "ytplayer.config not found";
constexpr auto INVALID_RADIX_BASE = "invalid radix base";
constexpr auto UNIMPLEMENTED = "unimplemented";

}  // namespace Error

namespace priv {
CLOUDSTORAGE_API extern std::mutex stream_mutex;
CLOUDSTORAGE_API extern std::unique_ptr<std::ostream> stream;

template <class... Args>
void log() {}

template <class First, class... Rest>
void log(First&& t, Rest&&... rest) {
  *priv::stream << t << " ";
  log(std::forward<Rest>(rest)...);
}

}  // namespace priv

CLOUDSTORAGE_API void log_stream(std::unique_ptr<std::ostream> stream);

template <class... Args>
void log(Args&&... t) {
  std::lock_guard<std::mutex> lock(priv::stream_mutex);
  std::time_t time = std::time(nullptr);
  auto tm = util::gmtime(time);
  *priv::stream << "[" << std::put_time(&tm, "%D %T") << "] ";
  priv::log(std::forward<Args>(t)...);
  *priv::stream << std::endl;
}

template <class Key, class Value>
class LRUCache {
 public:
  LRUCache(size_t size) : size_(size), time_() {}

  std::shared_ptr<Value> get(const Key& key) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto it = data_.find(key);
    if (it == data_.end())
      return nullptr;
    else {
      mark(key, time_);
      time_++;
      return it->second.first;
    }
  }

  void put(const Key& key, const std::shared_ptr<Value>& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto data_it = data_.find(key);
    if (data_it != data_.end()) {
      mark(key, time_);
      time_++;
      return;
    }
    data_[key] = {value, time_};
    access_time_[time_++] = key;
    if (data_.size() > size_) {
      auto access_it = access_time_.begin();
      auto data_it = data_.find(access_it->second);
      access_time_.erase(access_it);
      data_.erase(data_it);
    }
  }

 private:
  void mark(const Key& key, uint32_t time) {
    auto data_it = data_.find(key);
    auto access_it = access_time_.find(data_it->second.second);
    access_time_.erase(access_it);
    data_it->second.second = time;
    access_time_[time] = key;
  }

  std::mutex mutex_;
  std::unordered_map<Key, std::pair<std::shared_ptr<Value>, uint32_t>> data_;
  std::map<uint32_t, Key> access_time_;
  size_t size_;
  uint32_t time_;
};

}  // namespace util

}  // namespace cloudstorage

#endif  // UTILITY_H
