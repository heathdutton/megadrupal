<?php

global $theme, $base_path, $base_url, $theme_path, $arctica_theme_path, $abs_arctica_theme_path, $files_path;
/* Store theme paths in php and javascript variables */
$arctica_theme_path = drupal_get_path('theme', 'arctica');
$abs_arctica_theme_path = $base_path . $arctica_theme_path;
$files_path = variable_get('file_public_path', conf_path() . '/files');

drupal_add_css($arctica_theme_path . '/styling/css/arctica.reset.css', array('weight' => 0));
drupal_add_css($arctica_theme_path . '/styling/css/arctica.base.css', array('weight' => CSS_THEME));

// include theme dependency reporter
require_once($arctica_theme_path . '/includes/theme-system-report.inc');
// include theme overrides
require_once($arctica_theme_path . '/includes/theme-overrides.inc');
// include theme functions
require_once($arctica_theme_path . '/includes/theme-functions.inc');
// include theme settings controller
require_once($arctica_theme_path . '/includes/theme-settings-controller.inc');


if (theme_get_setting('meta') == 'RESET') {
  $link = "<strong><a href=\"admin/appearance/settings/$theme\">Arctica Configurator</a></strong>";
  drupal_set_message(t('Please visit the !conf for !theme and save the form. Some settings need to be initialized.', array(
      '!conf' => $link,
      '!theme' => $theme,
    )), 'warning');
}

/**
 * Implements hook_preprocess().
 *
 * This function checks to see if a hook has a preprocess file associated with
 * it, and if so, loads it.
 *
 * @param $vars
 * @param $hook
 * @return Array
 */
  /* if you rename the theme you have to change the the name of this function and of the drupal_get_path parameter */
function arctica_preprocess(&$vars, $hook) {
  if (is_file(drupal_get_path('theme', 'arctica') . '/preprocess/preprocess-' . str_replace('_', '-', $hook) . '.inc')) {
    include('preprocess/preprocess-' . str_replace('_', '-', $hook) . '.inc');
  }
}

/**
 * Alters
 */


/**
 * Implements hook_js_alter().
 *
 * Force all JS files to aggregate into 1 file
 */
function arctica_js_alter(&$js){
	if ((variable_get('preprocess_js', '')) && TRUE) {
	  uasort($js, 'drupal_sort_css_js');
	  $i = 0;

	  foreach($js as $name => $script) {
	    $js[$name]['weight'] = $i++;
	    $js[$name]['group'] = JS_DEFAULT;
	    $js[$name]['every_page'] = TRUE;
	    $js[$name]['preprocess'] = TRUE;
	    $js[$name]['cache'] = TRUE;
	    $js[$name]['scope'] = 'header';
	    $js[$name]['version'] = '';
	  }
	}
}

/**
 * Implements hook_css_alter().
 *
 * Force all CSS files to aggregate into 1 file
 */
function arctica_css_alter(&$css){
	if ((variable_get('preprocess_css', '')) && TRUE) {
	  uasort($css, 'drupal_sort_css_js');
	  $i = 0;

	  foreach($css as $name => $script) {
	    $css[$name]['weight'] = $i++;
	    $css[$name]['group'] = CSS_DEFAULT;
	    $css[$name]['every_page'] = TRUE;
	    // $css[$name]['preprocess'] = TRUE;
	  }
	}
}

/**
 * Implements hook_admin_paths().
 */
function arctica_admin_paths() {
  $paths = array();
  if (module_exists('color')) {
    $paths['admin/appearance/settings/*'] = FALSE;
  }
  return $paths;
}

/**
 * Implements hook_custom_theme().
 */
function arctica_custom_theme() {
  if (module_exists('color')) {
    if (arg(0) == 'admin' && arg(1) == 'appearance' && arg(2) == 'settings' && arg(3)) {
      $current_theme = check_plain(arg(3));
      return $current_theme;
    }
  }
}