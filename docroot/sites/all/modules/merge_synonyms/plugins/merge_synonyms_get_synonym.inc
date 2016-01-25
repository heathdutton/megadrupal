<?php

/**
 * @file
 * Merge synonyms feeds tamper plugin.
 */

/**
 * Plugin initialization.
 */
$plugin = array(
  'form' => 'merge_synonyms_feeds_tamper_get_synonym_form',
  'callback' => 'merge_synonyms_feeds_tamper_get_synonym_callback',
  'name' => 'Get synonym',
  'multi' => 'loop',
  'category' => 'Text',
);

/**
 * Settings form.
 */
function merge_synonyms_feeds_tamper_get_synonym_form($importer, $element_key, $settings) {
  $form = array();
  $form['vname'] = array(
    '#type' => 'textfield',
    '#title' => t('Vocabulary machine name of the term:'),
    '#default_value' => isset($settings['vname']) ? $settings['vname'] : '',
  );
  return $form;
}

/**
 * Callback.
 */
function merge_synonyms_feeds_tamper_get_synonym_callback($result, $item_key, $element_key, &$field, $settings) {

  // Get vocabulary ID.
  $vocabulary = taxonomy_vocabulary_machine_name_load($settings['vname']);

  // Helper array in case imported data($field) is array.
  $field_array = array();

  // If vocabulary actually exists.
  if ($vocabulary) {
    // Check if $field is array.
    if (is_array($field)) {
      foreach ($field as $key => $value) {
        // Check if this term is in synonyms.
        $synonym_tid = merge_synonyms_get_synonym_tid($vocabulary->vid, $value);

        // If synonym found.
        if ($synonym_tid) {
          $term = taxonomy_term_load($synonym_tid);
          $field_array[$key] = $term->name;
        }
        else {
          $field_array[$key] = $value;
        }
      }

      // Get any synonyms.
      $field = $field_array;
    }
    else {
      // Check if this term is in synonyms.
      $synonym_tid = merge_synonyms_get_synonym_tid($vocabulary->vid, $field);

      if ($synonym_tid) {
        $term = taxonomy_term_load($synonym_tid);
        $field = $term->name;
      }
    }
  }
  else {
    watchdog('merge_synonyms', $settings['vname'] . ' vocabulary not found.', NULL, WATCHDOG_ERROR);
  }

  return $field;
}
