<?php

/**
 * @file
 * Defines a base ad tier.
 */

namespace Drupal\google_dfp;

/**
 * A base ad tier plugin.
 */
abstract class TierBase extends PluginBase {

  /**
   * Utility function to filter a tier into a js/css safe name.
   *
   * @param string $value
   *   Input value to filter.
   *
   * @return string
   *   The filtered value.
   */
  protected static function filter($value) {
    return drupal_strtolower(drupal_clean_css_identifier(check_plain($value),
      array(' ' => '-', '/' => '-', '[' => '-', ']' => '')));
  }

}
