<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Notice:
 * This theme will serve as base theme thus
 * all $theme_key if pointed to the base theme
 * need to be declared manually to 'sigmaone'
 *
 * if need to point to current enabled theme
 * can use $form_state['build_info']['files'][$file]
 * or global $theme_key
 */
function sigmaone_form_system_theme_settings_alter(&$form, &$form_state) {

  // Eliminate missing vtcore.inc.php
  // Now we use drupal form build info to force load the
  // core file even on ajax or form rebuild
  $theme_key = $form_state['build_info']['args'][0];

  // Fix for crash when sigmaone is not Administration theme
  if (!defined('BASE_THEME')) {
    define('BASE_THEME', 'sigmaone');
  }

  // Break if we don't have sigmaone as admin theme.
  $admin_theme = variable_get('admin_theme', '0');
  if ($admin_theme != BASE_THEME) {
    drupal_set_message(t('Please use @theme for administration theme, @theme configuration won\'t work with other administration theme.', array('@theme' => ucfirst(BASE_THEME))));
    return $form;
  }

  // Main form to act as vertical tabs root
  $form['theme_core'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Theme Configuration'),
  );

  $file = drupal_get_path('theme', BASE_THEME) . '/vtcore/core/vtcore.inc.php';
  if (!isset($form_state['build_info']['files'][$file])) {
    if (is_file($file)) {
      require_once $file;
      $form_state['build_info']['files'][$file] = 'vtcore_core_file';
    }
  }

  // Load vtcore admin file
  $file = drupal_get_path('theme', BASE_THEME) . '/vtcore/core/admin/vtcore_admin.inc.php';
  vtcore_form_load_includes($file, $form, $form_state);

  // Core Admin configuration form
  // @see vtcore_admin.inc.php
  vtcore_core_loader_configuration($form, $form_state, $theme_key);

  // Allow Plugin to alter the form
  // This is used by plugins to add their own configuration
  // form to the main theme form.
  vtcore_alter_process('vtcore_settings', $form, $form_state, $theme_key);

  // Main VTCore submit function
  if (!isset($form['#submit']) || !in_array('vtcore_main_submit', $form['#submit'])) {
    $form['#submit'][] = 'vtcore_main_submit';
  }

}