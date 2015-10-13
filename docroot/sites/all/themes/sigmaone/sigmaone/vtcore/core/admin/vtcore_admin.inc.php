<?php
/**
 * @file
 * This file is the main file that will act as a bridge
 * and wrapper for all vtcore plugin admin form.
 *
 * The base theme will autolaod this file from its
 * theme_settings.php and then this file will search
 * and find all available plugin admin form then merge
 * them together.
 *
 * Final saving to database as theme settings variable
 * will be performed by this file, ensuring that other
 * plugin has the chances to modify $form_state['values']
 * before it is passed to the final submit function.
 *
 */

/**
 * The core configuration
 * This function is called from theme-settings.php
 * it will create the $form['theme_core'] fieldset
 * so other plugin can use it for configuration dash
 * board, without this other plugin may failed to load
 */
function vtcore_core_loader_configuration(&$form, &$form_state, $theme_key) {
  global $vtcore;

  // Rely on $form_state for theme name
  if (empty($theme_key)) {
    $theme_key = $form_state['build_info']['args'][0];
  }

  // Rebuild cached theme data
  $rebuild_theme = variable_get('theme_' . $theme_key . '_rebuild_state');
  if ($rebuild_theme == TRUE) {
    system_rebuild_theme_data();
    drupal_theme_rebuild();
    cache_clear_all();
    variable_set('theme_' . $theme_key . '_rebuild_state', FALSE);
  }

  // Cache our theme directory,So other
  // plugin doesn't have to call drupal_get_path
  // every time.
  $theme_path = drupal_get_path('theme', $theme_key);
  $base_theme_path = drupal_get_path('theme', BASE_THEME);

  $form_state['build_info']['theme']['theme_path'] = $theme_path;
  $form_state['build_info']['theme']['base_theme_path'] = $base_theme_path;
  $form_state['build_info']['theme']['theme_key'] = $theme_key;
  $form_state['build_info']['theme']['base_theme_key'] = BASE_THEME;

  $file = $base_theme_path . '/vtcore/core/admin/vtcore_css_builder.inc.php';
  vtcore_form_load_includes($file, $form, $form_state);

  // Intialize all VTCore plugin configuration form
  $plugins = vtcore_load_include($vtcore->plugin_path, 'admin');

  // Grab subthemes plugins and merge them with base theme plugins
  if (isset($vtcore->subtheme_plugin_path) && is_dir($vtcore->subtheme_plugin_path)) {
    $subtheme_plugins = vtcore_load_include($vtcore->subtheme_plugin_path, 'admin');
    $plugins = vtcore_array_merge_recursive_distinct($plugins, $subtheme_plugins);
  }

  foreach ($plugins as $plugin_path => $plugin_value) {
    if (strstr($plugin_path, '.admin')) {
      vtcore_form_load_includes($plugin_path, $form, $form_state);
    }
  }

  // add custom class to honor core.css modification
  $form['#attributes'] = array(
    'class' => array(
      'vtcore',
    ),
  );

  // store old value first
  $form_state['storage']['old_value'] = variable_get('theme_' . $theme_key . '_settings');

  // set the page title
  drupal_set_title(t('!theme Theme Configuration', array('!theme' => $theme_key)));

  // move drupal standard form into fieldset
  $form['theme_settings']['#title'] = t('Default');
  $form['theme_settings']['#collapsible'] = TRUE;
  $form['theme_settings']['#collapsed'] = TRUE;
  $form['theme_settings']['#group'] = 'theme_core';

  $form['logo']['#collapsible'] = TRUE;
  $form['logo']['#collapsed'] = TRUE;
  $form['logo']['#title'] = t('Logo');
  $form['logo']['#group'] = 'theme_core';

  $form['favicon']['#collapsible'] = TRUE;
  $form['favicon']['#collapsed'] = TRUE;
  $form['favicon']['#title'] = t('Favicon');
  $form['favicon']['#group'] = 'theme_core';


  // Reset button
  $form['actions']['reset'] = array(
    '#type' => 'button',
    '#executes_submit_callback' => TRUE,
    '#name' => 'reset',
    '#button_type' => 'submit',
    '#value' => t('Reset to default'),
    '#weight' => 19,
    '#submit' => array(999 => 'vtcore_reset_default'),
  );

}

/**
 * Custom submit function to store all enabled
 * theme plugin into a serialized data
 *
 * @todo : Clean out unwanted data before saving
 */
function vtcore_main_submit(&$form, &$form_state) {

  global $vtcore;

  // Preserve the setting eventhough plugin has been disabled
  if (is_array($form_state['storage']['old_value'])) {
    $form_state['values'] = vtcore_array_merge_recursive_distinct($form_state['storage']['old_value'], $form_state['values']);
  }

  // Build css if configured
  if (!empty($form_state['values']['collective_css'])) {
    // load the css builder plugin
    require_once($vtcore->core_path . '/admin/vtcore_css_builder.inc.php');

    // build the dynamic css
    vtcore_css_create($form_state);
  }

  // Wipe dynamic.css on empty collective_css
  $file = $vtcore->theme_path .'/css/dynamic.css';
  if (empty($form_state['values']['collective_css']) && is_file($file)) {
    file_unmanaged_delete($file);
  }
}

/**
 * submit function for reset button
 */
function vtcore_reset_default(&$form, &$form_state) {
  global $vtcore;

  // let other plugin hook and specify their reset function
  vtcore_alter_process('vtcore_reset_default', $form, $form_state);

  // remove saved configuration
  vtcore_reset_theme();

  // rebuild theme data
  system_rebuild_theme_data();
  drupal_theme_rebuild();

  // Remove dynamic css
  $file = $vtcore->theme_path . '/css/dynamic.css';
  file_unmanaged_delete($file);

  drupal_set_message(t('Reverted to default values'));
}