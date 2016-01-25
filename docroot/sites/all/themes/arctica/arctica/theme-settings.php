<?php

/**
 * Nitro module is a soft-requirement for the Configurator
 */

if (!module_exists('nitro')) {
  drupal_set_message(t('Please enable the !nitro module for an optimal Arctica Configurator experience.', array('!nitro' => l('Nitro', 'http://www.drupal.org/project/nitro'))), 'warning');
}

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
require_once dirname(__FILE__) . '/includes/theme-functions.inc';

/**
 * Fires arctica cache builder
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
arctica_css_cache_build(arg(count(arg()) - 1));

function arctica_form_system_theme_settings_alter(&$form, &$form_state) {
  /**
   * @ code
   * a bug in D7 causes the theme to load twice, if this file is loaded a
   * second time we return immediately to prevent further complications.
   */
  global $arctica_altered, $base_path, $theme_chain;
  if ($arctica_altered) return;
  $arctica_altered = TRUE;

  $subject_theme = arg(count(arg()) - 1);
  $arctica_theme_path = drupal_get_path('theme', 'arctica') . '/';
  $theme_path = drupal_get_path('theme', $subject_theme) . '/';
  $themes = list_themes();
  $theme_chain = array($subject_theme);
  foreach ($themes[$subject_theme]->base_themes as $base_theme => $base_theme_name) {
    $theme_chain[] = $base_theme;
  }

  drupal_add_css('themes/seven/vertical-tabs.css', array('group' => CSS_THEME, 'weight' => 9));
  drupal_add_css($arctica_theme_path . '/styling/css/arctica.configurator.css', array('group' => CSS_THEME, 'weight' => 10));
  drupal_add_library('system', 'ui.slider');
  drupal_add_library('system', 'ui.tabs');
  drupal_add_js($arctica_theme_path . "/scripts/admin/jquery.autotabs.js", 'file');
  drupal_add_js('$(function () {Drupal.behaviors.formUpdated = null;});', 'inline');


  $header  = '<div class="configurator-header">';
  $header .= '  <h3>Arctica Configurator</h3>';
  $header .= '  <h2>' . $subject_theme . ' ' . $themes[$subject_theme]->info['theme_version'] . '</h2>';
  $header .= '  <h3>Arctica ' . $themes['arctica']->info['theme_version'] . '</h3>';
  $header .= '  <a href="http://www.sooperthemes.com" title="Sponsored by SooperThemes premium Drupal themes. "><img class="configurator-logo" src="' . $base_path . drupal_get_path('theme', 'arctica') . '/styling/images/sooperthemes-logo.png" /></a>';
  $header .= '</div>';

  $form['arctica_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 0,
    '#prefix' => $header,
  );

  // Variable that contains easing options. Chose not to use function because $arctica_theme_path would be unavailable
  if (file_exists($arctica_theme_path . '/scripts/jquery.easing-sooper.js')) {
    $easing_options = array(
    'linear' => 'linear',
    'swing' => 'swing',
    'easeInTurbo' => 'easeInTurbo',
    'easeOutTurbo' => 'easeOutTurbo',
    'easeInTurbo2' => 'easeInTurbo2',
    'easeOutTurbo2' => 'easeOutTurbo2',
    'easeInTurbo3' => 'easeInTurbo3',
    'easeOutTurbo3' => 'easeOutTurbo3',
    'easeInSine' => 'easeInSine',
    'easeOutSine' => 'easeOutSine',
    'easeInExpo' => 'easeInExpo',
    'easeOutExpo' => 'easeOutExpo',
    'easeInCirc' => 'easeInCirc',
    'easeOutCirc' => 'easeOutCirc',
    'easeInElastic' => 'easeInElastic',
    'easeOutElastic' => 'easeOutElastic',
    'easeInOvershoot' => 'easeInOvershoot',
    'easeOutOvershoot' => 'easeOutOvershoot',
    'easeInOvershootTurbo' => 'easeInOvershootTurbo',
    'easeOutOvershootTurbo' => 'easeOutOvershootTurbo',
    'easeInBounce' => 'easeInBounce',
    'easeOutBounce' => 'easeOutBounce',
    );
  }
  else {
    $easing_options = array(
    'linear' => 'linear',
    'swing' => 'swing',
    );
  }
  // Create the form widgets using Forms API

  $form['arctica_settings']['polyfills'] = array(
    '#title' => t('Polyfills'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
  );

  $form['arctica_settings']['polyfills']['ie_polyfills'] = array(
    '#title' => t('Polyfills for internet explorer'),
    '#type' => 'fieldset',
  );

  $form['arctica_settings']['polyfills']['ie_polyfills']['boxsizing_polyfill'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE7 Box-sizing'),
    '#default_value' => theme_get_setting('boxsizing_polyfill'),
    '#description' => t('Polyfill that allows the usage of border-box module. This polyfill is required for the Arctica grid system to work in IE7.'),
  );

  $form['arctica_settings']['polyfills']['ie_polyfills']['flexible_images_polyfill'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE7 flexible images'),
    '#default_value' => theme_get_setting('flexible_images_polyfill'),
    '#description' => t('Flexible images do work in IE7 but the images look artifacted. This polyfill will enable anti-aliasing on images in IE7 and under to fix this.'),
  );

  $form['arctica_settings']['polyfills']['ie_polyfills']['html_polyfill'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE8 HTML5 shiv'),
    '#default_value' => theme_get_setting('html_polyfill'),
    '#description' => t('Without this ie 8 and below do not recognise HTML5 tags and cannot style them. You probably do not want to turn this off.'),
  );

  $form['arctica_settings']['polyfills']['ie_polyfills']['responsive_polyfill'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE8 Respond.js'),
    '#default_value' => theme_get_setting('responsive_polyfill'),
    '#description' => t('Respond.js is the most performant (fast) way to make ie8 and below understand simple mediaqueries. You probably do not want to turn this off, unless you want to use CSS3-MediaQueries-js.'),
  );

  $form['arctica_settings']['polyfills']['ie_polyfills']['responsive_polyfill2'] = array(
    '#type' => 'checkbox',
    '#title' => t('IE8 CSS3-MediaQueries-js'),
    '#default_value' => theme_get_setting('responsive_polyfill2'),
    '#description' => t('A much more elaborate but less performant polyfill for responsive design. Only enable this (and disable respond.js) when you must use advanced media queries. See !lewis for more details. ', array('!lewis' => l(t('this article'), 'http://coding.smashingmagazine.com/2011/08/10/techniques-for-gracefully-degrading-media-queries/'))),
  );

  $form['arctica_settings']['drupal'] = array(
    '#title' => t('Drupal core options / styles'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 8,
  );

  $form['arctica_settings']['drupal']['core_css'] = array(
    '#title' => t('Remove Drupal core css files'),
    '#type' => 'fieldset',
    '#description' => t('Select which stylsheets from Drupal core to disable. This helps you get rid of those list images, floats and ill placed paddings and margins. <strong>Checked stylesheets will be removed</strong>.'),
  );

  $form['arctica_settings']['drupal']['core_css']['css_overrides'] = array(
  '#type' => 'checkboxes',
  '#default_value' => theme_get_setting('css_overrides'),
  '#options' => array(
    'system_base_css' => t('<strong>system.base.css</strong> <em>Generic theme-independent base styles. These are kind of important.</em>'),
    'system_menus_css' => t('<strong>system.menus.css</strong> <em>Styles for menus and navigation markup.</em>'),
    'system_messages_css' => t('<strong>system.messages.css</strong> <em>Styles for system messages.</em>'),
    'system_theme_css' => t('<strong>theme.base.css</strong> <em>Basic styling for common markup.</em>'),
    'comment_css' => t('<strong>comment.css</strong> <em>Basic styling for comments.</em>'),
    'user_css' => t('<strong>user.css</strong> <em>Basic styling for user profiles as well as user forms (registration login etc.).</em>'),
    'search_css' => t('<strong>search.css</strong> <em>Basic styling for search results and forms</em>'),
    'taxonomy_css' => t('<strong>taxonomy.css</strong> <em>Basic styling for taxonomy terms</em>'),
    'poll_css' => t('<strong>poll.css</strong> <em>Basic styling for polls and minor styling in the node form</em>'),
    'book_css' => t('<strong>book.css</strong> <em>Basic styling for book pages/navigation and related forms</em>'),
    'node_css' => t('<strong>node.css</strong> <em>Minor styling for nodes (unpublished/preview/current revision)</em>'),
  ),
  );

  $form['arctica_settings']['drupal']['attribution_link'] = array(
    '#title' => t('Attribution link'),
    '#type' => 'fieldset',
  );

  $form['arctica_settings']['drupal']['attribution_link']['remove_attribution_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove attribution link'),
    '#default_value' => theme_get_setting('remove_attribution_link'),
    '#description' => t('Select to remove the link to the sponsor link to sooperthemes.com.'),
  );

  // Load Sooper Features
  foreach ($theme_chain as $theme) {
    foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/features', '/settings.inc/i') as $file) {
      include($file->uri);
    }
  }

  $form['arctica_settings']['drupal']['theme_settings'] = $form['theme_settings'];
  $form['arctica_settings']['drupal']['logo'] = $form['logo'];
  $form['arctica_settings']['drupal']['favicon'] = $form['favicon'];
  unset($form['theme_settings']);
  unset($form['logo']);
  unset($form['favicon']);
