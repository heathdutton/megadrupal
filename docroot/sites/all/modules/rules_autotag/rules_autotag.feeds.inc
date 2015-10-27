<?php

/**
 * @file
 * Feeds integration to autotag taxonomy terms.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 */
function rules_autotag_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'taxonomy_term_reference') {
      $targets["rules_autotag_$name"] = array(
        'name' => t('Autotag @label', array('@label' => $instance['label'])),
        'callback' => 'rules_autotag_feeds_set_target',
        'description' => t('Autotag @label field of the entity.', array('@label' => $instance['label'])),
      );
    }
  }
}

/**
 * Callback for mapping. Here is where the actual mapping happens. Largely
 * copied from taxonomy_feeds_set_target().
 */
function rules_autotag_feeds_set_target($source, $entity, $target, $terms, $mapping = array()) {
  // Allow mapping the string '0' to a term name.
  if (empty($terms) && $terms != 0) {
    return;
  }

  // Handle non-multiple values.
  if (!is_array($terms)) {
    $terms = array($terms);
  }

  // Cut off "rules_autotag_" from the beginning.
  $target = substr($target, 14);

  $cache = &drupal_static(__FUNCTION__);
  if (!isset($cache['allowed_vocabularies'][$target])) {
    $info = field_info_field($target);
    foreach ($info['settings']['allowed_values'] as $tree) {
      // If the content taxonomy setting content_taxonomy_ignore_in_suggestions
      // is set, then the vocabulary is ignored.
      if (empty($tree['content_taxonomy_ignore_in_suggestions']) && $vocabulary = taxonomy_vocabulary_machine_name_load($tree['vocabulary'])) {
        $cache['allowed_vocabularies'][$target][$vocabulary->vid] = $vocabulary;
      }
    }
  }
  $extracted_tids = array();

  foreach ($cache['allowed_vocabularies'][$target] as $vid => $vocabulary) {
    foreach ($terms as $text) {
      $extracted_tids = array_merge(rules_autotag_extract($text, $vocabulary), $extracted_tids);
    }
  }
  // Allow for multiple mappings to the same target.
  $field = empty($entity->$target) ? array(LANGUAGE_NONE => array()) : $entity->$target;

  // Take any existing terms in the field into account.
  foreach ($field[LANGUAGE_NONE] as $item) {
    $extracted_tids[] = $item['tid'];
  }
  // Remove duplicated term IDs so that a term is only saved once in the field.
  $extracted_tids = array_unique($extracted_tids);
  $field[LANGUAGE_NONE] = array();
  foreach ($extracted_tids as $tid) {
    $field[LANGUAGE_NONE][]['tid'] = $tid;
  }

  $entity->$target = $field;
}
