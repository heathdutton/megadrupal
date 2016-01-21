<?php

/**
 * A MenuTreeSource is an object reprenting a persistent structure that can
 * provide menu trees, and an "active trail" of paths that make some of these
 * menu items special.
 *
 * Usage:
 * - Use $source->setTrailPaths() to set the active trail.
 * - Use $source->build($settings) to load menu tree items and root mlid.
 */
interface menupoly_MenuTreeSource_Interface {

  /**
   * Set paths for the "active trail".
   *
   * @param array $paths
   *   Array of paths representing the "active trail".
   *   Some, all or none of these may be part of the menu.
   */
  function setTrailPaths(array $paths);

  /**
   * Load menu tree items and a root mlid, based on an array of settings.
   *
   * @param array $settings
   *   Settings array.
   *
   * @return array
   *   Array with two elements.
   *   The first element is the root mlid.
   *   The second element is the array of menu items found, by mlid.
   *   Each menu item may have a 'plid' key to identify a parent item.
   *   The order of menu items does not matter.
   */
  function build(array $settings);
}
