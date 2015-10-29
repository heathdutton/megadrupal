<?php

/**
 * @file
 * Interface for Zeitgeist block classes.
 *
 * This is an extension of Drupal core blocks behaviours, with internal caching
 * which won't stop working when node access is enabled.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Common methods on Block classes.
 */
interface BlockInterface {
  /**
   * Return the single instance of a block class for a given delta.
   *
   * @param string $delta
   *   The Drupal core block delta.
   *
   * @return Drupal\zeitgeist\BlockInterface
   *   A concrete block instance.
   */
  public static function instance($delta);

  /**
   * Return the expiration for a new cached block.
   *
   * @return int
   *   A UNIX timestamp.
   */
  public function expire();

  /**
   * Delegated implementation of the block_configure hook.
   */
  public function configure();

  /**
   * Delegated implementation of the block_info hook.
   */
  public function info();

  /**
   * Delegated implementation of the block_save hook.
   */
  public function save($delta = '', $edit = array());

  /**
   * Delegated implementation of the block_view hook.
   *
   * @return string|array
   *   HTML or render array.
   */
  public function view();

  /**
   * Zeitgeist raw display.
   *
   * @return string|array
   *   HTML or render array.
   */
  public function viewUncached();
}
