<?php

/**
 * @file
 * Enables Entity Reference field type to be source of synonyms.
 */

/**
 * Definition of EntityReferenceSynonymsBehavior class.
 */
class EntityReferenceSynonymsBehavior extends AbstractSynonymsSynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $target_tids = array();
    foreach ($items as $item) {
      $target_tids[] = $item['target_id'];
    }
    $entities = entity_load($field['settings']['target_type'], $target_tids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label($field['settings']['target_type'], $entity);
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Firstly validating that this entity reference is able to reference to
    // that type of entity.
    $expected_synonym_entity_type = $field['settings']['target_type'];
    if ($expected_synonym_entity_type != $synonym_entity_type) {
      return array();
    }
    $synonym_entity_id = entity_id($synonym_entity_type, $synonym_entity);
    return array(array(
      'target_id' => $synonym_entity_id,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $field['settings']['target_type'] . $item['target_id'];
  }

  public function synonymsFind(QueryConditionInterface $condition, $field, $instance) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsSynonymsBehaviorException(t('Not supported storage engine %type in synonymsFind() method.', array(
        '%type' => $field['storage']['type'],
      )));
    }
    $table = array_keys($field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['target_id'];

    $query = db_select($table, 'field');

    $target_entity_type_info = entity_get_info($field['settings']['target_type']);
    if (!isset($target_entity_type_info['base table']) || !$target_entity_type_info['base table']) {
      throw new SynonymsSynonymsBehaviorException(t('Target entity type %entity_type is not stored in database.', array(
        '%entity_type' => $field['settings']['target_type'],
      )));
    }
    if (!isset($target_entity_type_info['entity keys']['id'])) {
      throw new SynonymsSynonymsBehaviorException(t('Target entity type %entity_type does not declare primary key.', array(
        '%entity_type' => $field['settings']['target_type'],
      )));
    }
    if (!isset($target_entity_type_info['entity keys']['label'])) {
      throw new SynonymsSynonymsBehaviorException(t('Target entity type %entity_type does not declare label column.', array(
        '%entity_type' => $field['settings']['target_type'],
      )));
    }

    $target_entity_alias = $query->innerJoin($target_entity_type_info['base table'], 'target_entity', 'field.' . $column . ' = target_entity.' . $target_entity_type_info['entity keys']['id']);
    $query->addField($target_entity_alias, $target_entity_type_info['entity keys']['label'], 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $instance['entity_type']);
    $query->condition('field.bundle', $instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $target_entity_alias . '.' . $target_entity_type_info['entity keys']['label']);
    $query->condition($condition);
    return $query->execute();
  }
}
