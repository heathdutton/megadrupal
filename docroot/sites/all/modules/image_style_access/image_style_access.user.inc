<?php
/**
 * @file
 * Basic user-related access control implementation.
 */

/**
 * User access control factory.
 */
class ImageStyleAccessUserControlFactory implements ImageStyleAccessControlFactoryInterface {
  /**
   * {@inheritdoc}
   */
  public function createControl($style, $scheme, $target) {
    return new ImageStyleAccessUserControl();
  }
}

/**
 * User access control.
 */
class ImageStyleAccessUserControl implements ImageStyleAccessControlInterface {
  /**
   * Checks whether the current user is the configured user.
   */
  public function match(ImageStyleAccessConfig $config) {
    if (!empty($config->settings['uid']) && user_is_logged_in()) {
      return $config->settings['uid'] == $GLOBALS['user']->uid;
    }
  }
}

/**
 * User role access control factory.
 */
class ImageStyleAccessUserRoleControlFactory implements ImageStyleAccessControlFactoryInterface {
  /**
   * {@inheritdoc}
   */
  public function createControl($style, $scheme, $target) {
    return new ImageStyleAccessUserRoleControl();
  }
}

/**
 * User role access control.
 */
class ImageStyleAccessUserRoleControl implements ImageStyleAccessControlInterface {
  /**
   * Checks whether the current user is the configured user.
   */
  public function match(ImageStyleAccessConfig $config) {
    if (!empty($config->settings['roles']) && !empty($GLOBALS['user']->roles)) {
      return (bool) array_intersect_key(array_filter($config->settings['roles']), $GLOBALS['user']->roles);
    }
  }
}
