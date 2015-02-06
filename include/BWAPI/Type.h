#pragma once
#include <BWAPI/SetContainer.h>

#include <string>
#include <cctype>
#include <deque>
#include <unordered_map>
#include <algorithm>

namespace BWAPI
{
  template<class T, int UnknownId> class Type;

  /// Base superclass for all BWAPI Types.
  template<class T, int UnknownId>
  class Type
  {
  protected:
    /// Primitive storage unit for the type identifier (t id)
    int tid;

    /// Array of strings containing the type names.
    static const std::string typeNames[UnknownId+1];

    /// Type that maps names to their type values.
    typedef std::unordered_map<std::string,T> typeMapT;

  private:
    static typeMapT typeMapInit()
    {
      typeMapT result(UnknownId+1);
      for ( int i = 0; i < UnknownId + 1; ++i ) // include unknown
      {
        std::string n( typeNames[i] );

        // erase-remove idiom, eliminates spaces and underscores from the string
        n.erase( std::remove_if(n.begin(), n.end(), [](char const &c){ return isspace(c) || c == '_'; }), n.end() );
        
        // Make lowercase
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);

        result.insert( typeMapT::value_type(n, T(i)) );
      }
      return result;
    }
  public:
    /// Expected type constructor. If the type is an invalid type, then it becomes Types::Unknown.
    /// A type is invalid if its value is less than 0 or greater than Types::Unknown.
    ///
    /// @param id (optional)
    ///   The id that corresponds to this type. It is typically an integer value that corresponds
    ///   to an internal Broodwar type. If the given id is invalid, then it becomes Types::Unknown.
    ///   If it is omitted, then it becomes Types::None.
    explicit Type(int id) : tid( id < 0 || id > UnknownId ? UnknownId : id ) {};
    
    /// A set type that contains the current type.
    typedef SetContainer<T> set;

    /// A list type that contains the current type.
    typedef std::deque<T> list;

    /// Conversion/convenience operator to convert this type to its primitive type.
    inline operator int() const { return this->tid; };

    /// Retrieves this type's identifier.
    ///
    /// @returns An integer representation of this type.
    inline int getID() const { return this->tid; };

    /// Checks if the current type has a valid identifier. The purpose of this function is to
    /// prevent buffer overflows if a type has been handled improperly.
    ///
    /// A type is valid if it is between 0 and Unknown (inclusive).
    ///
    /// @returns true If this type is valid and false otherwise.
    inline bool isValid() const { return this->tid >= 0 && this->tid <= UnknownId; };

    /// Retrieves the variable name of the type.
    ///
    /// @returns Reference to std::string object containing the name.
    inline const std::string &getName() const
    {
      return typeNames[this->isValid() ? this->tid : UnknownId];
    };

    /// @copydoc Type::getName
    inline const std::string &toString() const
    {
      return this->getName();
    };

    /// Retrieves the variable name of the type as a c-style string. Meant to be a convenience
    /// member.
    ///
    /// @returns Pointer to constant c-style string containing the name.
    inline const char *c_str() const
    {
      return this->getName().c_str();
    };

    /// Output stream operator overload. Allows printing of the type without calling
    /// Type::getName.
    friend inline std::ostream &operator << (std::ostream &out, const Type<T, UnknownId> &t)
    {
      return out << t.getName();
    };
    friend inline std::wostream &operator << (std::wostream &out, const Type<T, UnknownId> &t)
    {
      std::wstring wideName{ t.getName().begin(), t.getName().end() };
      return out << wideName;
    };

    /// Searches for the type associated with the given string and returns it.
    ///
    /// @param name
    ///     A string containing the name of the type.
    ///
    /// @returns The type that resolves to the given name.
    static T getType(std::string name)
    {
      // Mapping of strings to types
      static const typeMapT typeMap( typeMapInit() );
    
      // erase-remove idiom, eliminates spaces and underscores from the string to search
      name.erase( std::remove_if(name.begin(), name.end(), [](char const &c){ return isspace(c) || c == '_'; }), name.end() );
      
      // Make lowercase
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);

      // Find the type
      auto it = typeMap.find(name);
      if ( it != typeMap.end() )
        return it->second;

      // Return unknown if it wasn't found
      return T(UnknownId);
    };
  };

}
