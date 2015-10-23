<?php
/**
 * @file
 * System module integration.
 */

/**
 * Implements hook_setup_info().
 */
function system_setup_info() {
  $types = array();

  $types['markup'] = array(
    'form callback' => 'system_setup_markup_form_callback',
  );

  $types['theme'] = array(
    'form callback' => 'system_setup_theme_form_callback',
    'finish callback' => 'system_setup_theme_finish_callback',
  );

  $types['variable_set'] = array(
    'form callback' => 'system_setup_variable_set_form_callback',
    'finish callback' => 'system_setup_variable_set_finish_callback',
  );

  return $types;
}

/**
 * Implements hook_setup_theme_alter() on behalf of system.module.
 */
function system_setup_theme_alter(&$items) {
  $items['system_setup_theme'] = array(
    'variables' => array(
      'info' => array(),
      'description' => TRUE,
    ),
    'template' => 'system.setup_theme',
    'path' => drupal_get_path('module', 'setup') . '/templates',
  );
}

/**
 * Form callback for 'Markup' method.
 */
function system_setup_markup_form_callback($form, &$form_state) {
  if (isset($form_state['setup_step']['markup'])) {
    $form['markup'] = array(
      '#markup' => isset($form_state['setup_step']['filter'])
        ? check_markup($form_state['setup_step']['markup'], $form_state['setup_step']['filter'])
        : filter_xss($form_state['setup_step']['markup']),
    );
  }

  return $form;
}

/**
 * Form callback for 'Theme' method.
 */
function system_setup_theme_form_callback($form, &$form_state) {
  // Add jQuery Cycle integration if available.
  if (module_exists('libraries') && in_array('jquery.cycle', array_keys(libraries_get_libraries()))) {
    drupal_add_js(libraries_get_path('jquery.cycle') . '/jquery.cycle.all.js');
    drupal_add_js(drupal_get_path('module', 'setup') . '/scripts/system.theme.js');
  }

  // Get available themes.
  $available_themes = array();
  $themes = list_themes();
  foreach ($form_state['setup_step']['options'] as $option) {
    if (isset($themes[$option])) {
      $available_themes[$option] = theme('system_setup_theme', array(
        'info' => $themes[$option]->info,
        'description' => isset($form_state['setup_step']['description']) ? $form_state['setup_step']['description'] : TRUE,
      ));
    }
  }

  // Get a Default Value that is in the list of available options.
  $default_value = isset($form_state['object'][$form_state['step']])
    ? $form_state['object'][$form_state['step']]['theme']
    : variable_get('theme_default', key($available_themes));
  $default_value = in_array($default_value, array_keys($available_themes)) ? $default_value : key($available_themes);

  $form['theme'] = array(
    '#type' => 'radios',
    '#options' => $available_themes,
    '#default_value' => $default_value,
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Finish callback for 'Theme' method.
 */
function system_setup_theme_finish_callback(&$form_state) {
  if (isset($form_state['values']['theme'])) {
    $theme = $form_state['values']['theme'];

    // Get current list of themes.
    $themes = list_themes();

    // Check if the specified theme is one recognized by the system.
    if (!empty($themes[$theme])) {
      // Enable the theme if it is currently disabled.
      if (empty($themes[$theme]->status)) {
       theme_enable(array($theme));
      }
      // Set the default theme.
      variable_set('theme_default', $theme);

      // Flush all caches.
      drupal_flush_all_caches();
    }
  }
}

/**
 * Form callback for 'Variable set' method.
 */
function system_setup_variable_set_form_callback($form, &$form_state) {
  foreach ($form_state['setup_step']['variables'] as $key => $variable) {
    $form[$key] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get($key, NULL),
    );
    drupal_alter('setup_variable_set_form', $form[$key], $key);
    foreach ($variable as $attribute => $value) {
      $form[$key]["#{$attribute}"] = $value;
    }
  }

  return $form;
}

/**
 * Finish callback for 'Variable set' method.
 */
function system_setup_variable_set_finish_callback(&$form_state) {
  foreach (array_keys($form_state['setup_step']['variables']) as $key) {
    if (isset($form_state['values'][$key])) {
      variable_set($key, $form_state['values'][$key]);
    }
  }
}
