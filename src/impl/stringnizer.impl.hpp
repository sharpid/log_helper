#pragma once
namespace log_helper{
///////////////////////////////////////////////////////////////////////////////
template <typename... Types>
inline std::string Stringnizer::ToString(const Types&... _args) {
  buffer_->clear();
  __separate_log_arguments(_args...);
  return *buffer_;
}

template <typename Iter>
inline std::string Stringnizer::RangeToString(const Iter& _begin,
                                               const Iter& _end) {
  buffer_->clear();
  *buffer_ += "[";
  for (auto it = _begin; it != _end;) {
    *buffer_ += __to_string<typename std::remove_pointer<
        typename std::iterator_traits<Iter>::value_type>::type>()(*it);
    if (++it != _end)
      *buffer_ += ",";
  }
  *buffer_ += "]";

  return *buffer_;
}

///////////////////////////////////////////////////////////////////////////////
// specialize
template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::ARITHMETIC> {
  inline std::string operator()(const T& _v) { return std::to_string(_v); }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::STL_CONTAINER> {
  inline std::string operator()(const T& _v) {
    std::string ret;
    ret += "[";
    for (auto it = _v.begin(); it != _v.end();) {
      ret += __to_string<
          typename std::remove_pointer<typename T::value_type>::type>()(*it);
      if (++it != _v.end())
        ret += ",";
    }
    ret += "]";
    return ret;
  }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::STL_PTR> {
  inline std::string operator()(const T& _v) { return __to_string<typename T::element_type>()(*_v) ;}
  inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::STL_WEAK_PTR> {
  inline std::string operator()(const T& _v) { 
    if(auto shared = _v.lock()) {
      return __to_string<typename T::element_type>()(*shared);
    }
    else return "weak_ptr has expired.";
  }
  inline std::string operator()(const T* _v) { return (!_v) ? "null" : operator()(*_v); }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::HAS_CAMEL_TOSTRING> {
  inline std::string operator()(const T& _v) { return const_cast<T*>(&_v)->ToString(); }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::HAS_SNAKE_TOSTRING> {
  inline std::string operator()(const T& _v) { return const_cast<T*>(&_v)->to_string(); }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::NONE> {
  inline std::string operator()(const T& _v) {
    std::stringstream ret;
    ret << typeid(T).name()<< ":" << std::hex << &_v;
    return ret.str();
  }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

#ifdef _USE_GOOGLE_PROTOBUF_
template <typename T>
struct Stringnizer::__to_string<T, Stringnizer::ARG_TYPE::GOOGLE_PROTO_ENUM> {
  inline std::string operator()(const T& _v) {
    return ::google::protobuf::internal::NameOfEnum(
        ::google::protobuf::GetEnumDescriptor<T>(), _v);
  }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename T>
struct Stringnizer::__to_string<T,
                                 Stringnizer::ARG_TYPE::GOOGLE_PROTO_MESSAGE> {
  inline std::string operator()(const T& _v) { return _v.ShortDebugString(); }
  inline std::string operator()(const T* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};
#endif  //_USE_GOOGLE_PROTOBUF_

// string and char
template <>
struct Stringnizer::__to_string<std::string> {
  inline std::string operator()(const std::string& _v) { return _v; }
  inline std::string operator()(const std::string* _v) {
    return (!_v) ? "null" : operator()(*_v);
  };
};

template <>
struct Stringnizer::__to_string<const char*> {
  inline std::string operator()(const char* const& _v) {
    return (!_v) ? "null" : std::string(_v);
  }
};

template <>
struct Stringnizer::__to_string<std::ostringstream> {
  inline std::string operator()(const std::ostringstream& _v) {
    return _v.str();
  }
  inline std::string operator()(const std::ostringstream* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

///////////////////////////////////////////////////////////////////////////////
// pair and tuple
template <typename... VA>
struct Stringnizer::__to_string<std::pair<VA...>,
                                 Stringnizer::ARG_TYPE::NONE> {
  inline std::string operator()(const std::pair<VA...>& _v) {
    // std::tuple_size<std::pair<VA...>>
    std::string ret;
    ret += "{";
    ret +=
        __to_string<typename std::tuple_element<0, std::pair<VA...>>::type>()(
            _v.first);
    ret += ",";
    ret +=
        __to_string<typename std::tuple_element<1, std::pair<VA...>>::type>()(
            _v.second);
    ret += "}";
    return ret;
  }
  inline std::string operator()(const std::pair<VA...>* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <typename... VA>
struct Stringnizer::__to_string<std::tuple<VA...>,
                                 Stringnizer::ARG_TYPE::NONE> {
  inline std::string operator()(const std::tuple<VA...>& _v) {
    std::string ret;
    ret += "{";
    const auto ts = std::tuple_size<const std::tuple<VA...>>::value - 1;
    ret += __tuple_decomposer__<ts, VA...>()(_v);
    ret += "}";
    return ret;
  }
  inline std::string operator()(const std::tuple<VA...>* _v) {
    return (!_v) ? "null" : operator()(*_v);
  }
};

template <size_t SZ, typename... VA>
struct Stringnizer::__tuple_decomposer__ {
  std::string operator()(const std::tuple<VA...>& t) {
    using current_type = typename std::remove_pointer<
        typename std::tuple_element<SZ, const std::tuple<VA...>>::type>::type;
    return __tuple_decomposer__<SZ - 1, VA...>()(t) + "," +
           __to_string<current_type>()(std::get<SZ>(t));
  }
};

template <typename... VA>
struct Stringnizer::__tuple_decomposer__<0, VA...> {
  std::string operator()(const std::tuple<VA...>& t) {
    using current_type = typename std::remove_pointer<
        typename std::tuple_element<0, const std::tuple<VA...>>::type>::type;
    return __to_string<current_type>()(std::get<0>(t));
  }
};

///////////////////////////////////////////////////////////////////////////////
// separate
template <typename T, typename... Types>
inline void Stringnizer::__separate_log_arguments(const T& _arg,
                                                   const Types&... _args) {
  *buffer_ += __to_string<typename std::remove_pointer<T>::type>()(_arg);
  __separate_log_arguments(_args...);
}

template <typename T>
inline void Stringnizer::__separate_log_arguments(const T& _arg) {
  *buffer_ += __to_string<typename std::remove_pointer<T>::type>()(_arg);
}

template <size_t N, typename... Types>
inline void Stringnizer::__separate_log_arguments(const char (&_arg)[N],
                                                   const Types&... _args) {
  *buffer_ += __to_string<const char*>()(_arg);
  __separate_log_arguments(_args...);
}

template <size_t N>
inline void Stringnizer::__separate_log_arguments(const char (&_arg)[N]) {
  *buffer_ += __to_string<const char*>()(_arg);
}
}; //namespace logger
