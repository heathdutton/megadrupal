<?php

/**
 * @file
 * Enables text and number field types to be source of synonyms.
 */

/**
 * Definition of TextSynonymsBehavior class.
 */
class TextSynonymsBehavior extends AbstractSynonymsSynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    $synonym = entity_label($synonym_entity_type, $synonym_entity);
    switch ($field['type']) {
      case 'text':
        break;

      // We add synonyms for numbers only if $synonym is a number.
      case 'number_integer':
      case 'number_float':
      case 'number_decimal':
        if (!is_numeric($synonym)) {
          return array();
        }
        break;

    }
    return array(array(
      'value' => $synonym,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $item['value'];
  }

  public function synonymsFind(QueryConditionInterface $condition, $field, $instance) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsSynonymsBehaviorException(t('Not supported storage engine %type in synonymsFind() method.', array(
        '%type' => $field['storage']['type'],
      )));
    }
    $table = array_keys($field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['value'];

    $this->synonymsFindProcessCondition($condition, $column);

    $query = db_select($table);
    $query->fields($table, array('entity_id'));
    $query->addField($table, $column, 'synonym');
    return $query->condition($condition)
      ->condition('entity_type', $instance['entity_type'])
      ->condition('bundle', $instance['bundle'])
      ->execute();
  }
}
