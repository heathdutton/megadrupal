<?php

/**
 * @file
 * Contains TMGMTBulkSelectorInterface class.
 */

/**
 * TMGMT Bulk Selector plugin interface.
 */
interface TMGMTBulkSelectorInterface extends TMGMTPluginBaseInterface {

  /**
   * Searches for items using given filters.
   *
   * @param string $item_type
   *   Source plugin item type.
   * @param string $translated_to
   *   The langcode. If not empty string: the result will include only items
   *   that already have translations to this language.
   * @param string $not_exists_in_jobs
   *   The langcode. If not empty string: items, that are exist in jobs with
   *   this target language, will be filtered out.
   * @param array $specific
   *   An array of values from the plugin-specific form elements.
   *
   * @return array
   *   An indexed array of item IDs.
   */
  public function searchItems($item_type, $translated_to, $not_exists_in_jobs, array $specific);

  /**
   * Returns an array of form elements that represents plugin-specific filters.
   *
   * @return array
   *   Form render array. The submitted values of that form elements will be
   *   passed then to the $specific parameter of the searchItems() method.
   */
  public function getFormSpecificFilters();
}
