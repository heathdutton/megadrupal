<?php

/**
 * @file
 * Defines a test site-wide ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

/**
 * A site-wide ad tier plugin.
 */
class TestSiteWide extends SiteWide {

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

}
