<?php
/**
 * @file
 * Contains Token API hook implementations.
 */

/**
 * Implements hook_token_info().
 */
function theme_filter_token_info() {
  // Define theme-filter type.
  $info['types']['theme-filter'] = array(
    'name' => t('Theme Filter'),
    'description' => t('Tokens to generate Theme path'),
    'needs-data' => 'theme-filter',
  );

  $info['tokens']['theme-filter']['theme'] = array(
    'name' => t('Theme Path'),
    'description' => t('Use it like [theme-filter:path-to-theme] to replace it as your theme path'),
    'dynamic' => TRUE,
    'type' => 'theme',
  );

  $info['tokens']['theme-filter']['path-to-theme'] = array(
    'name' => t('Current Theme Path'),
    'description' => t('Use it like [theme-filter:path-to-theme] to replace it as your theme path'),
    'type' => 'path-to-theme',
  );

  // Type path-to-theme.
  $info['types']['path-to-theme'] = array(
    'name' => t('Current Theme Path'),
    'description' => t('Tokens related to current theme.'),
    'needs-data' => 'theme-filter',
  );

  $info['tokens']['path-to-theme']['absolute'] = array(
    'name' => t('Current Theme Absolute path'),
    'description' => t('Use it like [theme-filter:path-to-theme:absolute] to replace it as your theme path'),
  );

  // Type theme.
  $info['types']['theme'] = array(
    'name' => t('Theme Path'),
    'description' => t('Tokens related to theme.'),
    'needs-data' => 'theme-filter',
  );

  $info['tokens']['theme']['absolute'] = array(
    'name' => t('Theme Path Absolute'),
    'description' => t('Use it like [theme-filter:theme:absolute] to replace it as your theme path'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function theme_filter_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'theme-filter') {
    foreach ($tokens as $name => $original) {
      $explode = explode(':', $name);
      switch ($explode[0]) {
        case 'theme':
          if (!empty($explode[1])) {
            if (isset($explode[2]) && $explode[2] == 'absolute') {
              $replacements[$original] = file_create_url(drupal_get_path('theme', $explode[1]));
            }
            else {
              $replacements[$original] = $GLOBALS['base_path'] . drupal_get_path('theme', $explode[1]);
            }
          }
          break;

        case 'path-to-theme':
          if (isset($explode[1]) && $explode[1] == 'absolute') {
            $replacements[$original] = file_create_url(path_to_theme());
          }
          else {
            $replacements[$original] = $GLOBALS['base_path'] . path_to_theme();
          }
          break;
      }
    }
  }

  return $replacements;
}
