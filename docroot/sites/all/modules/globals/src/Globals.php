<?php

/**
 * @file
 * Contains a Globals controller.
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\globals;

use Drupal\ghost\Exception\GhostException;
use Drupal\ghost\Traits\InitialiserTrait;

/**
 * Class Globals
 *
 * @package Drupal\globals
 */
class Globals {

  use InitialiserTrait;

  /**
   * Fetch all property definitions.
   *
   * @param bool $reset
   *   If TRUE, flush the static variable cache.
   *
   * @return GlobalItem[]
   *   An array of GlobalItems
   */
  public function getGlobalProperties($reset = FALSE) {
    $properties = drupal_static('globals_globals', array());

    if (empty($properties) || $reset == TRUE) {
      $settings = module_invoke_all('globals');

      drupal_alter('globals', $settings);

      foreach ($settings as $key => $item) {
        $properties[$key] = GlobalItem::init($item['key'], $item);
      }
    }

    return $properties;
  }

  /**
   * Reguest an individual property setting.
   *
   * @param string $key
   *   Name of the propery
   *
   * @return GlobalItem
   *   An item.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getGlobalProperty($key) {
    $properties = $this->getGlobalProperties();
    if (array_key_exists($key, $properties)) {
      return $properties[$key];
    }

    throw new GhostException('Invalid global property key requested');
  }

  /**
   * Get a global value.
   *
   * @param string $key
   *   Global value key
   *
   * @return mixed
   *   Result of the query.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getGlobalValue($key) {
    return $this->getGlobalProperty($key)->getValue();
  }

  /**
   * Get a global value filtered.
   *
   * @param string $key
   *   Global value key
   * @param string $filter
   *   Filter name. Defaults to 'check_plain'. If you provide another filter
   *   value, its up to you to ensure the output is secure.
   *
   * @return mixed
   *   Result of the query.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getFilteredValue($key, $filter = 'check_plain') {
    return $this->getGlobalProperty($key)->getFilteredValue($filter);
  }

  /**
   * Get the default value.
   *
   * @param string $key
   *   Global value key
   *
   * @return mixed|null
   *   The default value.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getDefaultValue($key) {
    return $this->getGlobalProperty($key)->getDefaultValue();
  }
}
