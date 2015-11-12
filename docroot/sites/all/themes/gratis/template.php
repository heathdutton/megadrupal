<?php
/**
 * @file
 * Template.php provides theme functions & overrides
 */

/**
 * Break up template.php into fine grained includes.
 */
global $theme_key, $path_to_gratis;
$theme_key = $GLOBALS['theme_key'];
$path_to_gratis = drupal_get_path('theme', 'gratis');

/**
 * The includes.
 */
include_once($path_to_gratis . '/inc/preprocess.inc');
include_once($path_to_gratis . '/inc/theme.inc');
include_once($path_to_gratis . '/inc/alter.inc');
include_once($path_to_gratis . '/inc/menu.inc');
