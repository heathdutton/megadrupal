<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function kands_preprocess_html(&$variables, $hook) {
  $variables['webfont_css'] = theme_get_setting('kands_webfont_css');
  $variables['webfont_js'] = theme_get_setting('kands_webfont_js');
  drupal_add_css($variables['webfont_css'], 'external');
  drupal_add_js($variables['webfont_js'], 'external');
}

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function kands_preprocess_page(&$variables, $hook) {
	//	Recreates the handling for logo or favicon, but does it for the
	//	splash image on the front page.  All processing is handled here so
	//	the template file can stay clean (page--front.tpl.php)
	global $base_url;
	if (theme_get_setting('default_splash')) {
		$variables['splash'] = $base_url . "/" . path_to_theme() . "/images/splash-default.png";
	} elseif (theme_get_setting('splash_path')) {
		$variables['splash'] = $base_url . "/" . theme_get_setting('splash_path');
	} else {
		$variables['splash'] = '';
	}
}
