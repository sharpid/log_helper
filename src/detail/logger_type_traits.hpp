#pragma once
///////////////////////////////////////////////////////////////////////////////
// type trait for stl container & has tostring func
// using SFINAE
// clang-format off

namespace log_helper{
// stl container
template <typename... T>
struct template_helper {};

template <typename T, typename _ = void>
struct is_stl_container : std::false_type {};

template <typename T>
struct is_stl_container< T, typename std::conditional<true, void,
        template_helper<typename T::iterator,
                        decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end()) >>::type>
    : std::true_type {};

// has tostring func
template <typename T, typename _ = void>
struct has_camel_tostring : std::false_type {};

template <typename T>
struct has_camel_tostring< T, typename std::conditional<true, void,
        template_helper<decltype(std::declval<T>().ToString()) >>::type>
    : std::true_type {};

template <typename T, typename _ = void>
struct has_snake_tostring : std::false_type {};

template <typename T>
struct has_snake_tostring< T, typename std::conditional<true, void,
        template_helper<decltype(std::declval<T>().to_string()) >>::type>
    : std::true_type {};

//std ptr type
template <typename T>
struct is_stl_ptr : std::false_type{};

template <typename T>
struct is_stl_ptr<std::shared_ptr<T>> : std::true_type{};

template <typename T>
struct is_stl_ptr<std::unique_ptr<T>> : std::true_type{};

//std weak ptr type
template <typename T>
struct is_stl_weak_ptr : std::false_type{};

template <typename T>
struct is_stl_weak_ptr<std::weak_ptr<T>> : std::true_type{};

}; //namespace logger
// clang-format on
