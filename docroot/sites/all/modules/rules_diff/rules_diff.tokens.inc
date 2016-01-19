<?php

/**
 * @file
 * Token integration with module Rules Diff
 */

/**
 * Implements hook_token_info().
 */
function rules_diff_token_info() {
  $types = array(
    'diff' => array(
      'name' => t('Differences'),
      'description' => t('Tokens related to differences.'),
      'needs-data' => 'diff',
    ),
    'diff_property' => array(
      'name' => t('Property differences'),
      'description' => t('Tokens related to property differences.'),
      'needs-data' => 'struct',
    ),
  );

  // Core tokens for nodes.
  $tokens['diff']['type'] = array(
    'name' => t("Type"),
    'description' => t("Whether the difference is and addition or a deletion."),
  );
  $tokens['diff']['difference'] = array(
    'name' => t("Difference"),
    'description' => t("The contents of the difference."),
  );
  $tokens['diff']['difference-plain'] = array(
    'name' => t("Difference"),
    'description' => t("The contents of the difference (plain text)."),
  );
  $tokens['diff']['subject_name'] = array(
    'name' => t("Subject name"),
    'description' => t("The name of the subject of the difference."),
  );

  // Chained tokens for nodes.
  $tokens['diff']['line'] = array(
    'name' => t("Line"),
    'description' => t("The line number where the difference is."),
  );
  $tokens['diff']['order'] = array(
    'name' => t("Order"),
    'description' => t("The order of the difference."),
  );

  $tokens['diff_property']['count'] = array(
    'name' => t("Count"),
    'description' => t("The number of differences between the two entities for a given property."),
  );
  $tokens['diff_property']['name'] = array(
    'name' => t("Name"),
    'description' => t("The name of the property."),
  );
  $tokens['diff_property']['differences'] = array(
    'name' => t("Differences"),
    'description' => t("The list of differences for a given property."),
  );
  $tokens['diff_property']['differences-plain'] = array(
    'name' => t("Differences"),
    'description' => t("The list of differences for a given property (plain text)."),
  );

  return array(
    'types' => $types,
    'tokens' => $tokens,
  );
}

/**
 * Implements hook_tokens().
 */
function rules_diff_tokens($type, $tokens, $data = array(), $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (!empty($data[$type]) && ($type == 'diff_property' || $type == 'diff')) {
    foreach ($tokens as $name => $original) {
      if ($type == 'diff_property' && strpos($name, 'differences') === 0) {
        $replacements[$original] = theme('rules_diff_property_differences', array(
          'differences' => $data[$type]['differences'],
          'name' => $data[$type]['name'],
          'sanitized' => strpos($name, '-plain') !== FALSE,
        ));
      }
      else if ($type == 'diff_property' && $name == 'count') {
        $replacements[$original] = count($data['diff_property']['differences']);
      }
      else if ($type == 'diff' && strpos($name, 'difference') === 0) {
        $replacements[$original] = theme('rules_diff_diff', array(
          'difference' => $data[$type]['difference'],
          'type' => $data[$type]['type'],
          'line' => $data[$type]['line'],
          'sanitized' => strpos($name, '-plain') !== FALSE,
        ));
      }
      else {
        $replacements[$original] = $data[$type][$name];
      }
    }
  }

  return $replacements;
}
