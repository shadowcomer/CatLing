#pragma once
#include <BWAPI/Type.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  class UnitType;
  class WeaponType;
  class Order;
  class Race;

  namespace TechTypes
  {
    /// Enumeration of Tech Types
    namespace Enum
    {
      /// Enumeration of Tech Types
      enum Enum
      {
        Stim_Packs = 0,
        Lockdown,
        EMP_Shockwave,
        Spider_Mines,
        Scanner_Sweep,
        Tank_Siege_Mode,
        Defensive_Matrix,
        Irradiate,
        Yamato_Gun,
        Cloaking_Field,
        Personnel_Cloaking,
        Burrowing,
        Infestation,
        Spawn_Broodlings,
        Dark_Swarm,
        Plague,
        Consume,
        Ensnare,
        Parasite,
        Psionic_Storm,
        Hallucination,
        Recall,
        Stasis_Field,
        Archon_Warp,
        Restoration,
        Disruption_Web,
        Unused_26,
        Mind_Control,
        Dark_Archon_Meld,
        Feedback,
        Optical_Flare,
        Maelstrom,
        Lurker_Aspect,
        Unused_33,
        Healing,

        None = 44,
        Nuclear_Strike,
        Unknown,
        MAX
      };
    };
  }
  /// The TechType (or Technology Type, also referred to as an Ability) represents a Unit's ability
  /// which can be researched with UnitInterface::research or used with UnitInterface::useTech. In order for a Unit
  /// to use its own specialized ability, it must first be available and researched.
  class TechType : public Type<TechType, TechTypes::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      TechType(int id = TechTypes::Enum::None);

      /// Retrieves the race that is required to research or use the TechType.
      ///
      /// @note There is an exception where @Infested_Kerrigan can use @Psi_Storm. This does not
      /// apply to the behavior of this function.
      ///
      /// @returns Race object indicating which race is designed to use this technology type.
      Race getRace() const;

      /// Retrieves the mineral cost of researching this technology.
      ///
      /// @returns Amount of minerals needed in order to research this technology.
      int mineralPrice() const;

      /// Retrieves the vespene gas cost of researching this technology.
      ///
      /// @returns Amount of vespene gas needed in order to research this technology.
      int gasPrice() const;

      /// Retrieves the number of frames needed to research the tech type.
      ///
      /// @returns The time, in frames, it will take for the research to complete.
      /// @see UnitInterface::getRemainingResearchTime
      int researchTime() const;

      /// Retrieves the amount of energy needed to use this TechType as an ability.
      ///
      /// @returns Energy cost of the ability.
      /// @see UnitInterface::getEnergy
      int energyCost() const;

      /// Retrieves the UnitType that can research this technology.
      ///
      /// @returns UnitType that is able to research the technology in the game.
      /// @retval UnitTypes::None If the technology/ability is either provided for free or never
      /// available.
      UnitType whatResearches() const;

      /// Retrieves the Weapon that is attached to this tech type. A technology's WeaponType
      /// is used to indicate the range and behaviour of the ability when used by a Unit.
      ///
      /// @returns WeaponType containing information about the ability's behavior.
      /// @retval WeaponTypes::None If there is no corresponding WeaponType.
      WeaponType getWeapon() const;

      /// Checks if this ability can be used on other units.
      ///
      /// @returns true if the ability can be used on other units, and false if it can not.
      bool targetsUnit() const;

      /// Checks if this ability can be used on the terrain (ground).
      ///
      /// @returns true if the ability can be used on the terrain.
      bool targetsPosition() const;

      /// Retrieves the set of all UnitTypes that are capable of using this ability.
      ///
      /// @returns Set of UnitTypes that can use this ability when researched.
      const UnitType::set& whatUses() const;

      /// Retrieves the Order that a Unit uses when using this ability.
      ///
      /// @returns Order representing the action a Unit uses to perform this ability
      Order getOrder() const;
  };
  /// Namespace containing tech types
  namespace TechTypes
  {
    /// Retrieves the set of all the TechTypes.
    ///
    /// @returns Set of all available TechTypes.
    const TechType::set& allTechTypes();

    extern const TechType Stim_Packs;
    extern const TechType Lockdown;
    extern const TechType EMP_Shockwave;
    extern const TechType Spider_Mines;
    extern const TechType Scanner_Sweep;
    extern const TechType Tank_Siege_Mode;
    extern const TechType Defensive_Matrix;
    extern const TechType Irradiate;
    extern const TechType Yamato_Gun;
    extern const TechType Cloaking_Field;
    extern const TechType Personnel_Cloaking;
    extern const TechType Burrowing;
    extern const TechType Infestation;
    extern const TechType Spawn_Broodlings;
    extern const TechType Dark_Swarm;
    extern const TechType Plague;
    extern const TechType Consume;
    extern const TechType Ensnare;
    extern const TechType Parasite;
    extern const TechType Psionic_Storm;
    extern const TechType Hallucination;
    extern const TechType Recall;
    extern const TechType Stasis_Field;
    extern const TechType Archon_Warp;
    extern const TechType Restoration;
    extern const TechType Disruption_Web;
    extern const TechType Mind_Control;
    extern const TechType Dark_Archon_Meld;
    extern const TechType Feedback;
    extern const TechType Optical_Flare;
    extern const TechType Maelstrom;
    extern const TechType Lurker_Aspect;
    extern const TechType Healing;
    extern const TechType None;
    extern const TechType Nuclear_Strike;
    extern const TechType Unknown;
  };

  static_assert(sizeof(TechType) == sizeof(int), "Expected type to resolve to primitive size.");
}
