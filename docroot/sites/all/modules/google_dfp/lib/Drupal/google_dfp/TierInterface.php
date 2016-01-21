<?php

/**
 * @file
 * Contains GoogleDfpTierInterface.
 */

namespace Drupal\google_dfp;

/**
 * Defines an interface for plugins defining ad tiers.
 */
interface TierInterface extends PluginInterface {

  /**
   * Returns tiers for this plugin.
   *
   * @return string|array
   *   The active tier or an array of tiers.
   */
  public function getTier();

}
