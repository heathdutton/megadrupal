<?php

/* Add Default Styling */

drupal_add_css(drupal_get_path('theme', 'blacksea') . '/css/default.css', array('weight' => CSS_THEME, 'type' => 'file'));

/* Custom Styles */

$style = theme_get_setting('style');

switch ($style) {
	case 0:
		drupal_add_js(drupal_get_path('theme', 'blacksea') . "/js/scripts-style1.js"); 
		drupal_add_css(drupal_get_path('theme', 'blacksea') . '/css/style1.css', array('weight' => CSS_THEME, 'type' => 'file'));
		break;
	default:
  	drupal_add_js(drupal_get_path('theme', 'blacksea') . "/js/scripts-style1.js");
		drupal_add_css(drupal_get_path('theme', 'blacksea') . '/css/style1.css', array('weight' => CSS_THEME, 'type' => 'file'));
}

/* Get Custom CSS */

$css = theme_get_setting('css');

  if  ($css == 1) {
    drupal_add_css(drupal_get_path('theme', 'blacksea') . '/css/custom.css', array('weight' => CSS_THEME, 'type' => 'file')); 
  } 
