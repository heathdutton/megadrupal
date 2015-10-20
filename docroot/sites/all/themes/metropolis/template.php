<?php

if (drupal_is_front_page()) {
	drupal_add_js(drupal_get_path('theme', 'metropolis') . '/scripts/jquery.cycle.all.min.js');
}

function metropolis_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_js(drupal_get_path('theme', 'metropolis') . '/scripts/jquery.cycle.all.min.js');
}

function metropolis_preprocess_html(&$vars) {
	// Add conditional CSS for IE6.
	drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

function metropolis_preprocess_page(&$vars, $hook) {
	$notice = '<div style="clear:both; font-size:11px; text-align:center;"><em>Theme provided by <a href="http://drupalbycity.com" target="_blank">DrupalByCity</a> under GLP license from <a href="http://www.comprendo.dk">Comprendo</a></em></div>';
	$vars['notice'] = $notice;
}