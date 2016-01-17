<?php

/**
 * @file
 * Enables Taxonomy Term Reference field type to be source of synonyms.
 */

/**
 * Definition of TaxonomySynonymsBehavior class.
 */
class TaxonomySynonymsBehavior extends AbstractSynonymsSynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SelectSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $terms = array();
    foreach ($items as $item) {
      $terms[] = $item['tid'];
    }
    $terms = taxonomy_term_load_multiple($terms);
    foreach ($terms as $term) {
      $synonyms[] = entity_label('taxonomy_term', $term);
    }
    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Taxonomy term reference supports only referencing of entity types
    // 'taxonomy_term'.. duh.
    if ($synonym_entity_type != 'taxonomy_term') {
      return array();
    }
    // Checking that $field is configured to reference the vocabulary of
    // $synonym_entity term.
    $is_allowed = FALSE;
    foreach ($field['settings']['allowed_values'] as $setting) {
      if ($synonym_entity->vocabulary_machine_name == $setting['vocabulary']) {
        if ($setting['parent'] == 0) {
          // No need to check parent property as there is no limitation on it.
          $is_allowed = TRUE;
          break;
        }
        else {
          foreach (taxonomy_get_parents_all($synonym_entity->tid) as $parent) {
            if ($parent->tid == $setting['parent']) {
              $is_allowed = TRUE;
              break(2);
            }
          }
        }
      }
    }
    if (!$is_allowed) {
      // Synonym term is from a vocabulary that is not expected by this field,
      // or under unexpected parent.
      return array();
    }
    return array(array(
      'tid' => $synonym_entity->tid,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $item['tid'];
  }

  public function synonymsFind(QueryConditionInterface $condition, $field, $instance) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsSynonymsBehaviorException(t('Not supported storage engine %type in synonymsFind() method.', array(
        '%type' => $field['storage']['type'],
      )));
    }
    $table = array_keys($field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['tid'];

    $query = db_select($table, 'field');
    $term_alias = $query->innerJoin('taxonomy_term_data', 'term', 'field.' . $column . ' = term.tid');
    $query->addField($term_alias, 'name', 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $instance['entity_type']);
    $query->condition('field.bundle', $instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $term_alias . '.name');
    $query->condition($condition);
    return $query->execute();
  }
}
