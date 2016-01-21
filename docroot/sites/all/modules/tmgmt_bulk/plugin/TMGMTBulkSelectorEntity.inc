<?php

/**
 * @file
 * Contains TMGMTBulkSelectorEntity class.
 */

/**
 * Selector for the "entity" source plugin.
 */
class TMGMTBulkSelectorEntity extends TMGMTBulkSelectorBase implements TMGMTBulkSelectorInterface {

  /**
   * {@inheritdoc}
   */
  public function searchItems($item_type, $translated_to, $not_exists_in_jobs, array $specific) {

    // Base query.
    $entity_info = entity_get_info($item_type);
    $id_key = $entity_info['entity keys']['id'];
    $query = db_select($entity_info['base table'], 'e');
    $query->addField('e', $id_key);

    // Exclude entities with "und" source languages.
    $source_language_table_alias = $query->leftJoin(
      'entity_translation',
      NULL,
      "%alias.entity_type = :entity_type AND %alias.entity_id = e.$id_key AND %alias.source = ''",
      array(':entity_type' => $item_type)
    );
    $query->condition("$source_language_table_alias.language", LANGUAGE_NONE, '<>');

    if ($translated_to) {
      $target_language_table_alias = $query->leftJoin(
        'entity_translation',
        NULL,
        "%alias.entity_type = :entity_type AND %alias.entity_id = e.$id_key AND %alias.language = :language",
        array(':entity_type' => $item_type, ':language' => $translated_to)
      );
      $query->isNotNull("$target_language_table_alias.language");
      // Exclude entities with having source language same as the target
      // language we search for.
      $query->condition("$source_language_table_alias.language", $translated_to, '<>');
    }

    $item_ids = $query->execute()->fetchCol();
    if ($not_exists_in_jobs) {
      $item_ids = $this->filterExistingInJobs('entity', $item_type, $item_ids, $not_exists_in_jobs);
    }
    return $item_ids;
  }
}
