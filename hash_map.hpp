#ifndef CLI_BASIC_ENGINE_HASH_MAP_HPP
#define CLI_BASIC_ENGINE_HASH_MAP_HPP

#include <unordered_map>


namespace cli {

  //! Insensitive comparator
  struct InsensitiveEqual {

    /*!
     * @brief   Compare a pair of strings insensitively
     * @retval  true  : two strings are insensitively the same
     * @retval  false : two strings are obviously different
     * @note    It enables insensitive comparison of string.
     * @code
     * std::string str1 = "test";
     * std::string str2 = "TEST";
     *
     * // standard comparison
     * assert( str1 != str2 );
     *
     * // with this comparator
     * InsensitiveEqual comp;
     * assert( comp(str1,str2) == true );
     * @endcode
     */
    bool operator()(const std::string&, const std::string&) const noexcept;

  };


  //! Insensitive hash generator for string
  struct InsensitiveHash {

    /*!
     * @typedef  result_type
     * @brief    hash value type
     */
    using result_type = size_t;

    /*!
     * @brief   Generates hash value from the string
     * @return  integral hash value
     * @note    It enables insensitive comparison of string.
     * @code
     * std::string str1 = "test";
     * std::string str2 = "TEST";
     *
     * // they are different according to the standard hash generator
     * std::Hash<std::string> std_hash;
     * assert( std_hash(str1) != std_hash(str2) );
     *
     * // they are the same string according to insensitive comparator
     * InsensitiveHash ist_hash;
     * assert(ist_hash(str1) == ist_hash(str2) );
     * @endcode
     */
    result_type operator()(std::string) const noexcept;

  };


  /*!
   * @typedef HashMap
   * @brief   Key-value map with insensitive string keys
   */
  template<typename ValueType>
  using HashMap = std::unordered_map<std::string, ValueType, InsensitiveHash, InsensitiveEqual>;

}

#endif  /* CLI_BASIC_ENGINE_HASH_MAP_HPP */
