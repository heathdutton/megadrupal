<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */

function glazed_free_form_system_theme_settings_alter(&$form, &$form_state) {
  /**
   * @ code
   * a bug in D7 causes the theme to load twice, if this file is loaded a
   * second time we return immediately to prevent further complications.
   */
  global $glazed_altered, $base_path, $theme_chain;
  if ($glazed_altered) return;
  $glazed_altered = TRUE;

  // Wrap global and jQuery setting fieldsets in vertical tabs.
  $form['global'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Override Global Settings') . '</small></h2>',
    '#weight' => -9,
  );
  $form['theme_settings']['#group'] = 'global';
  $form['logo']['#group'] = 'global';
  $form['favicon']['#group'] = 'global';
  $form['favicon']['#group'] = 'global';

  // $subject_theme = arg(count(arg()) - 1); old way
  $subject_theme = $form_state['build_info']['args'][0];
  $glazed_theme_path = drupal_get_path('theme', 'glazed_free') . '/';
  $theme_path = drupal_get_path('theme', $subject_theme) . '/';
  $themes = list_themes();
  $theme_chain = array($subject_theme);
  foreach ($themes[$subject_theme]->base_themes as $base_theme => $base_theme_name) {
    $theme_chain[] = $base_theme;
  }

  /**
   * Glazed cache builder
   * Cannot run as submit function because  it will set outdated values by
   * using theme_get_setting to retrieve settings from database before the db is
   * updated. Cannot put cache builder in form scope and use $form_state because
   * it also needs to initialize default settings by reading the .info file.
   * By calling the cache builder here it will run twice: once before the
   * settings are saved and once after the redirect with the updated settings.
   * @todo come up with a less 'icky' solution
   */

  if (!isset($files_path)) { // in case admin theme is used
    global $files_path;
    $files_path = variable_get('file_public_path', conf_path() . '/files');
  }
  require_once(drupal_get_path('theme', 'glazed_free') . '/glazed_callbacks.inc');
  glazed_css_cache_build(arg(count(arg()) - 1));

  drupal_add_css('themes/seven/vertical-tabs.css');
  drupal_add_library('system', 'ui.slider');
  drupal_add_library('system', 'ui.tabs');
  drupal_add_css($glazed_theme_path . 'js/vendor/bootstrap-switch/bootstrap-switch.min.css');
  drupal_add_css($glazed_theme_path . 'js/vendor/bootstrap-slider/bootstrap-slider.min.css');
  drupal_add_css($glazed_theme_path . 'css/glazed.admin.themesettings.css');
  drupal_add_js($glazed_theme_path . 'js/vendor/bootstrap-switch/bootstrap-switch.min.js', 'file');
  drupal_add_js($glazed_theme_path . 'js/vendor/bootstrap-slider/bootstrap-slider.min.js', 'file');
  drupal_add_js($glazed_theme_path . 'js/glazed-settings.admin.js', 'file');
  drupal_add_js('jQuery(function () {Drupal.behaviors.formUpdated = null;});', 'inline');
  // Decoy function to fix erros resulting from missing preview.js
  drupal_add_js('Drupal.color = { callback: function() {} }', 'inline');

  if (!isset($themes['bootstrap']->info['version'])) {
    $themes['bootstrap']->info['version'] = 'dev';
  }

  $header  = '<div class="settings-header">';
  $header .= '  <h2>' . $subject_theme . ' ' . $themes[$subject_theme]->info['version'] . '</h2>';
  $header .= '  <h3>Bootstrap ' . $themes['bootstrap']->info['version'] . '</h3>';
  $header .= '  <a href="http://www.sooperthemes.com" title="Drupal Theme by SooperThemes premium Drupal themes. "><img class="settings-logo glazed-util-background-primary" src="' . $base_path . drupal_get_path('theme', 'glazed_free') . '/glazed-favicon.png" /></a>';
  $header .= '</div>';

  $form['glazed_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -20,
    '#prefix' => $header,
  );
  // Load Sooper Features
  foreach ($theme_chain as $theme) {
    foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/features', '/settings.inc/i') as $file) {
      include($file->uri);
    }
  }

  // Adding submit handler requires some extra code due to buggy theme settings system
  // http://ghosty.co.uk/2014/03/managed-file-upload-in-drupal-theme-settings/
  $form['#submit'][] = 'glazed_settings_form_submit';
  // Get all themes.
  $themes = list_themes();
  // Get the current theme
  $active_theme = $GLOBALS['theme_key'];
  $form_state['build_info']['files'][] = str_replace("/$active_theme.info", '', $themes[$active_theme]->filename) . '/theme-settings.php';

    // $form['glazed_settings']['drupal']['theme_settings'] = $form['theme_settings'];
    // $form['glazed_settings']['drupal']['logo'] = $form['logo'];
    // $form['glazed_settings']['drupal']['favicon'] = $form['favicon'];
    // unset($form['theme_settings']);
    // unset($form['logo']);
    // unset($form['favicon']);
  // Return the additional form widgets
  return $form;
}

function glazed_settings_form_submit(&$form, $form_state) {
  // Save Page Title Image
  // @See glazed_config.module
  if (isset($form_state['values']['page_title_image'])) {
    $page_title_image = $form_state['values']['page_title_image'];
    if ($page_title_image && is_numeric($page_title_image) && ($page_title_image > 0)) {
      $image_fid = $page_title_image;
      $image = file_load($image_fid);
      if (is_object($image)) {
        // Check to make sure that the file is set to be permanent.
        if ($image->status == 0) {
          // Update the status.
          $image->status = FILE_STATUS_PERMANENT;
          // Save the update.
          file_save($image);
          // Add a reference to prevent warnings.
          file_usage_add($image, 'glazed', 'theme', 1);
         }
      }
    }
  }

  // If requested import additional demo content
  module_load_include('module', 'uuid');
  module_load_include('inc', 'features', 'features.admin');
  $demo_content_modules = array_filter(_features_get_features_list(), "_glazed_is_demo_content");

  usort($demo_content_modules, function($a, $b) {
    return (count($a->info['features']['uuid_node']) < count($b->info['features']['uuid_node'])) ? 1 : -1;
  });
  foreach ($demo_content_modules as $module) {
    if (isset($module->info['features']) && isset($module->info['features']['uuid_node'])) {
      $node_sample = $module->info['features']['uuid_node'][0];
      if ($form_state['values'][$module->name] && !entity_get_id_by_uuid('node', array($node_sample))) {
        drupal_set_message($module->name . ' is missing and selected for installation');
        module_enable(array($module->name));
        module_disable(array($module->name), FALSE);
      }
    }
  }
}

function _glazed_is_demo_content ($feature) {
  return ((strpos($feature->name, '_content') OR (strpos($feature->name, '_theme_settings')))
    && isset($feature->info['features']['uuid_node']));
}

/**
 * Implements hook_form_FORM_ID_alter().
 * We hijack the function that is reserved for the user module in order
 * to get the full monty of $form stuff. The module cache is cleared to make sure
 * our hook implementation is known before this point. Yes this is a dirty hack.
 */
if (module_exists('color')) {
  registry_rebuild();
  function user_form_system_theme_settings_alter(&$form, &$form_state) {
    if (isset($form['color'])) {
      $form['glazed_settings']['color'] = $form['color'];
      unset($form['color']);
      $form['glazed_settings']['color']['#title'] = 'Colors';
      $form['glazed_settings']['color']['#weight'] = 1;
    }
  }
}
