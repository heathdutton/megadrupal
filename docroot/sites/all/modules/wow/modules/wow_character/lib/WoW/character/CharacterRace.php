<?php

/**
 * @file
 * Definition of WoW\character\CharacterRace.
 */

/**
 * Defines the character race class.
 */
class WoWCharacterRace extends Entity {

  /**
   * The character race ID.
   *
   * @var integer
   */
  public $id;

  /**
   * The character race bitmask.
   *
   * @var integer
   */
  public $mask;

  /**
   * The character race side. Whether horde or alliance.
   *
   * @var string
   */
  public $side;
}
