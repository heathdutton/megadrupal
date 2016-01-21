<?php

/**
 * @file
 * Defines a test page title ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

/**
 * A page title ad tier plugin.
 */
class TestPageTitle extends PageTitle {

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

  /**
   * {@inheritdoc}
   */
  protected function getPageTitle() {
    static $calls;
    if (!isset($calls)) {
      $calls = TRUE;
      return 'Drupal';
    }
    return FALSE;
  }

}
