<?php
/**
 * @file
 * Contains a class providing a Extension Registry.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Registry;

use Drupal\extensions\Collection\Collection;

/**
 * Class Register
 *
 * @package Drupal\sourcery\Filters
 */
class Registry {

  /**
   * @var array
   */
  protected $sets = array();

  /**
   * Register a new filter.
   *
   * @param string $set_name
   *   Name of the set to add the extension to.
   * @param string $extension_name
   *   Machine name of the extension.
   * @param string|null $human_name
   *   A human readable name.
   */
  public function registerExtension($set_name, $extension_name, $human_name = NULL) {
    if (!isset($this->sets[$set_name])) {
      $this->setCollection($set_name);
    }

    $this->getCollection($set_name)->registerExtension($extension_name, $human_name);
  }

  /**
   * Test whether this registry actually has any collections.
   *
   * @return bool
   *   TRUE, if collections are present, or FALSE.
   */
  public function hasCollections() {
    if (isset($this->sets) && !empty($this->sets)) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Get a extension set.
   *
   * @param string $name
   *   Machine name of the extension set.
   *
   * @return \Drupal\extensions\Collection\Collection
   *   A extension set object.
   */
  public function getCollection($name) {
    if (isset($this->sets[$name])) {
      return $this->sets[$name];
    }

    return FALSE;
  }

  /**
   * Set a extension set.
   *
   * @param string $name
   *   Machine name of the extension set.
   */
  public function setCollection($name) {
    if (!isset($this->sets[$name])) {
      $this->sets[$name] = new Collection($name);
    }
  }
}