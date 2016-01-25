<?php

/**
 * @file
 * Class that loads Drupal\maps_import\Profile\Profile objects.
 */

namespace Drupal\maps_import\Cache\Object;

/**
 * The MaPS Import Cache class related to profiles.
 */
class Profile extends Object {

  /**
   * @inheritdoc
   */
  protected function useCacheBin() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  protected function getCacheKey() {
    return 'maps_import:profiles';
  }

  /**
   * @inheritdoc
   */
  protected function getTable() {
    return 'maps_import_profile';
  }

  /**
   * @inheritdoc
   */
  protected function getDefaultColumn() {
    return 'pid';
  }

  /**
   * @inheritdoc
   */
  protected function getInterfaceName() {
    // Actually there is no interface, but the class implements the getter
    // methods directly.
    return 'Drupal\\maps_import\\Profile\\Profile';
  }

  /**
   * @inheritdoc
   */
  protected function getClassName($data) {
    return 'Drupal\\maps_import\\Profile\\Profile';
  }

}
