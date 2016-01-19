<?php

/**
 * @file
 * Contains TMGMTBulkSelectorI18nString class.
 */

/**
 * Selector for the "i18n_string" source plugin.
 */
class TMGMTBulkSelectorI18nString extends TMGMTBulkSelectorBase implements TMGMTBulkSelectorInterface {

  /**
   * {@inheritdoc}
   */
  public function searchItems($item_type, $translated_to, $not_exists_in_jobs, array $specific) {

    // Base query.
    $query = db_select('i18n_string', 'i18n_s')->distinct();
    $item_id_expression = "concat(i18n_s.textgroup, ':', i18n_s.type, ':', i18n_s.objectid)";
    $query->addExpression($item_id_expression);

    // Filter by type as tmgmt_i18n_string_get_strings() function does.
    $info = i18n_object_info($item_type);
    $query->condition('i18n_s.textgroup', $info['string translation']['textgroup']);
    if (isset($info['string translation']['type'])) {
      $query->condition('i18n_s.type', $info['string translation']['type']);
    }
    elseif ($item_type == 'field' || $item_type == 'field_instance') {
      $bundles = array();
      foreach (entity_get_info() as $entity_info) {
        $bundles = array_merge($bundles, array_keys($entity_info['bundles']));
      }
      $query->condition('i18n_s.objectid', $bundles, $item_type == 'field_instance' ? 'IN' : 'NOT IN');
    }

    if ($translated_to) {
      $target_language_table_alias = $query->leftJoin(
        'locales_target',
        NULL,
        "%alias.lid = i18n_s.lid AND %alias.language = :language",
        array(':language' => $translated_to)
      );
      $query->isNotNull("$target_language_table_alias.lid");
    }

    $item_ids = $query->execute()->fetchCol();
    if ($not_exists_in_jobs) {
      $item_ids = $this->filterExistingInJobs('i18n_string', $item_type, $item_ids, $not_exists_in_jobs);
    }
    return $item_ids;
  }
}
