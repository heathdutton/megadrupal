<?php

/**
 * @file
 * Contains GoogleDfpInterface.
 */

namespace Drupal\google_dfp;

/**
 * An ad interface.
 */
interface GoogleDfpInterface {
  /**
   * Converts tier config into instances.
   *
   * @param bool $enabled_only
   *   (optional) TRUE to return only enabled plugins. Defaults to FALSE.
   *
   * @return array[\Drupal\google_dfp\TierInterface]
   *   An array of \Drupal\google_dfp\TierInterface objects.
   */
  public function getTierInstances($enabled_only = FALSE);

  /**
   * Converts keyword config into instances.
   *
   * @param bool $enabled_only
   *   (optional) TRUE to return only enabled plugins. Defaults to FALSE.
   *
   * @return array[\Drupal\google_dfp\KeywordInterface]
   *   An array of \Drupal\google_dfp\KeywordInterface objects.
   */
  public function getKeywordInstances($enabled_only = FALSE);

  /**
   * Builds block output for a given placement.
   *
   * @return array
   *   Render array markup.
   */
  public function buildOutput();

}
