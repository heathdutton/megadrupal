<?php
/**
 * @file
 * Zeitgeist enumeration classes.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Ancestor for Zeitgeist enumeration classes.
 */
abstract class Enum {
  /**
   * Return the list of constants in the called class.
   *
   * Ignore constants named VAR* or DEF*.
   *
   * @return array
   *   A name-indexed hash of constant values.
   */
  public static function getConstants() {
    $reflected = new \ReflectionClass(get_called_class());
    $raw = $reflected->getConstants();
    foreach ($raw as $var => $name) {
      if (strpos($var, 'VAR') === 0 || strpos($var, 'DEF') === 0) {
        break;
      }
      $ret[$var] = $name;
    }
    return $ret;
  }
}
