<?php

/**
 * @file
 * Definition of WoW\character\CharacterClass.
 */

/**
 * Defines the character class.
 */
class WoWCharacterClass extends Entity {

  /**
   * The character class ID.
   *
   * @var integer
   */
  public $id;

  /**
   * The character class bitmask.
   *
   * @var integer
   */
  public $mask;

  /**
   * The character class power type: focus, mana, energy, rage, runic-power.
   *
   * @var string
   */
  public $powerType;
}
