<?php

/**
 * @file
 * Defines a placement name ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

/**
 * A placement tier plugin.
 */
class TestPlacement extends Placement {

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

}
