<?php

/**
 * @file
 * Token callbacks for the ls_survey module.
 */

/**
 * Implements hook_token_info().
 *
 */
function ls_survey_token_info() {
  // This file is for the https://drupal.org/project/token module.
  // Rules module needs https://drupal.org/project/entity module.
  $ls_fields = array(
    'ls_sid' => 'ls-ssid',
    'ls_lang' => 'ls-slang',
    'ls_orig_type' => 'ls-type',
  );
  $descriptions = array(
    'ls_sid' => t('The survey ID from the LimeSurvey software for this answers content type.'),
    'ls_lang' => t('The language from the LimeSurvey software for this answers content type.'),
    'ls_orig_type' => t('The machine name type of this answers content type.'),
  );
  $node = array();
  module_load_include('inc', 'ls_survey', 'ls_survey');
  foreach ($ls_fields as $ls_field => $ls_field_key) {
    $name = ls_survey_display_sync_headers($ls_field);
    // Answers-related tokens for nodes.
    // First letter uppercase.
    $node[$ls_field_key] = array(
      'name' => drupal_strtoupper(drupal_substr($name, 0, 1)) . drupal_substr($name, 1),
      'description' => $descriptions[$ls_field],
    );
  }
  $node['ls-type-name'] = array(
    'name' => t('Answers content type.'),
    'description' => t('The human readable type of this answers content type.'),
  );

  return array(
    'tokens' => array('node' => $node),
  );
}

/**
 * Implements hook_tokens().
 *
 */
function ls_survey_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $replacements = array();

  if ($type == 'node' && !empty($data['node']) && $data['node']->type == 'survey') {
    $node = $data['node'];

    $ls_field_keys = array(
      'ls-ssid' => 'ls_sid',
      'ls-slang' => 'ls_lang',
      'ls-type' => 'ls_orig_type',
    );

    module_load_include('inc', 'ls_survey', 'ls_survey');
    // the ls_survey_display_sync_fields() function already sanitize datas.
    foreach ($tokens as $name => $original) {
      if (array_key_exists($name, $ls_field_keys)) {
        $ls_field = $ls_field_keys[$name];
        $replacements[$original] = ls_survey_display_sync_fields($ls_field, $node->$ls_field, $language_code);
      }
      switch ($name) {
        case 'ls-type-name':
          $answer_type = ls_survey_find_answ_type($node->ls_orig_type);
          $replacements[$original] = (!$answer_type) ? '' : $answer_type->name;
          break;
      }
    }
  }
  return $replacements;
}
