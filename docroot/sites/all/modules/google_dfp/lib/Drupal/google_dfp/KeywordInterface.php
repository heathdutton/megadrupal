<?php

/**
 * @file
 * Contains GoogleDfpKeywordInterface.
 */

namespace Drupal\google_dfp;

/**
 * Defines an interface for plugins defining ad tiers.
 */
interface KeywordInterface extends PluginInterface {

  /**
   * Returns keywords for this plugin.
   *
   * @return array
   *   The active keywords.
   */
  public function getKeywords();

}
