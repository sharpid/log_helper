#pragma once
#include <string>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <memory>
#ifdef _USE_GOOGLE_PROTOBUF_
#include <google/protobuf/message.h>
#endif  //_USE_GOOGLE_PROTOBUF_

#include "logger_type_traits.hpp"

namespace log_helper{
class Stringnizer {
 public:
  enum ARG_TYPE {
    NONE = 0,
    ARITHMETIC,
    STL_CONTAINER,
    HAS_CAMEL_TOSTRING,
    HAS_SNAKE_TOSTRING,

#ifdef _USE_GOOGLE_PROTOBUF_
    GOOGLE_PROTO_ENUM,
    GOOGLE_PROTO_MESSAGE,
#endif  //_USE_GOOGLE_PROTOBUF_

  };

 public:
  Stringnizer():buffer_(std::make_shared<std::string>()) { buffer_->reserve(reserve_buffer_size_); };
  Stringnizer(std::shared_ptr<std::string> _buffer):buffer_(_buffer) { buffer_->reserve(reserve_buffer_size_); };

 public:
  template <typename... Types>
  std::string ToString(const Types&... _args);

  template <typename Iter>
  std::string RangeToString(const Iter&, const Iter&);

 private:
  const size_t reserve_buffer_size_ = 1024;
  std::shared_ptr<std::string> buffer_;


// clang-format off
  template <typename T>
  constexpr static ARG_TYPE GetArgumentType() {
    return std::is_arithmetic<T>::value ? ARG_TYPE::ARITHMETIC
      : is_stl_container<T>::value ? ARG_TYPE::STL_CONTAINER
      : has_camel_tostring<T>::value ? ARG_TYPE::HAS_CAMEL_TOSTRING
      : has_snake_tostring<T>::value ? ARG_TYPE::HAS_SNAKE_TOSTRING
  #ifdef _USE_GOOGLE_PROTOBUF_
      : google::protobuf::is_proto_enum<typename std::remove_pointer<T>::type>::value ? ARG_TYPE::GOOGLE_PROTO_ENUM
      : std::is_convertible<typename std::add_pointer<T>::type, const ::google::protobuf::Message*>:: value ? ARG_TYPE::GOOGLE_PROTO_MESSAGE
  #endif
      : ARG_TYPE::NONE;
  }
// clang-format on


  ///////////////////////////////////////////////////////////////////////////////
  // to string template functors
  template <typename T, ARG_TYPE = GetArgumentType<T>()>
  struct __to_string;

  ///////////////////////////////////////////////////////////////////////////////
  // separate tuple arguments
  template <size_t SZ, typename... VA>
  struct __tuple_decomposer__;

  ///////////////////////////////////////////////////////////////////////////////
  // separate arguments
  template <typename T, typename... Types>
  void __separate_log_arguments(const T& _arg, const Types&... _args);
  template <typename T>
  void __separate_log_arguments(const T& _arg);

  template <size_t N, typename... Types>
  void __separate_log_arguments(const char (&_arg)[N], const Types&... _args);
  template <size_t N>
  void __separate_log_arguments(const char (&_arg)[N]);
}; // class Stringnizer
}; // namespace logger