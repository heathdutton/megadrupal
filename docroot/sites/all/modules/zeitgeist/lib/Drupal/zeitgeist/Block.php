<?php

/**
 * @file
 * Zeitgeist base block class.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Parent class for Zeitgeist cached blocks.
 */
abstract class Block implements BlockInterface, ThemableInterface {
  public static $instances = array();

  public $cid;
  public $data = NULL;
  public $delta;
  public $duration;

  /**
   * Block instance retrieval method.
   *
   * @param string $delta
   *   The block delta.
   *
   * @return Drupal\zeitgeist\Block
   *   A Block (or child class) instance.
   */
  public static final function instance($delta) {
    if (empty(self::$instances[$delta])) {
      $class = 'Drupal\zeitgeist\Block' . drupal_ucfirst($delta);
      $ret = new $class($delta);
      self::$instances[$delta] = $ret;
    }
    else {
      $ret = self::$instances[$delta];
    }

    return $ret;
  }

  /**
   * Block constructor.
   *
   * @see Drupal\zeitgeist\Block::instance()
   *
   * @param string $delta
   *   The Drupal core block delta for a concrete block class.
   */
  public function __construct($delta) {
    $this->delta = $delta;
    $this->duration = $this->expire();

    $cid_parts = drupal_render_cid_parts(DRUPAL_CACHE_PER_ROLE);
    array_unshift($cid_parts, $this->delta);
    $this->cid = implode(':', $cid_parts);
  }

  /**
   * Delegated implementation of the block_configure hook.
   */
  public function configure() {
  }

  /**
   * Return the timestamp for a cache_set operation in this request.
   *
   * @return int
   *   A UNIX timestamp.
   */
  public function expire() {
    $variable = "zeitgeist_{$this->delta}_cache";
    $ret = REQUEST_TIME + 60 * variable_get($variable, 0);
    return $ret;
  }

  /**
   * Delegated implementation of the block_info hook.
   *
   * Since ZG blocks use internal caching, they declare DRUPAL_NO_CACHE to avoid
   * duplicate caching.
   */
  public abstract function info();

  /**
   * Delegated implementation of the block_save hook.
   */
  public function save($delta = '', $edit = array()) {
  }

  /**
   * Delegated implementation of the block_view hook.
   *
   * @return string|array
   *   HTML or render array.
   */
  public function view() {
    $cached = Cache::get($this->cid);
    if (empty($cached) || (REQUEST_TIME >= $cached->expire)) {
      $ret = $this->data = $this->viewUncached();
      Cache::set($this->cid, $this->data, $this->expire());
    }
    else {
      $ret = $cached->data;
    }
    return $ret;
  }

  /**
   * Return a freshly built block.
   *
   * @return array
   *   A subject/content hash.
   */
  public abstract function viewUncached();
}
