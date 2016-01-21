<?php
/**
 * @file
 * Features integration for the Election Candidate module.
 */

/**
 * Implements hook_field_default_field_bases().
 *
 * This hook is provided by the Features module (not the Field module).
 */
function election_candidate_field_default_field_bases() {
  // Let the Features module know that the candidate_name and candidate_mail
  // field bases are provided by the election_candidate module, so they are not
  // exported into other features.
  $field_bases = array();
  module_load_include('inc', 'election_candidate', 'election_candidate.field');
  foreach (_election_candidate_fields() as $field_name => $field) {
    if (!isset($field_bases[$field_name])) {
      $field_bases[$field_name] = $field;
    }
  }
  return $field_bases;
}

/**
 * Implements hook_features_export().
 */
function election_candidate_type_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('election_candidate_type');

  foreach ($data as $type) {
    if ($info = election_candidate_get_types($type)) {
      // If this candidate type is provided by a different module, add that as
      // a dependency.
      if (isset($map[$type]) && $map[$type] != $module_name) {
        $export['dependencies'][$map[$type]] = $map[$type];
      }
      // Otherwise export the candidate type.
      elseif (in_array($info->base, array('election_candidate_type', 'features'))) {
        $export['features']['election_candidate_type'][$type] = $type;
        $export['dependencies']['election'] = 'election';
        $export['dependencies']['election_candidate'] = 'election_candidate';
        $export['dependencies']['features'] = 'features';
      }

      // Export the field instances on the candidate type.
      $fields = field_info_instances('election_candidate', $type);
      foreach ($fields as $name => $field) {
        $pipe['field_instance'][] = "election_candidate-{$field['bundle']}-{$field['field_name']}";
      }
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function election_candidate_type_features_export_render($module, $data, $export = NULL) {
  $elements = array(
    'type' => FALSE,
    'label' => TRUE,
    'weight' => FALSE,
    'data' => FALSE,
  );
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $type) {
    if ($info = election_candidate_get_types($type)) {
      $output[] = "    '{$type}' => array(";
      foreach ($elements as $key => $t) {
        if ($t) {
          $text = str_replace("'", "\'", $info->$key);
          $text = !empty($text) ? "t('{$text}')" : "''";
          $output[] = "      '{$key}' => {$text},";
        }
        else {
          $output[] = "      '{$key}' => '{$info->$key}',";
        }
      }
      $output[] = "    ),";
    }
  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('election_candidate_type_info' => $output);
}

/**
 * Implements hook_features_revert().
 */
function election_candidate_type_features_revert($module = NULL) {
  if ($default_types = features_get_default('election_candidate_type', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      // Delete candidate types
      db_delete('election_candidate_type')
        ->condition('type', $type_name)
        ->execute();
    }
    menu_rebuild();
  }
}
