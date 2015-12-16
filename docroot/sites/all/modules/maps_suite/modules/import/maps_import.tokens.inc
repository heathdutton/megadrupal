<?php

/**
 * @file
 * Builds placeholder replacement tokens for maps_import data.
 */

/**
 * Implements hook_token_info().
 */
function maps_import_token_info() {
  $types = array(
    'maps_import_log' => array(
      'name' => t('MaPS Log'),
      'description' => t('Tokens related to MaPS Import log reports.'),
    ),
    'maps_import_profile' => array(
      'name' => t('MaPS Import profile'),
      'description' => t('Tokens related to MaPS Import profiles.'),
    ),
  );

  $maps_import_log['url'] = array(
    'name' => t('Log Url'),
    'description' => t('URL to a log report.'),
  );

  $maps_import_profile['pid'] = array(
    'name' => t('Pid'),
    'description' => t('The unique numeric ID of the profile.'),
  );

  $maps_import_profile['title'] = array(
    'name' => t('Title'),
    'description' => t('The title of the profile.'),
  );

  return array(
    'types' => $types,
    'tokens' => array('maps_import_log' => $maps_import_log, 'maps_import_profile' => $maps_import_profile),
  );
}

/**
 * Implements hook_tokens().
 */
function maps_import_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  switch ($type) {
    case 'maps_import_profile':
    case 'maps_import_log':
      $object = $data[$type];

      if (is_object($object)) {
        foreach ($tokens as $name => $original) {
          $getter = 'get' . ucfirst($name);
          if (is_callable(array($object, $getter))) {
            $replacements[$original] = $object->{$getter}();
          }
        }
      }

      break;
  }

  return $replacements;
}