// Return the additional form widgets
return $form;
}


/**
 * Helper function to provide a list of sizes for use in theme settings.
 */
function _arctica_size_range($start = 11, $end = 16, $unit = FALSE, $default = NULL, $granularity = 1) {
  $range = '';
  if (is_numeric($start) && is_numeric($end)) {
    $range = array();
    $size = $start;
    while ($size >= $start && $size <= $end) {
      if ($size == $default) {
        $range[$size . $unit] = $size . $unit . ' (default)';
      }
      else {
        $range[$size . $unit] = $size . $unit;
      }
      $size += $granularity;
    }
  }
  return $range;
}

/**
 * Validation Function to enforce numeric values
 */
function _arctica_is_number($formelement, &$form_state) {
  $title = $formelement['#title'];
  if (!is_numeric($formelement['#value'])) {
    form_error($formelement, t("@title must be a number.", array('@title' => "<em>$title</em>")));
  }
}

/**
 * Validation Function to enforce valid CSS width
 */
function _arctica_is_width($formelement, &$form_state) {
  $thevalue = $formelement['#value'];
  $title = $formelement['#title'];
  if (!preg_match('/^0$|^auto$|^[0-9]+\\.?([0-9]+)?(px|em|ex|%|in|cm|mm|pt|pc)$/i', $formelement['#value'])) {
    form_error($formelement, t("@title must be a valid CSS width such as 900px, 100%, 60em or auto.", array('@title' => $title)));
  }
}

/**
 * Validation Function to enforce valid CSS fixed width
 */
function _arctica_is_fixed_width($formelement, &$form_state) {
  $thevalue = $formelement['#value'];
  $title = $formelement['#title'];
  if (!preg_match('/^none$|^0$|^[0-9]+\\.?([0-9]+)?(px|em|ex|in|cm|mm|pt|pc)$/i', $formelement['#value'])) {
    form_error($formelement, t("@title must be a valid CSS width such as 900px, 60em or 500pt.", array('@title' => $title)));
  }
}
