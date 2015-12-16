<?php

/**
 * @file
 * Filter helper file.
 */

/**
 * Implements callback_filter_settings().
 * Taxonomy autolink filter settings callback.
 * @see filter_filter_info()
 */
function _taxonomy_autolink_settings($form, &$form_state, $filter, $format, $defaults) {
  $filter->settings += $defaults;
  $settings['taxonomy_autolink_vocabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Taxonomy vocabularies'),
    '#description' => t('Apply Taxonomy autolink to above selected taxonomy vocabularies.'),
    '#options' => taxonomy_autolink_vocabs(),
    '#default_value' => $filter->settings['taxonomy_autolink_vocabs'],
  );
  $settings['taxonomy_autolink_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Advanced mode with Plural/Singular support'),
    '#description' => t('Whether or not the match is case sensitive.'),
    '#default_value' => $filter->settings['taxonomy_autolink_mode'],
  );
  $settings['taxonomy_autolink_case_sensitivity'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case sensitive'),
    '#description' => t('Whether or not the match is case sensitive.'),
    '#default_value' => $filter->settings['taxonomy_autolink_case_sensitivity'],
  );
  $settings['taxonomy_autolink_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum links per term'),
    '#description' => t('Default is only first occurrence.'),
    '#size' => 5,
    '#maxlength' => 4,
    '#default_value' => $filter->settings['taxonomy_autolink_limit'],
    '#element_validate' => array('element_validate_integer_positive'),
  );
  return $settings;
}

/**
 * Implements callback_filter_process().
 * Taxonomy autolink filter process callback.
 */
function _taxonomy_autolink_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  // Get vocabularies
  $vocabs = array_filter($filter->settings['taxonomy_autolink_vocabs']);
  if (!$vocabs) {
    return $text;
  }
  // Get terms.
  $result = db_query("SELECT tid, name, LOWER(name) AS name_lower FROM {taxonomy_term_data} WHERE vid IN (:vids)", array(':vids' => $vocabs));
  $terms = $result->fetchAllAssoc('name');
  // Process text.
  if (count($terms) > 0) {
    $configs = array(
      'mode' => isset($filter->settings['taxonomy_autolink_mode']) ? $filter->settings['taxonomy_autolink_mode'] : 0,
      'limit' => isset($filter->settings['taxonomy_autolink_limit']) ? trim($filter->settings['taxonomy_autolink_limit']) : 1,
      'case' => isset($filter->settings['taxonomy_autolink_case_sensitivity']) ? $filter->settings['taxonomy_autolink_case_sensitivity'] : 0,
    );
    // Load text parser.
    module_load_include('inc', 'taxonomy_autolink', 'includes/taxonomy_autolink.func');
    return _taxonomy_autolink_links($text, $terms, $configs);
  }
  else {
    // Return default text.
    return $text;
  }
}
