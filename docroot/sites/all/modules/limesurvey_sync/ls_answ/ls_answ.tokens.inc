<?php

/**
 * @file
 * Token callbacks for the ls_answ module.
 */

/**
 * Implements hook_token_info().
 *
 */
function ls_answ_token_info() {
  $ls_fields = array(
    'ls_status',
    'ls_sync',
    'ls_last_import',
    'ls_sid',
    'ls_lang',
    'ls_aid',
    'ls_token',
  );
  $node = array();
  module_load_include('inc', 'ls_answ', 'ls_answ');
  foreach ($ls_fields as $ls_field) {
    $ls_field_key = str_replace('_', '-', $ls_field);
    $name = ls_answ_display_sync_headers($ls_field);
    // Answers-related tokens for nodes.
    // First letter uppercase.
    $node[$ls_field_key] = array(
      'name' => drupal_strtoupper(drupal_substr($name, 0, 1)) . drupal_substr($name, 1),
      'description' => ls_answ_display_sync_headers_description($ls_field),
    );
  }
  $node['ls-url'] = array(
    'name' => t('Survey page'),
    'description' => t('The LimeSurvey answer URL, in order to fill it.'),
  );

  // Chained tokens for nodes.
  $node['survey'] = array(
    'name' => t('the associated survey'),
    'description' => t('The survey node associated to this answers content type.'),
    'type' => 'node',
  );

  return array(
    'tokens' => array('node' => $node),
  );
}

/**
 * Implements hook_tokens().
 *
 */
function ls_answ_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $replacements = array();

  if ($type == 'node' && !empty($data['node']) && ls_answ_type_is_answer($data['node']->type)) {
    $node = $data['node'];

    $ls_fields = array(
      'ls_status',
      'ls_sync',
      'ls_last_import',
      'ls_sid',
      'ls_lang',
      'ls_aid',
      'ls_token',
    );
    foreach ($ls_fields as $ls_field) {
      $ls_field_keys[str_replace('_', '-', $ls_field)] = $ls_field;
    }

    module_load_include('inc', 'ls_answ', 'ls_answ');
    // the ls_answ_display_sync_fields() function already sanitize datas.
    foreach ($tokens as $name => $original) {
      if (array_key_exists($name, $ls_field_keys)) {
        $ls_field = $ls_field_keys[$name];
        $replacements[$original] = ls_answ_display_sync_fields($ls_field, $node->$ls_field, $language_code);
      }
      switch ($name) {
        case 'ls-url':
          module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
          $replacements[$original] = limesurvey_sync_answ_surveyurl($node);
          break;
      }
    }

    if ($ls_ssid_tokens = token_find_with_prefix($tokens, 'survey')) {
      $node_survey = ls_answ_find_survey($node->type);
      $replacements += token_generate('node', $ls_ssid_tokens, array('node' => $node_survey), $options);
    }
  }
  return $replacements;
}
