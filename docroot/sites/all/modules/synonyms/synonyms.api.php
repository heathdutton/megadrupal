<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Hook to collect info about available synonyms behavior implementations.
 *
 * Hook to collect info about what PHP classes implement provided synonyms
 * behavior for different field types.
 *
 * @param string $behavior
 *   Name of a synonyms behavior. This string will always be among the keys
 *   of the return of synonyms_behaviors(), i.e. name of a ctools plugin
 *
 * @return array
 *   Array of information about what synonyms behavior implementations your
 *   module supplies. The return array must contain field types as keys, whereas
 *   corresponding values should be names of PHP classes that implement the
 *   provided behavior for that field type. Read more about how to implement a
 *   specific behavior in the advanced help of this module. In a few words: you
 *   will have to implement an interface that is defined in the behavior
 *   definition. Do not forget to make sure your PHP class is visible to Drupal
 *   auto discovery mechanism
 */
function hook_synonyms_behavior_implementation_info($behavior) {
  switch ($behavior) {
    case 'autocomplete':
      return array(
        'my-field-type' => 'MyFieldTypeAutocompleteSynonymsBehavior',
      );
      break;

    case 'another-behavior':
      return array(
        'my-field-type-or-yet-another-field-type' => 'MyFieldTypeAnotherBehaviorSynonymsBehavior',
      );
      break;
  }

  return array();
}

/**
 * Hook to alter info about available synonyms behavior implementations.
 *
 * This hook is invoked right after hook_synonyms_behavior_implementation_info()
 * and is designed to let modules overwrite implementation info from some other
 * modules. For example, if module A provides implementation for some field
 * type, but your module has a better version of that implementation, you would
 * need to implement this hook and to overwrite the implementation info.
 *
 * @param array $info
 *   Array of information about existing synonyms behavior implementations that
 *   was collected from modules
 * @param string $behavior
 *   Name of the behavior for which the info about implementation is being
 *   generated
 */
function hook_synonyms_behavior_implementation_info_alter(&$info, $behavior) {
  switch ($behavior) {
    case 'the-behavior-i-want':
      $info['the-field-type-i-want'] = 'MyFieldTypeAutocompleteSynonymsBehavior';
      break;
  }
}

/**
 * Example of how to implement a synonyms behavior for an arbitrary field type.
 */
class MyFieldTypeAutocompleteSynonymsBehavior extends AbstractSynonymsSynonymsBehavior implements AutocompleteSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    // Let's say our synonyms is stored in the 'foo' column of the field.
    $synonyms = array();
    foreach ($items as $item) {
      $synonyms[] = $item['foo'];
    }
    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Let's say we keep the synonyms as strings and under the 'foo' column, to
    // keep it consistent with the extractSynonyms() method.
    $label = entity_label($synonym_entity_type, $synonym_entity);
    return array(array(
      'foo' => $label,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    // Since we've agreed that the column that stores data in our imaginary
    // field type is "foo". Then it suffices just to implement the hash function
    // as the value of foo column.
    return $item['foo'];
  }

  public function synonymsFind(QueryConditionInterface $condition, $field, $instance) {
    // We only can find synonyms in SQL storage. If this field is not one, then
    // we have full right to throw an exception.
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsSynonymsBehaviorException(t('Not supported storage engine %type in synonymsFind() method.', array(
        '%type' => $field['storage']['type'],
      )));
    }
    // Now we will figure out in what table $field is stored. We want to
    // condition the 'foo' column within that field table.
    $table = array_keys($field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['foo'];

    // Once we know full path to the column that stores synonyms as plain text,
    // we can use a supplementary method from AbstractSynonymsSynonymsBehavior,
    // which helps us to convert a column placeholder into its real value within
    // the condition we have received from outside.
    $this->synonymsFindProcessCondition($condition, $column);

    // Now we are all set to build a SELECT query and return its result set.
    $query = db_select($table);
    $query->fields($table, array('entity_id'));
    $query->addField($table, $column, 'synonym');
    return $query->condition($condition)
      ->condition('entity_type', $instance['entity_type'])
      ->condition('bundle', $instance['bundle'])
      ->execute();
  }
}
