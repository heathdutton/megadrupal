<?php

/**
 * @file
 * comstack.theme.inc
 */

/**
 * Implements hook_theme().
 */
function comstack_theme() {
  $theme = array();

  $theme['comstack_angular'] = array(
    'template' => 'comstack-angular',
    'path' => drupal_get_path('module', 'comstack') . '/templates',
    'variables' => array(
      'app' => NULL,
      'url' => NULL,
      'controller' => NULL,
      'ui_view' => NULL,
      'apply_scroll' => FALSE,
    ),
  );

  return $theme;
}

/**
 * Process function for theme_comstack_angular().
 */
function template_preprocess_comstack_angular(&$variables) {
  // Lots of attributes to add!
  if (isset($variables['app'])) {
    $app = check_plain($variables['app']);
    $variables['attributes_array']['ng-app'] = $app;
    $variables['classes_array'][] = "cs-angular-$app";
  }
  if (isset($variables['controller'])) {
    $variables['attributes_array']['ng-controller'] = check_plain($variables['controller']);
  }
  $variables['ui_view_attributes'] = array();
  if ($variables['apply_scroll']) {
    $variables['ui_view_attributes']['autoscroll'] = 'true';
  }
  if ($variables['apply_idle']) {
    $variables['ui_view_attributes']['idle'] = NULL;
  }
}
