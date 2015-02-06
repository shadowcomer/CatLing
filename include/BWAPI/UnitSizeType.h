#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  namespace UnitSizeTypes
  {
    /// Enumeration of unit size types
    namespace Enum
    {
      /// Enumeration of unit size types
      enum Enum
      {
        Independent = 0,
        Small,
        Medium,
        Large,
        None,
        Unknown,
        MAX
      };
    };
  };
  class UnitSizeType : public Type<UnitSizeType, UnitSizeTypes::Enum::Unknown>
  {
  public:
    /// @copydoc Type::Type(int)
    UnitSizeType(int id = UnitSizeTypes::Enum::None);
  };
  /// Namespace containing unit size types
  ///
  /// [View on Starcraft Campendium (Official Website)](http://classic.battle.net/scc/gs/damage.shtml)<br>
  namespace UnitSizeTypes
  {
    /// Retrieves the set of all UnitSizeTypes.
    ///
    /// @returns Set of all UnitSizeTypes.
    const UnitSizeType::set& allUnitSizeTypes();
    
    extern const UnitSizeType Independent;
    extern const UnitSizeType Small;
    extern const UnitSizeType Medium;
    extern const UnitSizeType Large;
    extern const UnitSizeType None;
    extern const UnitSizeType Unknown;
  }

  static_assert(sizeof(UnitSizeType) == sizeof(int), "Expected type to resolve to primitive size.");
}
