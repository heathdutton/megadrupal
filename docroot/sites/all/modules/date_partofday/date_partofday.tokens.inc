<?php

/**
 * Implements hook_token_info_alter().
 */
function date_partofday_token_info_alter(&$info) {
  $info['tokens']['date']['partofday'] = array(
    'name' => t('Part of day'),
    'description' => t('Part of day: %early_morning, %morning, %noon, %afternoon, %evening, %night or %midnight', array(
      '%early_morning' => t('morning'),
      '%morning' => t('morning'),
      '%noon' => t('noon'),
      '%afternoon' => t('afternoon'),
      '%evening' => t('evening'),
      '%night' => t('night'),
      '%midnight' => t('midnight'),
    )),
    'module' => 'date_partofday',
  );
}

/**
 * Implements hook_tokens().
 */
function date_partofday_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'date' && isset($tokens['partofday'])) {
    $date = !empty($data['date']) ? $data['date'] : REQUEST_TIME;

    if (isset($options['language'])) {
      $language_code = $options['language']->language;
    }
    else {
      $language_code = NULL;
    }

    $replacements[$tokens['partofday']] = date_partofday($date, NULL, $language_code);
  }

  return $replacements;
}
